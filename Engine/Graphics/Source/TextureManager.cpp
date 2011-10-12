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
			if (!(textures.Has(id))){
				//textures[id] = texture;
				textures.Set(id, texture);
				return true;
			}
			return false;
		}

		bool TextureManager::AddCubeMap(std::string id, EG::Graphics::CubeMap *texture){
			if (!(cube_maps.Has(id))){
				//cube_maps[id] = texture;
				cube_maps.Set(id, texture);
				return true;
			}
			return false;
		}

		bool TextureManager::HasTexture(std::string id){
			return textures.Has(id);
		}

		bool TextureManager::HasCubeMap(std::string id){
			return cube_maps.Has(id);
		}

		EG::Graphics::Texture *TextureManager::GetTexture(std::string id){
			EG::Graphics::Texture *out = textures.Get(id);
			if (out != NULL){
				return out;
			}
			return NULL;
		}

		EG::Graphics::CubeMap *TextureManager::GetCubeMap(std::string id){
			EG::Graphics::CubeMap *out = cube_maps.Get(id);
			if (out != NULL){
				return out;
			}
			return NULL;
		}
	}
}
