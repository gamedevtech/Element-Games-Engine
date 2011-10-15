#ifndef FONT_H
#define FONT_H

#include "../Graphics/GraphicsSubsystem.h"
#include "../Graphics/OpenGLInterface.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Mesh.h"
#include "../Math/Math.h"
#include "../Utility/Dictionary.h"
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#define font_manager EG::Utility::FontManager::Instance()

namespace EG{
	namespace Utility{
		class Font{
			public:
				Font(std::string _font_name, std::string _font_path, unsigned int _font_size, FT_Library *_library);
				~Font(void);

				void Load(void);
				unsigned int GetCharacterTextureId(unsigned int char_index);
				glm::vec2 GetCharacterDimensions(unsigned int char_index);
				glm::vec2 GetExpandedDimensions(unsigned int char_index);
				glm::vec2 GetCharacterOffsets(unsigned int char_index);
				unsigned int GetFontSize(void);
			private:
				std::string font_name;
				std::string font_path;
				unsigned int font_size;
				FT_Library *library;
				FT_Face face;
				unsigned int *character_texture_ids;
				glm::vec2 *character_dimensions;
				glm::vec2 *expanded_dimensions;
				glm::vec2 *character_offsets;
		};

		class FontManager{
			public:
				FontManager(void);
				~FontManager(void);

				bool AddFont(std::string font_name, std::string font_path, unsigned int font_size = 12);
				bool DrawText(std::string text, glm::vec3 position, glm::vec2 scale, EG::Graphics::ShaderManager *shaders, std::string font_name = "default");

				static FontManager *Instance(void);
			private:
				FT_Library library;
				static FontManager *instance;
				EG::Graphics::Mesh *rect;
				EG::Utility::Dictionary<std::string, EG::Utility::Font *> fonts;
		};
	}
}

#endif
