#ifndef EG_GRAPHICS_TEXTURE_MANAGER_H
#define EG_GRAPHICS_TEXTURE_MANAGER_H

//#include <map>
#include "../Utility/StringDictionary.h"
#include <string>

#include "Texture.h"

namespace EG{
	namespace Graphics{
		class TextureManager{
			public:
				TextureManager(void);
				~TextureManager(void);

				bool AddTexture(std::string id, EG::Graphics::Texture *texture);
				bool AddCubeMap(std::string id, EG::Graphics::CubeMap *texture);

				bool HasTexture(std::string id);
				bool HasCubeMap(std::string id);

				EG::Graphics::Texture *GetTexture(std::string id);
				EG::Graphics::CubeMap *GetCubeMap(std::string id);
			private:
				EG::Utility::StringDictionary<EG::Graphics::Texture *> textures;
				EG::Utility::StringDictionary<EG::Graphics::CubeMap *> cube_maps;
		};
	}
}

#endif
