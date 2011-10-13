#ifndef EG_MEDIA_ASSIMP_INTERFACE_H
#define EG_MEDIA_ASSIMP_INTERFACE_H

#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

#include "../Utility/Dictionary.h"
#include "../Game/Scene.h"
#include "../Graphics/RenderingMaterial.h"
#include "../Graphics/Mesh.h"

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

				void LoadMesh(const aiMesh *ai_mesh, unsigned int index);
				void LoadMaterial(const aiMaterial *ai_material, unsigned int index);

				unsigned int count;
				EG::Utility::Dictionary<unsigned int, std::string> meshes;
				EG::Utility::Dictionary<unsigned int, EG::Graphics::RenderingMaterial *> materials;
		};
	}
}

#endif
