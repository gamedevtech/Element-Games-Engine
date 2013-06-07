#include "../AssimpInterface.h"

#include <iostream>

namespace EG{
    namespace Media{
        AssimpInterface::AssimpInterface(EG::Game::Scene *_scene){
            scene = _scene;
            count = 0;
            has_animations = false;
        }

        AssimpInterface::~AssimpInterface(void){
            //delete ai_scene;
        }

        bool AssimpInterface::Load(std::string _file_path){
            std::cout << "Loading " << file_path << std::endl;

            // Use Assimp to Load The Scene
            Assimp::Importer importer;
            importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
            file_path = _file_path;
            ai_scene = importer.ReadFile(file_path, /*aiProcess_CalcTangentSpace | */aiProcess_Triangulate | /*aiProcess_JoinIdenticalVertices | */aiProcess_SortByPType);
            if (!ai_scene){
                return false;
            }

            // Load Animations
            if (ai_scene->HasAnimations()) {
                has_animations = true;
                // Find Bones in Meshes
                LoadBones(ai_scene);
                // Build Skeleton
                LoadSkeleton(ai_scene);
                // Load Keyframe Information
                animations = new EG::Dynamics::Animations();
                animations->SetBindPose(bind_pose_skeleton);
                for (unsigned int i = 0; i < ai_scene->mNumAnimations; i++) {
                    animations->Add(LoadAnimation(ai_scene->mAnimations[i], i));
                }
            }

            // Load Meshes and Materials
            for (unsigned int i = 0; i < ai_scene->mNumMeshes; i++){
                const struct aiMesh *mesh = ai_scene->mMeshes[i];
                const struct aiMaterial *material = ai_scene->mMaterials[mesh->mMaterialIndex];

                LoadMaterial(material, i);
                // TODO: Go back and find this meshes node and get it's local transformation
                LoadMesh(mesh, i);
                count += 1;
            }

            std::cout << "Done." << std::endl;
            return true;
        }

        // Skeletal Functions
        void AssimpInterface::LoadBones(const aiScene *ai_scene){
            unsigned int bone_id = 0;
            for (unsigned int mesh_index = 0; mesh_index < ai_scene->mNumMeshes; mesh_index++) {
                aiMesh *ai_mesh = ai_scene->mMeshes[mesh_index];
                if (ai_mesh->HasBones()) {
                    for (unsigned int bone_index = 0; bone_index < ai_mesh->mNumBones; bone_index++) {
                        aiBone *ai_bone = ai_mesh->mBones[bone_index];
                        std::string bone_name(ai_bone->mName.data);
                        if (bone_name_map.count(bone_name) < 1) {
                            bone_name_map[bone_name] = bone_id;
                            bone_names.push_back(bone_name);
                            bind_pose_map[bone_id] = ConvertMatrix(ai_bone->mOffsetMatrix);
                            bone_id += 1;
                        }
                    }
                }
            }
            bone_count = bone_id;
        }

        aiNode *AssimpInterface::FindRootBone(const aiScene *ai_scene) {
            std::vector<std::string>::iterator bi = bone_names.begin();
            std::string root_bone;
            aiNode *root_bone_node;
            unsigned int lowest_separation = 99999999;
            while (bi != bone_names.end()) {
                std::string name = (*bi);
                unsigned int separation = 0;
                aiNode *bone_node = ai_scene->mRootNode->FindNode(name.c_str());
                aiNode *original_bone_node = bone_node;
                while (bone_node != NULL) {
                    separation += 1;
                    bone_node = bone_node->mParent;
                }
                if (separation < lowest_separation) {
                    lowest_separation = separation;
                    root_bone = name;
                    root_bone_node = original_bone_node;
                }
                ++bi;
            }
            return root_bone_node;
        }

        void AssimpInterface::LoadSkeleton(const aiScene *ai_scene){
            bind_pose_skeleton = new EG::Dynamics::Skeleton();

            aiNode *root_bone_node = FindRootBone(ai_scene);

            // Now Recurively Add Child Bones
            BuildSkeleton(root_bone_node, NULL);
            //bind_pose_skeleton->PrintRecursive();
        }

        void AssimpInterface::BuildSkeleton(aiNode *current, EG::Dynamics::Bone *parent) {
            unsigned int bone_id = bone_name_map[std::string(current->mName.data)];
            glm::mat4 bone_trans = bind_pose_map[bone_id];
            //glm::mat4 transformation = ConvertMatrix(current->mTransformation);
            EG::Dynamics::Bone *bone = new EG::Dynamics::Bone(bone_id, bone_trans/*transformation*/);
            if (parent == NULL) {
                bind_pose_skeleton->SetRoot(bone);
            } else {
                parent->AddChild(bone);
            }
            bind_pose_skeleton->AddBone(bone);
            for (unsigned int child_index = 0; child_index < current->mNumChildren; child_index++) {
                BuildSkeleton(current->mChildren[child_index], bone);
            }
        }

        EG::Dynamics::Animation *AssimpInterface::LoadAnimation(const aiAnimation *ai_animation, unsigned int index) {
            std::string animation_name(ai_animation->mName.data);
            double duration = ai_animation->mDuration;
            double ticks_per_second = ai_animation->mTicksPerSecond;
            std::cout << "Animation: " << animation_name << " " << duration << " " << ticks_per_second << std::endl;

            EG::Dynamics::Animation *animation = new EG::Dynamics::Animation(animation_name, duration);
            animation->SetBoneCount(bind_pose_skeleton->GetBones()->size());

            for (unsigned int bone_index = 0; bone_index < ai_animation->mNumChannels; bone_index++) {
                aiNodeAnim *channel = ai_animation->mChannels[bone_index];
                std::string bone_name(channel->mNodeName.data);
                unsigned int bone_id = bone_name_map[bone_name];

                for (unsigned int i = 0; i < channel->mNumPositionKeys; i++) {
                    aiVectorKey pos_key = channel->mPositionKeys[i];
                    animation->AddBonePosition(bone_id, float(pos_key.mTime), glm::vec3(pos_key.mValue.x, pos_key.mValue.y, pos_key.mValue.z));
                }
                for (unsigned int i = 0; i < channel->mNumRotationKeys; i++) {
                    aiQuatKey rot_key = channel->mRotationKeys[i];
                    animation->AddBoneRotation(bone_id, float(rot_key.mTime), glm::quat(rot_key.mValue.w, rot_key.mValue.x, rot_key.mValue.y, rot_key.mValue.z));
                }
                for (unsigned int i = 0; i < channel->mNumScalingKeys; i++) {
                    aiVectorKey scale_key = channel->mScalingKeys[i];
                    animation->AddBoneScaling(bone_id, float(scale_key.mTime), glm::vec3(scale_key.mValue.x, scale_key.mValue.y, scale_key.mValue.z));
                }
            }

            return animation;
        }
        ///////////////////////

        void AssimpInterface::LoadMesh(const aiMesh *ai_mesh, unsigned int index){
            if (ai_mesh->HasFaces()){
                std::string mesh_name = file_path + std::string(ai_mesh->mName.data);
                EG::Graphics::Triangle *faces = new EG::Graphics::Triangle[ai_mesh->mNumFaces];

                // Skeletal Structure
                std::map<unsigned int, std::vector<std::pair<float, float> > > vertex_weights;
                if (ai_mesh->HasBones()) {
                    for (unsigned int i = 0; i < ai_mesh->mNumBones; i++) {
                        const struct aiBone *bone = ai_mesh->mBones[i];
                        glm::mat4 offset_transform;
                        for (unsigned int i = 0; i < 4; i++) {
                            for (unsigned int j = 0; j < 4; j++) {
                                offset_transform[i][j] = bone->mOffsetMatrix[i][j];
                            }
                        }
                        std::string bone_name_str = std::string(bone->mName.data);
                        //bone_transformations[index][bone_name_map[bone_name_str]] = offset_transform;
                        for (unsigned int weight_index = 0; weight_index < bone->mNumWeights; weight_index++) {
                            aiVertexWeight weight = bone->mWeights[weight_index];
                            vertex_weights[weight.mVertexId].push_back(std::pair<float, float>(float(bone_name_map[bone_name_str]), weight.mWeight));
                        }
                    }
                }

                bool has_skeleton = false;
                for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++){
                    const aiFace *ai_face = &(ai_mesh->mFaces[i]);
                    // For each vertex which is garanteed to be 3 because we always triangulate!
                    for (unsigned int v = 0; v < /*3*/ai_face->mNumIndices; v++){
                        unsigned int vertex_index = ai_face->mIndices[v];
                        aiVector3D vertex = ai_mesh->mVertices[vertex_index];
                        aiVector3D texcoord = ai_mesh->mTextureCoords[0][vertex_index];

                        faces[i].vertices[2 - v] = glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
                        faces[i].texcoords[2 - v] = glm::vec4(texcoord.x, texcoord.y, 1.0f, 1.0f);
                        if (ai_mesh->HasBones()) {
                            has_skeleton = true;
                            faces[i].weights[2 - v] = vertex_weights[vertex_index];
                        }
                    }
                }
                EG::Graphics::TriangleMesh *tmesh = new EG::Graphics::TriangleMesh(ai_mesh->mNumFaces, faces, /*false, false*/true, true, true, true, true, true, has_skeleton);
                std::cout << tmesh->GetTriangles()->at(0).tangents[0].x << ' ' << tmesh->GetTriangles()->at(0).tangents[0].y << ' ' << tmesh->GetTriangles()->at(0).tangents[0].z << ' ' << tmesh->GetTriangles()->at(0).tangents[0].w << std::endl;
                std::cout << tmesh->GetTriangles()->at(0).bitangents[0].x << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].y << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].z << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].w << std::endl;
                scene->GetMeshManager()->Add(mesh_name, new EG::Graphics::Mesh(tmesh));
                meshes[index] = mesh_name;
            }
        }

        void AssimpInterface::LoadMaterial(const aiMaterial *ai_material, unsigned int index){
            EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();
            // Texture Loading
            aiString path;
            std::string texture_path;
            // Diffuse
            unsigned int diffuse_texture_count = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
            if (diffuse_texture_count > 0){
                for (unsigned int mi = 0; mi < diffuse_texture_count; mi++){
                    ai_material->GetTexture(aiTextureType_DIFFUSE, mi, &path);
                }
                texture_path = std::string("Assets/Models/") + std::string(path.data);
                if (!(scene->GetTextureManager()->HasTexture(texture_path))){
                    scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
                }
                material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, texture_path);
            }
            // Normal
            unsigned int normal_texture_count = ai_material->GetTextureCount(aiTextureType_NORMALS);
            if (normal_texture_count > 0){
                for (unsigned int mi = 0; mi < normal_texture_count; mi++){
                    ai_material->GetTexture(aiTextureType_NORMALS, mi, &path);
                }
                texture_path = std::string("Assets/Models/") + std::string(path.data);
                if (!(scene->GetTextureManager()->HasTexture(texture_path))){
                    scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
                }
                material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, texture_path);
            }
            // Height
            unsigned int height_texture_count = ai_material->GetTextureCount(aiTextureType_HEIGHT);
            if (height_texture_count > 0){
                for (unsigned int mi = 0; mi < height_texture_count; mi++){
                    ai_material->GetTexture(aiTextureType_HEIGHT, mi, &path);
                }
                texture_path = std::string("Assets/Models/") + std::string(path.data);
                if (!(scene->GetTextureManager()->HasTexture(texture_path))){
                    scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
                }
                material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, texture_path);
            }
            // Specular
            unsigned int specular_texture_count = ai_material->GetTextureCount(aiTextureType_SPECULAR);
            if (specular_texture_count > 0){
                for (unsigned int mi = 0; mi < specular_texture_count; mi++){
                    ai_material->GetTexture(aiTextureType_SPECULAR, mi, &path);
                }
                texture_path = std::string("Assets/Models/") + std::string(path.data);
                if (!(scene->GetTextureManager()->HasTexture(texture_path))){
                    scene->GetTextureManager()->AddTexture(texture_path, new EG::Graphics::Texture(texture_path));
                }
                material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, texture_path);
            }
            materials[index] = material;
        }

        glm::mat4 AssimpInterface::ConvertMatrix(aiMatrix4x4 in) {
            glm::mat4 out;
//             for (int i = 0; i < 4; i++) {
//                 for (int j = 0; j < 4; j++) {
//                     out[j][i] = float(in[i][j]);
//                 }
//             }
            out[0][0] = in.a1;
            out[1][0] = in.a2;
            out[2][0] = in.a3;
            out[3][0] = in.a4;
            out[0][1] = in.b1;
            out[1][1] = in.b2;
            out[2][1] = in.b3;
            out[3][1] = in.b4;
            out[0][2] = in.c1;
            out[1][2] = in.c2;
            out[2][2] = in.c3;
            out[3][2] = in.c4;
            out[0][3] = in.d1;
            out[1][3] = in.d2;
            out[2][3] = in.d3;
            out[3][3] = in.d4;
            return out;
        }
    }
}
