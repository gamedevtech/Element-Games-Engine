#ifndef FONT_H
#define FONT_H

#include "../Graphics/OpenGLInterface.h"
#include "../Graphics/Texture.h"
#include "../Math/Math.h"
#include <map>
#include <string>

/*#ifdef WIN32
#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#else
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#endif

#define FONT_TEXCOORD_STEP 0.0625f

#define font_manager EG::Utility::FontManager::SelfInstance()

namespace EG{
	namespace Utility{
		class Font{
		public:
			Font(std::string file_name, float height, FT_Library *library);
			~Font(void);

			unsigned int GetDisplayListBase(void){
				return list_base;
			}

			unsigned int *GetTextures(void){
				return textures;
			}

			float GetHeight(void){
				return font_height;
			}

		private:
			float font_height;

			FT_Library *ft_library;
			FT_Face face;

			GLuint *textures;
			GLuint list_base;

			void CreateCharacterDisplayList(char ch);
		};

		class FontManager{
		public:
			static FontManager *SelfInstance();

			FontManager(void);
			~FontManager(void);

			bool AddFont(std::string font_name, std::string font_path, float height);
			unsigned int RemoveFont(std::string font_name);
			Font *GetFont(std::string font_name);
		private:
			FT_Library library;
			std::map<std::string, Font *> fonts;

			// Singleton self instance
			static FontManager *self_instance;
		};
	}
}*/

#endif
