#ifndef FONT_H
#define FONT_H

#include "../Graphics/OpenGLInterface.h"
#include "../Graphics/Texture.h"
#include "../Math/Math.h"
#include "../Utility/Dictionary.h"
#include <string>

#include <FTGL/ftgl.h>

#define font_manager EG::Utility::FontManager::Instance()

namespace EG{
	namespace Utility{
		class FontManager{
			public:
				FontManager(void);
				~FontManager(void);

				bool AddFont(std::string font_name, std::string font_path, unsigned int font_size = 12);
				FTFont *GetFont(std::string font_name);
				void DrawText(std::string text, std::string font_name = "default");

				static FontManager *Instance(void);
			private:
				EG::Utility::Dictionary<std::string, FTFont *> fonts;

				static FontManager* instance;
		};
	}
}

#endif
