#include "../MeshManager.h"

#include <iostream>

namespace EG{
	namespace Graphics{
		MeshManager::MeshManager(void){
			//
		}

		MeshManager::~MeshManager(void){
			meshes.Clear();
		}

		bool MeshManager::Add(std::string mesh_id, EG::Graphics::Mesh *mesh){
			EG::Graphics::Mesh *temp = meshes.Get(mesh_id);
			if (temp == NULL){
				meshes.Set(mesh_id, mesh);
				return true;
			}
			return false;
		}

		EG::Graphics::Mesh *MeshManager::Get(std::string mesh_id){
			EG::Graphics::Mesh *temp = meshes.Get(mesh_id);
			if (temp != NULL){
				return meshes.Get(mesh_id);
			}
			return NULL;
		}
	}
}
