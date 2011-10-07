#include "../Text.h"

#include <iostream>

namespace EG{
	namespace Utility{
		/*float PrintText(std::string text, glm::vec3 position, std::string font_name){
			EG::Utility::Font *font = font_manager->GetFont(font_name);
			unsigned int display_list = font->GetDisplayListBase();
			unsigned int *font_textures = font->GetTextures();
			float height = font->GetHeight();
			const char *c_text = text.c_str();
			unsigned int text_length = text.size();

			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glPushMatrix();
			glTranslatef(position.x, position.y, 0.0f);
			glListBase(display_list);
			glCallLists(text_length, GL_UNSIGNED_BYTE, c_text);
			glPopMatrix();
		}

		Text::Text(std::string _text_string, std::string _font_name, float position_x, float position_y){
			text_string = _text_string;
			font_name = _font_name;
			position = glm::vec3(position_x, position_y, 0.0f);
			color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			scale = 1.0f;
		}

		Text::~Text(void){
			//
		}

		void Text::Draw(void){
			std::cout << "Drawing Font" << std::endl;
			EG::Utility::Font *font = font_manager->GetFont(font_name);
			unsigned int display_list = font->GetDisplayListBase();
			const char *c_text = text_string.c_str();
			unsigned int text_length = text_string.size();

			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(color.x, color.y, color.z, color.w);

			glPushMatrix();
			glTranslatef(position.x, position.y, 0.0f);
			glScalef(scale, scale, 1.0f);
			glListBase(display_list);
			glCallLists(text_length, GL_UNSIGNED_BYTE, c_text);
			glPopMatrix();
		}

		TextManager *TextManager::self_instance = 0;

		TextManager *TextManager::SelfInstance(void){
			if (!self_instance){
				self_instance = new TextManager();
			}
			return self_instance;
		}

		TextManager::TextManager(void){
			//
		}

		TextManager::~TextManager(void){
			//
		}

		bool TextManager::AddText(std::string text_name, std::string text_value, std::string text_font, float position_x, float position_y){
			if (text_outputs.count(text_name) > 0){
				return false;
			}
			Text *text = new Text(text_value, text_font, position_x, position_y);
			text_outputs[text_name] = text;
			return true;
		}

		bool TextManager::SetTextValue(std::string text_name, std::string text_value){
			if (text_outputs.count(text_name) > 0){
				text_outputs[text_name]->SetTextString(text_value);
				return true;
			}
			return false;
		}

		bool TextManager::SetTextPosition(std::string text_name, float position_x, float position_y){
			if (text_outputs.count(text_name) > 0){
				text_outputs[text_name]->SetPosition(position_x, position_y);
				return true;
			}
			return false;
		}

		void TextManager::Draw(void){
			std::map<std::string, Text *>::iterator text_outputs_iterator = text_outputs.begin();
			while (text_outputs_iterator != text_outputs.end()){
				(*text_outputs_iterator).second->Draw();
				++text_outputs_iterator;
			}
		}

		bool TextManager::HasText(std::string text_name){
			if (text_outputs.count(text_name) > 0){
				return true;
			}
			return false;
		}*/
	}
}
