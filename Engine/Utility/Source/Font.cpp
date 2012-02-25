#include "../Font.h"

namespace EG{
    namespace Utility{
        Font::Font(std::string font_path){
            font = new FTTextureFont(font_path.c_str());
        }

        Font::~Font(void){
            delete font;
        }

        void Font::Draw(std::string text, unsigned int size){
            font->FaceSize(size);
            font->Render(text.c_str());
        }

        // Singleton Stuff
        FontManager *FontManager::instance = NULL;
        FontManager *FontManager::Instance(void){
            if (instance == NULL){
                instance = new FontManager();
            }
            return instance;
        }

        FontManager::FontManager(void){
            AddFont("default", "Assets/Fonts/DejaVuSansMono.ttf");
        }

        FontManager::~FontManager(void){
            //
        }

        bool FontManager::AddFont(std::string font_name, std::string font_path){
            if (!(fonts.Has(font_name))){
                Font *new_font = new Font(font_path);
                fonts.Set(font_name, new_font);
                return true;
            }
            return false;
        }

        bool FontManager::Draw(std::string text, unsigned int size, std::string font_name){
            if (fonts.Has(font_name)){
                Font *font = fonts.Get(font_name);
                font->Draw(text, size);
                return true;
            }
            return false;
        }
    }
}
