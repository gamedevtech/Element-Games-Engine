#include "../Font.h"

#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

namespace EG{
	namespace Utility{
		Font::Font(std::string _font_name, std::string _font_path, unsigned int _font_size, FT_Library *_library){
			font_name = _font_name;
			font_path = _font_path;
			font_size = _font_size;
			library = _library;
			character_texture_ids = new unsigned int[128];
			character_dimensions = new glm::vec2[128];
			expanded_dimensions = new glm::vec2[128];
			character_offsets = new glm::vec2[128];
			FT_New_Face(*library, font_path.c_str(), 0, &face);
		}

		Font::~Font(void){
			//
		}

		void Font::Load(void){
			FT_Set_Char_Size(face, font_size * 64, font_size * 64, 96, 96);
			for (int i = 0; i < 128; i++){
				if (FT_Load_Glyph(face, FT_Get_Char_Index(face, char(i)), FT_LOAD_DEFAULT)){
					std::cout << "Error Loading Glyph" << std::endl;
				}

				FT_Glyph glyph;
				if(FT_Get_Glyph(face->glyph, &glyph)){
					std::cout << "Error Getting Glypp" << std::endl;
				}

				// Offsets
				FT_BBox bbox;
				FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_SUBPIXELS, &bbox);
				character_offsets[i] = glm::vec2(bbox.xMin / 64.0f, bbox.yMin / 64.0f);

				// Character Dimensions
				character_dimensions[i] = glm::vec2(float(bbox.xMax - bbox.xMin) / 64.0f, float(bbox.yMax - bbox.yMin) / 64.0f);

				FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
				FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
				FT_Bitmap& bitmap = bitmap_glyph->bitmap;

				// Next power of 2 data.
				int width = EG::Math::Utility::NextPowerOfTwo(bitmap.width);
				int height = EG::Math::Utility::NextPowerOfTwo(bitmap.rows);
				expanded_dimensions[i] = glm::vec2(float(width), float(height));
				GLubyte* expanded_data = new GLubyte[ 2 * width * height];

				for (int j = 0; j < height; j++){
					for (int i = (width - 1); i >= 0; i--){
						expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
					}
				}

				glGenTextures(1, &(character_texture_ids[i]));
				glBindTexture(GL_TEXTURE_2D, character_texture_ids[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

				delete []expanded_data;
			}
			FT_Done_Face(face);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		unsigned int Font::GetCharacterTextureId(unsigned int char_index){
			return character_texture_ids[char_index];
		}

		glm::vec2 Font::GetCharacterDimensions(unsigned int char_index){
			return character_dimensions[char_index];
		}

		glm::vec2 Font::GetExpandedDimensions(unsigned int char_index){
			return expanded_dimensions[char_index];
		}

		glm::vec2 Font::GetCharacterOffsets(unsigned int char_index){
			return character_offsets[char_index];
		}

		unsigned int Font::GetFontSize(void){
			return font_size;
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
			FT_Init_FreeType(&library);
			EG::Graphics::Triangle *rect_triangles = new EG::Graphics::Triangle[2];
			rect_triangles[0].vertices[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[0].vertices[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[0].vertices[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].vertices[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[1].vertices[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].vertices[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[0].texcoords[0] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[0].texcoords[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[0].texcoords[2] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[1].texcoords[0] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			rect_triangles[1].texcoords[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			rect_triangles[1].texcoords[2] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			EG::Graphics::TriangleMesh *rect_triangle_mesh = new EG::Graphics::TriangleMesh(2, rect_triangles, true, true, true, true, true, true);
			rect = new EG::Graphics::Mesh(rect_triangle_mesh);

			AddFont("default", "Assets/Fonts/DejaVuSansMono.ttf", 24);
		}

		FontManager::~FontManager(void){
			FT_Done_FreeType(library);
		}

		bool FontManager::AddFont(std::string font_name, std::string font_path, unsigned int font_size){
			Font *new_font = new Font(font_name, font_path, font_size, &library);
			new_font->Load();
			fonts.Set(font_name, new_font);

			return true;
		}

		bool FontManager::DrawText(std::string text, glm::vec3 position, glm::vec2 scale, EG::Graphics::ShaderManager *shaders, std::string font_name){
			Font *font = fonts.Get(font_name);

			float string_x_offset = 0.0f;
			for (unsigned int i = 0; i < text.size(); i++){
				unsigned int character_index = int(char(text[i]));
				float size = float(font->GetFontSize());

				glm::vec2 cdims = font->GetCharacterDimensions(character_index);
				glm::vec2 offset = font->GetCharacterOffsets(character_index);
				glm::vec2 bdims = font->GetExpandedDimensions(character_index);
				if (char(character_index) != '.'){
					offset.y += cdims.y - (size - 1.0f);
				}else{
					offset.x -= (size / 8.0f);
					offset.y += (size / 8.0f);
				}

				//std::cout << char(character_index) << ' ' << offset.x << ' ' << offset.y << std::endl;

				glm::vec3 character_position;
				character_position.x = position.x + string_x_offset + offset.x + (size / 4.0f);
				character_position.y = position.y + offset.y;
				character_position.z = position.z;

				glm::vec3 character_scale;
				character_scale.x = scale.x * bdims.x;
				character_scale.y = scale.y * bdims.y;
				character_scale.z = 1.0f;

				glm::mat4 transformation_matrix = glm::scale(glm::translate(glm::mat4(1.0f), character_position), character_scale);

				graphics->BindTexture(font->GetCharacterTextureId(character_index), 0);
				shaders->SetMatrix4("model_matrix", transformation_matrix);

				rect->Draw();
				string_x_offset += cdims.x + offset.x + (size / 4.0f);
			}

			graphics->BindTexture(0, 0);
			return true;
		}
	}
}
