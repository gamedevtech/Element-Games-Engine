#include "../TextureManager.h"

namespace EG{
	namespace Graphics{
		TextureManager::TextureManager(void){
			//
		}

		TextureManager::~TextureManager(void){
			//
		}

		bool TextureManager::AddTexture(std::string id, EG::Graphics::Texture *texture){
			if (textures.count(id) < 1){
				textures[id] = texture;
				return true;
			}
			return false;
		}

		bool TextureManager::AddCubeMap(std::string id, EG::Graphics::CubeMap *texture){
			if (cube_maps.count(id) < 1){
				cube_maps[id] = texture;
				return true;
			}
			return false;
		}

		bool TextureManager::HasTexture(std::string id){
			if (textures.count(id) > 0){
				return true;
			}
			return false;
		}

		bool TextureManager::HasCubeMap(std::string id){
			if (cube_maps.count(id) > 0){
				return true;
			}
			return false;
		}

		EG::Graphics::Texture *TextureManager::GetTexture(std::string id){
			if (textures.count(id) > 0){
				return textures[id];
			}
			return NULL;
		}

		EG::Graphics::CubeMap *TextureManager::GetCubeMap(std::string id){
			if (cube_maps.count(id) > 0){
				return cube_maps[id];
			}
			return NULL;
		}
	}
}
