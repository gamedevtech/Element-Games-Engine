#include "../AssimpInterface.h"

#include <iostream>

namespace EG{
    namespace Media{
        AssimpInterface::AssimpInterface(EG::Game::Scene *_scene){
            scene = _scene;
            count = 0;
        }

        AssimpInterface::~AssimpInterface(void){
            //delete ai_scene;
        }

        bool AssimpInterface::Load(std::string _file_path){
            std::cout << "Loading " << file_path << std::endl;

            Assimp::Importer importer;
            file_path = _file_path;
            ai_scene = importer.ReadFile(file_path, /*aiProcess_CalcTangentSpace | */aiProcess_Triangulate | /*aiProcess_JoinIdenticalVertices | */aiProcess_SortByPType);
            if (!ai_scene){
                return false;
            }

            if (ai_scene->HasAnimations()) {
                LoadSkeleton(ai_scene);
            }

            for (unsigned int i = 0; i < ai_scene->mNumMeshes; i++){
                const struct aiMesh *mesh = ai_scene->mMeshes[i];
                const struct aiMaterial *material = ai_scene->mMaterials[mesh->mMaterialIndex];

                LoadMaterial(material, i);
                LoadMesh(mesh, i);
                count += 1;
            }

            if (ai_scene->HasAnimations()) {
                for (unsigned int i = 0; i < ai_scene->mNumAnimations; i++) {
                    LoadAnimation(ai_scene->mAnimations[i], i);
                }
            }

            std::cout << "Done." << std::endl;

            return true;
        }

        void AssimpInterface::LoadSkeleton(const aiScene *ai_scene){
            // Get Bone List, store in bone_name_map
            unsigned int bone_id = 0;
            for (unsigned int i = 0; i < ai_scene->mNumAnimations; i++) {
                aiAnimation *ai_animation = ai_scene->mAnimations[i];
                for (unsigned int j = 0; j < ai_animation->mNumChannels; j++) {
                    aiNodeAnim *channel = ai_animation->mChannels[j];
                    std::string bone_name(channel->mNodeName.data);
                    if (bone_name_map.count(bone_name) < 1) {
                        bone_name_map[bone_name] = bone_id;
                        bone_names.push_back(bone_name);
                        std::cout << "Found Bone: " << bone_name << std::endl;
                        bone_id += 1;
                    }
                }
            }
            bone_count = bone_id;

            bind_pose_skeleton = new EG::Dynamics::Skeleton();
            RecursiveLoadSkeleton(ai_scene->mRootNode, NULL);
            /*std::vector<EG::Dynamics::Bone *>::iterator bi = bind_pose_skeleton->GetBones()->begin();
            while (bi != bind_pose_skeleton->GetBones()->end()) {
                std::cout << "Bone Array: " << (*bi)->GetId() << std::endl;
                bi++;
            }*/
            bind_pose_skeleton->PrintRecursive();
        }

        void AssimpInterface::RecursiveLoadSkeleton(const aiNode *parent_node, EG::Dynamics::Bone *parent_bone) {
            std::string node_name(parent_node->mName.data);
            //std::cout << "Node Name: " << node_name << std::endl;
            // compare node name against bones
            EG::Dynamics::Bone *this_bone = NULL;
            if (bone_name_map.count(node_name) > 0) {
                unsigned int bone_id = bone_name_map[node_name];
                std::cout << "Is Bone: " << bone_id << std::endl;
                aiMatrix4x4 ai_trans = parent_node->mTransformation;
                glm::mat4 trans = glm::mat4(1.0f);
                for (unsigned int i = 0; i < 4; i++) {
                    for (unsigned int j = 0; j < 4; j++) {
                        trans[i][j] = parent_node->mTransformation[i][j];
                    }
                }
                this_bone = new EG::Dynamics::Bone(bone_id, trans);
                if (parent_bone == NULL) {
                    std::cout << "HERE" << std::endl;
                    bind_pose_skeleton->SetRoot(this_bone);
                } else {
                    parent_bone->AddChild(this_bone);
                }
                bind_pose_skeleton->AddBone(this_bone);
                for (unsigned int i = 0; i < parent_node->mNumChildren; i++) {
                    //std::cout << "Child: " << parent_node->mChildren[i]->mName.data << std::endl;
                    RecursiveLoadSkeleton(parent_node->mChildren[i], this_bone);
                }
            }
            if (parent_bone == NULL) {
                for (unsigned int i = 0; i < parent_node->mNumChildren; i++) {
                    //std::cout << "Child: " << parent_node->mChildren[i]->mName.data << std::endl;
                    RecursiveLoadSkeleton(parent_node->mChildren[i], NULL);
                }
            }
        }

        void AssimpInterface::LoadMesh(const aiMesh *ai_mesh, unsigned int index){
            if (ai_mesh->HasFaces()){
                std::string mesh_name = file_path + std::string(ai_mesh->mName.data);
                EG::Graphics::Triangle *faces = new EG::Graphics::Triangle[ai_mesh->mNumFaces];

                // Skeletal Structure
                std::map<unsigned int, std::vector<std::pair<unsigned int, float> > > vertex_weights;
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
                            vertex_weights[weight.mVertexId].push_back(std::pair<unsigned int, float>(bone_name_map[bone_name_str], weight.mWeight));
                        }
                    }
                }

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
                            faces[i].weights[2 - v] = vertex_weights[vertex_index];
                        }
                    }
                }
                EG::Graphics::TriangleMesh *tmesh = new EG::Graphics::TriangleMesh(ai_mesh->mNumFaces, faces, /*false, false*/true, true, true, true, true, true);
                std::cout << tmesh->GetTriangles()->at(0).tangents[0].x << ' ' << tmesh->GetTriangles()->at(0).tangents[0].y << ' ' << tmesh->GetTriangles()->at(0).tangents[0].z << ' ' << tmesh->GetTriangles()->at(0).tangents[0].w << std::endl;
                std::cout << tmesh->GetTriangles()->at(0).bitangents[0].x << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].y << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].z << ' ' << tmesh->GetTriangles()->at(0).bitangents[0].w << std::endl;
                scene->GetMeshManager()->Add(mesh_name, new EG::Graphics::Mesh(tmesh));
                meshes.Set(index, mesh_name);
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
            materials.Set(index, material);
        }

        void AssimpInterface::LoadAnimation(const aiAnimation *ai_animation, unsigned int index) {
            std::string animation_name(ai_animation->mName.data);
            double duration = ai_animation->mDuration;
            //std::cout << "Animation: " << animation_name << " " << duration << std::endl;
            // NOTE: What are Mesh Channels For?
            for (unsigned int j = 0; j < ai_animation->mNumChannels; j++) {
                aiNodeAnim *channel = ai_animation->mChannels[j];
                std::string bone_name(channel->mNodeName.data);
                unsigned int bone_id = bone_name_map[bone_name];
                //std::cout << "Bone: " << bone_name << std::endl;
                //std::cout << channel->mNumPositionKeys << ' ' << channel->mNumRotationKeys << ' ' << channel->mNumScalingKeys << std::endl;
            }
        }
    }
}
