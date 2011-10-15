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

				FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
				FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
				FT_Bitmap& bitmap = bitmap_glyph->bitmap;

				int width = EG::Math::Utility::NextPowerOfTwo(bitmap.width);
				int height = EG::Math::Utility::NextPowerOfTwo(bitmap.rows);
				character_dimensions[i] = glm::vec2(float(width), float(height));

				GLubyte* expanded_data = new GLubyte[ 2 * width * height];

				for (int j = 0; j < height; j++){
					for (int i = (width - 1); i >= 0; i--){
						//std::cout << "J: " << j << " I: " << i << std::endl;
						expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
					}
				}

				glGenTextures(1, &(character_texture_ids[i]));
				glBindTexture(GL_TEXTURE_2D, character_texture_ids[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

				GLuint texture_id = character_texture_ids[i];
				//std::cout << "CInit (" << i << "): " << char(i) << " TexID: " << texture_id << std::endl;
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

			AddFont("default", "Assets/Fonts/DejaVuSansMono.ttf", 12);
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

			float x_offset = 0.0f;
			for (unsigned int i = 0; i < text.size(); i++){
				unsigned int character_index = int(char(text[i]));
				//std::cout << "Character (" << character_index << "): " << char(character_index) << std::endl;
				glm::vec2 scale = font->GetCharacterDimensions(character_index);
				glm::mat4 transformation_matrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position.x + x_offset, position.y, position.z)), glm::vec3(scale.x, scale.y, 1.0f));
				GLuint texture_id = font->GetCharacterTextureId(character_index);
				//std::cout << "Texture ID: " << texture_id << std::endl;
				graphics->BindTexture(texture_id, 0);
				shaders->SetMatrix4("model_matrix", transformation_matrix);
				rect->Draw();
				x_offset += scale.x;
			}

			graphics->BindTexture(0, 0);
			return true;
		}
	}
}
