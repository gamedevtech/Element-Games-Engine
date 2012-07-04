#include "../ObjectWriter.h"

#include "../../Game/ObjectRenderingAttribute.h"
#include "../../Game/ObjectBasicAttribute.h"
#include "../../Game/ObjectEmissionAttribute.h"
#include "../../Game/ObjectControlAttribute.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/RenderingMaterial.h"
#include "../../Graphics/GraphicsSubsystem.h"
#include "../../Utility/StringMethods.h"

namespace EG{
    namespace Media{
        ObjectWriter::ObjectWriter(EG::Game::Object *_object, EG::Game::Scene *_scene, std::string _object_name, std::string _images_output_path, std::string _model_output_path){
            scene = _scene;
            object = _object;
            object_name = _object_name;
            images_output_path = _images_output_path;
            model_output_path = _model_output_path;
        }
        ObjectWriter::~ObjectWriter(void){
            //
        }

        void ObjectWriter::Write(std::string file_name) {
            out.open((model_output_path + file_name).c_str(), std::ios::binary);

            // Write Object Name
            WriteUInt(object_name.size());
            WriteString(object_name);

            // Write Basic Transformation
            std::vector<EG::Game::ObjectAttribute *> *transformation_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION);
            EG::Game::ObjectAttributeBasicTransformation *transformation_attribute = static_cast<EG::Game::ObjectAttributeBasicTransformation *>(transformation_attributes->at(0));
            WriteMat4(transformation_attribute->GetTransformation());

            // Mesh/Material Attributes
            std::vector<EG::Game::ObjectAttribute *> *mesh_attributes = object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH);
            std::vector<EG::Game::ObjectAttribute *>::iterator mesh_attribute_iterator = mesh_attributes->begin();
            WriteUInt(mesh_attributes->size());
            while (mesh_attribute_iterator != mesh_attributes->end()){
                EG::Game::ObjectAttributeRenderingMesh *mesh_attribute = static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*mesh_attribute_iterator);
                EG::Graphics::RenderingMaterial *material = mesh_attribute->GetMaterial();
                EG::Graphics::Mesh *mesh = scene->GetMeshManager()->Get(mesh_attribute->GetMeshId());

                // Material
                if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)) {
                    EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL));
                    std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
                    CopyFile(texture->GetFilePath(), image_out_path.c_str());
                    WriteBool(true);
                    WriteUInt(image_out_path.size());
                    WriteString(image_out_path);
                }else{
                    WriteBool(false);
                }
                if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)){
                    EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL));
                    std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
                    CopyFile(texture->GetFilePath(), image_out_path.c_str());
                    WriteBool(true);
                    WriteUInt(image_out_path.size());
                    WriteString(image_out_path);
                }else{
                    WriteBool(false);
                }
                if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)){
                    EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT));
                    std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
                    CopyFile(texture->GetFilePath(), image_out_path.c_str());
                    WriteBool(true);
                    WriteUInt(image_out_path.size());
                    WriteString(image_out_path);
                }else{
                    WriteBool(false);
                }
                if (material->HasTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR)){
                    EG::Graphics::Texture *texture = scene->GetTextureManager()->GetTexture(material->GetTexture(EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR));
                    std::string image_out_path = (images_output_path + EG::Utility::StringMethods::GetFilenameFromPath(texture->GetFilePath()));
                    CopyFile(texture->GetFilePath(), image_out_path.c_str());
                    WriteBool(true);
                    WriteUInt(image_out_path.size());
                    WriteString(image_out_path);
                }else{
                    WriteBool(false);
                }
                if (material->GetLit()){
                    WriteBool(true);
                }else{
                    WriteBool(false);
                }
                if (material->GetCastsShadows()){
                    WriteBool(true);
                }else{
                    WriteBool(false);
                }
                if (material->GetTranslucent()){
                    WriteBool(true);
                }else{
                    WriteBool(false);
                }
                WriteFloat(material->GetAmbient());
                WriteFloat(material->GetDiffuse());
                WriteFloat(material->GetSpecular());
                WriteFloat(material->GetSpecularExponent());
                WriteVec4(material->GetColor());

                // Mesh
                WriteUInt(mesh_attribute->GetMeshId().size());
                WriteString(mesh_attribute->GetMeshId());
                WriteUInt(mesh->GetVertexCount());
                WriteUInt(mesh->GetStride());
                if (mesh->HasVertices()){
                    WriteBool(true);
                    float *d = mesh->GetVertices();
                    WriteFloatV(d, mesh->GetVertexCount() * 4);
                } else {
                    WriteBool(false);
                }
                if (mesh->HasNormals()){
                    WriteBool(true);
                    float *d = mesh->GetNormals();
                    WriteFloatV(d, mesh->GetVertexCount() * 4);
                } else {
                    WriteBool(false);
                }
                if (mesh->HasTexCoords()){
                    WriteBool(true);
                    float *d = mesh->GetTexCoords();
                    WriteFloatV(d, mesh->GetVertexCount() * 4);
                } else {
                    WriteBool(false);
                }
                if (mesh->HasBinormals()){
                    WriteBool(true);
                    float *d = mesh->GetBinormals();
                    WriteFloatV(d, mesh->GetVertexCount() * 4);
                } else {
                    WriteBool(false);
                }
                if (mesh->HasBitangents()){
                    WriteBool(true);
                    float *d = mesh->GetBitangents();
                    WriteFloatV(d, mesh->GetVertexCount() * 4);
                } else {
                    WriteBool(false);
                }
                if (mesh->HasSkeleton()) {
                    WriteBool(true);
                    float *d = mesh->GetWeights();
                    WriteFloatV(d, mesh->GetVertexCount() * 4);
                    float *di = mesh->GetWeightVertexIndices();
                    WriteFloatV(di, mesh->GetVertexCount() * 4);
                } else {
                    WriteBool(false);
                }

                // Animations
                if (object->HasAttributesOfType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_ANIMATION)) {
                    WriteBool(true); // Has Animations
                    
                    EG::Game::ObjectAttributeControlAnimationState *animation_attribute = static_cast<EG::Game::ObjectAttributeControlAnimationState *>(object->GetAttributesByType(EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_ANIMATION)->at(0));
                    EG::Dynamics::AnimationState *animation_state = animation_attribute->GetAnimationState();
                    EG::Dynamics::Animations *animations = animation_state->GetAnimations();

                    // Bind Pose Skeleton
                    EG::Dynamics::Skeleton *bind_pose = animations->GetBindPose();
                    std::map<unsigned int, EG::Dynamics::Bone *> *bones = bind_pose->GetBoneMap();
                    unsigned int bone_count = bind_pose->GetBones()->size();
                    WriteUInt(bone_count);
                    for (unsigned int i = 0; i < bone_count; i++) {
                        EG::Dynamics::Bone *bone = (*bones)[i];
                        WriteUInt(i); // Bone ID
                        if (bone->GetParent() != NULL) {
                            WriteUInt(bone->GetParent()->GetId());
                        } else {
                            WriteUInt(99999);
                        }
                        WriteMat4(bone->GetOffset());
                    }

                    // Animations
                    std::vector<std::string> animation_names = animations->GetAnimationNames();
                    WriteUInt(animation_names.size());
                    std::vector<std::string>::iterator aiter = animation_names.begin();
                    while (aiter != animation_names.end()) {
                        // Animation
                        std::string animation_name = (*aiter);
                        EG::Dynamics::Animation *animation = animations->Get(animation_name);

                        // Animation Name
                        WriteUInt(animation_name.size());
                        WriteString(animation_name);
                        WriteFloat(animation->GetDuration());

                        WriteUInt(animation->GetBoneCount());
                        std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > *positions = animation->GetPositions();
                        std::map<unsigned int, std::vector<std::pair<float, glm::vec3> > > *scalings = animation->GetScalings();
                        std::map<unsigned int, std::vector<std::pair<float, glm::quat> > > *rotations = animation->GetRotations();
                        for (unsigned int i = 0; i < animation->GetBoneCount(); i++) {
                            WriteUInt(i); // Bone ID
                            std::vector<std::pair<float, glm::vec3> > p = (*positions)[i];
                            std::vector<std::pair<float, glm::vec3> > s = (*scalings)[i];
                            std::vector<std::pair<float, glm::quat> > r = (*rotations)[i];

                            // Write Positions
                            WriteUInt(p.size());
                            for (unsigned int j = 0; j < p.size(); j++) {
                                WriteFloat(p[j].first);
                                WriteVec3(p[j].second);
                            }
                            // Write Scalings
                            WriteUInt(s.size());
                            for (unsigned int j = 0; j < s.size(); j++) {
                                WriteFloat(s[j].first);
                                WriteVec3(s[j].second);
                            }
                            // Write Rotations
                            WriteUInt(r.size());
                            for (unsigned int j = 0; j < r.size(); j++) {
                                WriteFloat(r[j].first);
                                WriteQuat(r[j].second);
                            }
                        }

                        ++aiter;
                    }
                } else {
                    WriteBool(false);
                }

                // Lights
                // Particle Systems?

                ++mesh_attribute_iterator;
            }

            out.close();
        }

        void ObjectWriter::WriteString(std::string value) {
            out.write(value.c_str(), sizeof(char) * value.size());
        }

        void ObjectWriter::WriteBool(bool value) {
            out.write(reinterpret_cast<const char *>(&value), sizeof(bool));
        }

        void ObjectWriter::WriteUInt(unsigned int value) {
            out.write(reinterpret_cast<const char *>(&value), sizeof(unsigned int));
        }

        void ObjectWriter::WriteFloat(float value) {
            out.write(reinterpret_cast<const char *>(&value), sizeof(float));
        }

        void ObjectWriter::WriteFloatV(float *value, unsigned int count) {
            out.write(reinterpret_cast<const char *>(value), sizeof(float) * count);
        }

        void ObjectWriter::WriteVec3(glm::vec3 value) {
            out.write(reinterpret_cast<const char *>(glm::value_ptr(value)), sizeof(float) * 3);
        }

        void ObjectWriter::WriteVec4(glm::vec4 value) {
            out.write(reinterpret_cast<const char *>(glm::value_ptr(value)), sizeof(float) * 4);
        }

        void ObjectWriter::WriteQuat(glm::quat value) {
            WriteFloat(value.x);
            WriteFloat(value.y);
            WriteFloat(value.z);
            WriteFloat(value.w);
        }

        void ObjectWriter::WriteMat4(glm::mat4 value) {
            out.write(reinterpret_cast<const char *>(glm::value_ptr(value)), sizeof(float) * 16);
        }

        void ObjectWriter::CopyFile(std::string in, std::string out){
            std::ifstream in_file(in.c_str(), std::fstream::binary);
            std::ofstream out_file(out.c_str(), std::fstream::trunc|std::fstream::binary);
            out_file << in_file.rdbuf();
            in_file.close();
            out_file.close();
        }
    }
}
