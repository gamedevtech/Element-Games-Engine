#ifndef EG_MEDIA_ASSIMP_INTERFACE_H
#define EG_MEDIA_ASSIMP_INTERFACE_H

#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// #include <assimp/assimp.hpp>
// #include <assimp/aiScene.h>
// #include <assimp/aiPostProcess.h>

#include "../Game/Scene.h"
#include "../Graphics/RenderingMaterial.h"
#include "../Graphics/Mesh.h"
#include "../Dynamics/Animation.h"

namespace EG{
    namespace Media{
        class AssimpInterface{
            public:
                AssimpInterface(EG::Game::Scene *_scene);
                ~AssimpInterface(void);

                bool Load(std::string _file_path);

                unsigned int GetMeshMaterialPairCount(void){
                    return count;
                }
                std::string GetMesh(unsigned int index){
                    return meshes[index];
                }
                EG::Graphics::RenderingMaterial *GetMaterial(unsigned int index){
                    return materials[index];
                }
                EG::Dynamics::Animations *GetAnimations(void){
                    return animations;
                }
                bool HasAnimations(void){
                    return has_animations;
                }
            private:
                std::string file_path;
                EG::Game::Scene *scene;
                const aiScene *ai_scene;
                aiNode *root_node;

                // Utility Functions
                glm::mat4 ConvertMatrix(aiMatrix4x4 in);
                aiNode *FindNode(std::string find_name, glm::mat4 &t);

                // Skeletal Loaders
                void LoadBones(const aiScene *ai_scene);
                aiNode *FindRootBone(const aiScene *ai_scene);
                void LoadSkeleton(const aiScene *ai_scene);
                void BuildSkeleton(aiNode *current, EG::Dynamics::Bone *parent);

                // Mesh and Material Loaders
                void LoadMesh(const aiMesh *ai_mesh, unsigned int index);
                void LoadMaterial(const aiMaterial *ai_material, unsigned int index);

                // Animation Loaders
                EG::Dynamics::Animation *LoadAnimation(const aiAnimation *ai_animation,
                                                       unsigned int index);

                unsigned int count;
                std::unordered_map<unsigned int, std::string> meshes;
                std::unordered_map<unsigned int, EG::Graphics::RenderingMaterial *> materials;
                std::map<unsigned int, std::map<unsigned int, glm::mat4> > bone_transformations;
                EG::Dynamics::Animations *animations;
                bool has_animations;

                // Bone Name and Count Info
                unsigned int bone_count;
                std::vector<std::string> bone_names;
                std::map<std::string, unsigned int> bone_name_map;
                std::map<unsigned int, glm::mat4> bind_pose_map;
                EG::Dynamics::Skeleton *bind_pose_skeleton;
        };
    }
}

#endif
