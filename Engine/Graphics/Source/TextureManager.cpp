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
            if (!(textures.count(id))){
                textures[id] = texture;
                return true;
            }
            return false;
        }

        bool TextureManager::AddCubeMap(std::string id, EG::Graphics::CubeMap *texture){
            if (!(cube_maps.count(id))){
                cube_maps[id] = texture;
                return true;
            }
            return false;
        }

        bool TextureManager::HasTexture(std::string id){
            return textures.count(id) > 0;
        }

        bool TextureManager::HasCubeMap(std::string id){
            return cube_maps.count(id) > 0;
        }

        EG::Graphics::Texture *TextureManager::GetTexture(std::string id){
            if (textures.count(id) > 0) {
                return textures[id];
            }
            return nullptr;
        }

        EG::Graphics::CubeMap *TextureManager::GetCubeMap(std::string id){
            if (cube_maps.count(id) > 0) {
                return cube_maps[id];
            }
            return nullptr;
        }
    }
}
