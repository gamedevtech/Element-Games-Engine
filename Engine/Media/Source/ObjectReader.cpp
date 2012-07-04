#include "../ObjectReader.h"
#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Game/ObjectControlAttribute.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/RenderingMaterial.h"
#include <fcntl.h>

namespace EG{
    namespace Media{
        ObjectReader::ObjectReader(void){
            object = NULL;
        }

        ObjectReader::~ObjectReader(void){
            //
        }

        bool ObjectReader::Read(std::string file_path, EG::Game::Scene *scene) {
            in.open(file_path.c_str(), std::ios::binary);

            // Read Object ID and Create Object
            unsigned int string_size = ReadUInt();
            std::string object_id = ReadString(string_size);
            object = new EG::Game::Object(object_id);
            

            // Read Transformation
            glm::mat4 transformation = ReadMat4();
            object->AddAttribute(new EG::Game::ObjectAttributeBasicTransformation(transformation));

            // Mesh/Materials
            unsigned int count = ReadUInt();
            for (unsigned int i = 0; i < count; i++) {
                EG::Graphics::RenderingMaterial *material = new EG::Graphics::RenderingMaterial();

                // Decal
                bool has_decal = ReadBool();
                if (has_decal) {
                    string_size = ReadUInt();
                    std::string decal_path = ReadString(string_size);
                    decal_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(decal_path);
                    if (decal_path.size() > 2){
                        if (!(scene->GetTextureManager()->HasTexture(decal_path))){
                            scene->GetTextureManager()->AddTexture(decal_path, new EG::Graphics::Texture(decal_path));
                        }
                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL, decal_path);
                    }
                }

                // Normal
                bool has_normal = ReadBool();
                if (has_normal) {
                    string_size = ReadUInt();
                    std::string normal_path = ReadString(string_size);
                    normal_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(normal_path);
                    if (normal_path.size() > 2){
                        if (!(scene->GetTextureManager()->HasTexture(normal_path))){
                            scene->GetTextureManager()->AddTexture(normal_path, new EG::Graphics::Texture(normal_path));
                        }
                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL, normal_path);
                    }
                }

                // Height
                bool has_height = ReadBool();
                if (has_height) {
                    string_size = ReadUInt();
                    std::string height_path = ReadString(string_size);
                    height_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(height_path);
                    if (height_path.size() > 2){
                        if (!(scene->GetTextureManager()->HasTexture(height_path))){
                            scene->GetTextureManager()->AddTexture(height_path, new EG::Graphics::Texture(height_path));
                        }
                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT, height_path);
                    }
                }

                // Specular
                bool has_specular = ReadBool();
                if (has_specular) {
                    string_size = ReadUInt();
                    std::string specular_path = ReadString(string_size);
                    specular_path = EG::Utility::StringMethods::RemoveSpecialCharactersFromPathString(specular_path);
                    if (specular_path.size() > 2){
                        if (!(scene->GetTextureManager()->HasTexture(specular_path))){
                            scene->GetTextureManager()->AddTexture(specular_path, new EG::Graphics::Texture(specular_path));
                        }
                        material->SetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR, specular_path);
                    }
                }

                material->SetLit(ReadBool());
                material->SetCastsShadows(ReadBool());
                material->SetTranslucent(ReadBool());
                material->SetAmbient(ReadFloat());
                material->SetDiffuse(ReadFloat());
                material->SetSpecular(ReadFloat());
                material->SetSpecularExponent(ReadFloat());
                material->SetColor(ReadVec4());

                // Mesh Data
                string_size = ReadUInt();
                std::string mesh_id = ReadString(string_size);
                unsigned int vertex_count = ReadUInt();
                unsigned int strid = ReadUInt();

                float *vertices, *normals, *tex_coords, *binormals, *bitangents, *weights, *bone_indices;
                bool has_vertices = ReadBool();
                if (has_vertices) {
                    vertices = ReadFloatV(vertex_count * 4);
                }
                bool has_normals = ReadBool();
                if (has_normals) {
                    normals = ReadFloatV(vertex_count * 4);
                }
                bool has_tex_coords = ReadBool();
                if (has_tex_coords) {
                    tex_coords = ReadFloatV(vertex_count * 4);
                }
                bool has_binormals = ReadBool();
                if (has_binormals) {
                    binormals = ReadFloatV(vertex_count * 4);
                }
                bool has_bitangents = ReadBool();
                if (has_bitangents) {
                    bitangents = ReadFloatV(vertex_count * 4);
                }
                bool has_skeleton = ReadBool();
                if (has_skeleton) {
                    weights = ReadFloatV(vertex_count * 4);
                    bone_indices = ReadFloatV(vertex_count * 4);
                }

                EG::Graphics::Mesh *mesh = new EG::Graphics::Mesh(vertex_count, 4, vertices, has_vertices, tex_coords,
                                                                  has_tex_coords, normals, has_normals, binormals,
                                                                  has_binormals, bitangents, has_bitangents, weights,
                                                                  bone_indices, has_skeleton);
                scene->GetMeshManager()->Add(mesh_id, mesh);
                object->AddAttribute(new EG::Game::ObjectAttributeRenderingMesh(mesh_id, material));
            }

            // Read Animations
            bool has_animations = ReadBool();
            if (has_animations) {
                EG::Dynamics::Animations *animations = new EG::Dynamics::Animations;

                // Build Bind Pose
                unsigned int bone_count = ReadUInt();
                EG::Dynamics::Skeleton *bind_pose = new EG::Dynamics::Skeleton;
                std::map<unsigned int, unsigned int> parent_relations;
                for (unsigned int i = 0; i < bone_count; i++) {
                    unsigned int bone_id = ReadUInt();
                    unsigned int parent_bone_id = ReadUInt();
                    glm::mat4 offset = ReadMat4();
                    parent_relations[bone_id] = parent_bone_id;
                    bind_pose->AddBone(new EG::Dynamics::Bone(bone_id, offset));
                }
                std::map<unsigned int, unsigned int>::iterator riter = parent_relations.begin();
                while (riter != parent_relations.end()) {
                    unsigned int bone_id = riter->first;
                    unsigned int parent_bone_id = riter->second;
                    EG::Dynamics::Bone *bone = bind_pose->GetBone(bone_id);
                    if (parent_bone_id > 999) {
                        bind_pose->SetRoot(bone);
                    } else {
                        EG::Dynamics::Bone *parent_bone = bind_pose->GetBone(parent_bone_id);
                        parent_bone->AddChild(bone);
                    }
                    ++riter;
                }
                animations->SetBindPose(bind_pose);

                // Load Animations
                unsigned int animation_count = ReadUInt();
                for (unsigned int i = 0; i < animation_count; i++) {
                    string_size = ReadUInt();
                    std::string animation_name = ReadString(string_size);
                    float duration = ReadFloat();
                    EG::Dynamics::Animation *animation = new EG::Dynamics::Animation(animation_name, duration);
                    unsigned int bone_count = ReadUInt();
                    animation->SetBoneCount(bone_count);
                    for (unsigned int bi = 0; bi < bone_count; bi++) {
                        unsigned int bone_id = ReadUInt();

                        // Positions
                        unsigned int position_count = ReadUInt();
                        for (unsigned int pi = 0; pi < position_count; pi++) {
                            float time = ReadFloat();
                            glm::vec3 value = ReadVec3();
                            animation->AddBonePosition(bone_id, time, value);
                        }

                        // Scalings
                        unsigned int scaling_count = ReadUInt();
                        for (unsigned int si = 0; si < scaling_count; si++) {
                            float time = ReadFloat();
                            glm::vec3 value = ReadVec3();
                            animation->AddBoneScaling(bone_id, time, value);
                        }

                        // Rotations
                        unsigned int rotation_count = ReadUInt();
                        for (unsigned int ri = 0; ri < rotation_count; ri++) {
                            float time = ReadFloat();
                            glm::quat value = ReadQuat();
                            animation->AddBoneRotation(bone_id, time, value);
                        }
                    }
                    animations->Add(animation);
                }

                EG::Dynamics::AnimationState *animation_state = new EG::Dynamics::AnimationState(animations);
                object->AddAttribute(new EG::Game::ObjectAttributeControlAnimationState(animation_state));
            }

            in.close();
            return true;
        }

        std::string ObjectReader::ReadString(unsigned int size) {
            characters = new char[size];
            in.read(characters, sizeof(char) * size);
            std::string out(characters, size);
            delete []characters;
            return out;
        }
        bool ObjectReader::ReadBool(void) {
            bool out;
            in.read((char *)&out, sizeof(bool));
            return out;
        }
        unsigned int ObjectReader::ReadUInt(void) {
            unsigned int out;
            in.read((char *)&out, sizeof(unsigned int));
            return out;
        }
        float ObjectReader::ReadFloat(void) {
            float out;
            in.read((char *)&out, sizeof(float));
            return out;
        }
        float *ObjectReader::ReadFloatV(unsigned int size) {
            float *out = new float[size];
            in.read((char *)out, sizeof(float) * size);
            return out;
        }
        glm::vec3 ObjectReader::ReadVec3(void) {
            float tmp[3];
            in.read((char *)tmp, sizeof(float) * 3);
            return glm::vec3(tmp[0], tmp[1], tmp[2]);
        }
        glm::vec4 ObjectReader::ReadVec4(void) {
            float tmp[4];
            in.read((char *)tmp, sizeof(float) * 4);
            return glm::vec4(tmp[0], tmp[1], tmp[2], tmp[3]);
        }
        glm::quat ObjectReader::ReadQuat(void) {
            float tmp[4];
            tmp[0] = ReadFloat();
            tmp[1] = ReadFloat();
            tmp[2] = ReadFloat();
            tmp[3] = ReadFloat();
            return glm::quat(tmp[3], tmp[0], tmp[1], tmp[2]);
        }
        glm::mat4 ObjectReader::ReadMat4(void) {
            float tmp[16];
            in.read((char *)tmp, sizeof(float) * 16);
            glm::mat4 out(tmp[0], tmp[1], tmp[2], tmp[3],
                           tmp[4], tmp[5], tmp[6], tmp[7],
                           tmp[8], tmp[9], tmp[10], tmp[11],
                           tmp[12], tmp[13], tmp[14], tmp[15]);
            return out;
        }

        EG::Game::Object *ObjectReader::GetLoadedObject(void){
            return object;
        }
    }
}

