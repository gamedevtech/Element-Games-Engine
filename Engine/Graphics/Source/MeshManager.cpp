#include "../MeshManager.h"

#include <iostream>

namespace EG{
    namespace Graphics{
        MeshManager::MeshManager(void){
            //
        }

        MeshManager::~MeshManager(void){
            // NOTE: Delete meshes here?
            meshes.clear();
        }

        bool MeshManager::Add(std::string mesh_id, EG::Graphics::Mesh *mesh){
            if (meshes.count(mesh_id) < 1) {
                meshes[mesh_id] = mesh;
                return true;
            }
            return false;
        }

        EG::Graphics::Mesh *MeshManager::Get(std::string mesh_id){
            if (meshes.count(mesh_id) > 0) {
                return meshes[mesh_id];
            }
            return nullptr;
        }
    }
}
