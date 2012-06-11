#ifndef EG_MEDIA_ASSIMP_INTERFACE_H
#define EG_MEDIA_ASSIMP_INTERFACE_H

#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include "../Utility/Dictionary.h"
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
                    return meshes.Get(index);
                }
                EG::Graphics::RenderingMaterial *GetMaterial(unsigned int index){
                    return materials.Get(index);
                }
            private:
                std::string file_path;
                EG::Game::Scene *scene;
                const aiScene *ai_scene;

                void LoadSkeleton(const aiScene *ai_scene);
                void RecursiveLoadSkeleton(const aiNode *parent_node, EG::Dynamics::Bone *parent_bone);
                void LoadMesh(const aiMesh *ai_mesh, unsigned int index);
                void LoadMaterial(const aiMaterial *ai_material, unsigned int index);
                void LoadAnimation(const aiAnimation *ai_animation, unsigned int index);

                unsigned int count;
                EG::Utility::Dictionary<unsigned int, std::string> meshes;
                EG::Utility::Dictionary<unsigned int, EG::Graphics::RenderingMaterial *> materials;
                std::map<unsigned int, std::map<unsigned int, glm::mat4> > bone_transformations;

                // Bone Name and Count Info
                unsigned int bone_count;
                std::vector<std::string> bone_names;
                std::map<std::string, unsigned int> bone_name_map;
                EG::Dynamics::Skeleton *bind_pose_skeleton;
        };
    }
}

#endif
