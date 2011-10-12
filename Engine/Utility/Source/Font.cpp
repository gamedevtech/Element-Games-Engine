#include "../Font.h"

namespace EG{
	namespace Utility{
		// Singleton Stuff
		FontManager *FontManager::instance = NULL;
		FontManager *FontManager::Instance(void){
			if (instance == NULL){
				instance = new FontManager();
			}
			return instance;
		}

		FontManager::FontManager(void){
			FTFont *default_font = new FTGLTextureFont("Assets/Fonts/DejaVuSansMono.ttf");
			default_font->FaceSize(12);
			fonts.Set("default", default_font);
		}

		FontManager::~FontManager(void){
			//
		}

		bool FontManager::AddFont(std::string font_name, std::string font_path, unsigned int font_size){
			FTFont *new_font = new FTGLTextureFont(font_path.c_str());
			new_font->FaceSize(font_size);
			//if (new_font->Open(font_path)){
				fonts.Set(font_name, new_font);
				return true;
			//}
			//return false;
		}

		FTFont *FontManager::GetFont(std::string font_name){
			if (fonts.Has(font_name)){
				return fonts.Get(font_name);
			}
			return NULL;
		}

		void FontManager::DrawText(std::string text, std::string font_name){
			if (fonts.Has(font_name)){
				fonts.Get(font_name)->Render(text.c_str());
			}
		}
	}
}
