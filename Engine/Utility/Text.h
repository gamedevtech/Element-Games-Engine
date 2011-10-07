#ifndef TEXT_H
#define TEXT_H

#include "Font.h"
#include "../Math/Math.h"

#define text_manager EG::Utility::TextManager::SelfInstance()

namespace EG{
	namespace Utility{
		/*float PrintText(std::string text, glm::vec3 position, std::string font_name = "default");

		class Text{
		public:
			Text(std::string _text_string, std::string _font_name, float position_x, float position_y);
			~Text(void);

			void Draw(void);

			void SetTextString(std::string _text_string){
				text_string = _text_string;
			}

			void SetPosition(float position_x, float position_y){
				position = glm::vec3(position_x, position_y, 0.0f);
			}

		private:
			std::string text_string;
			std::string font_name;
			float scale;
			glm::vec4 color;
			glm::vec3 position;
		};

		class TextManager{
		public:
			static TextManager *SelfInstance();

			TextManager(void);
			~TextManager(void);

			bool AddText(std::string text_name, std::string text_value, std::string text_font, float position_x, float position_y);
			bool SetTextValue(std::string text_name, std::string text_value);
			bool SetTextPosition(std::string text_name, float position_x, float position_y);
			bool HasText(std::string text_name);

			void Draw(void);
		private:
			std::map<std::string, Text *> text_outputs;

			// Singleton self instance
			static TextManager *self_instance;
		};*/
	}
}

#endif
