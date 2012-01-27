#ifndef EG_GAME_SCENE_H
#define EG_GAME_SCENE_H

#include "ObjectManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/TextureManager.h"
#include "../Graphics/Camera.h"

namespace EG{
	namespace Game{
		class Scene{
			public:
				Scene(void);
				~Scene(void);

				EG::Game::ObjectManager *GetObjectManager(void);

				EG::Graphics::MeshManager *GetMeshManager(void);
				EG::Graphics::TextureManager *GetTextureManager(void);

				void SetCurrentCamera(EG::Graphics::Camera *camera);
				EG::Graphics::Camera *GetCurrentCamera(void);
			private:
				EG::Game::ObjectManager *objects;
				EG::Graphics::MeshManager *meshes;
				EG::Graphics::TextureManager *textures;
				EG::Graphics::Camera *current_camera;
		};
	}
}

#endif
