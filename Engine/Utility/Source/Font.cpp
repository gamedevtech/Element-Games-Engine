#include "../Font.h"
//#include "../Headers/MathUtility.h"

namespace EG{
	namespace Utility{
/*		Font::Font(std::string font_path, float height, FT_Library *library){
			// Set Variables
			font_height = height;
			ft_library = library;

			// Setup FT Font
			FT_New_Face(*library, font_path.c_str(), 0, &face);
			unsigned int font_height_int = (unsigned int)(font_height);
			FT_Set_Char_Size(face, font_height_int << 6, font_height_int << 6, 96, 96);

			// Setup GL
			list_base = glGenLists(128);
			textures = new unsigned int[128];
			glGenTextures(128, textures);

			for(unsigned char i = 0; i < 128; i++){
				CreateCharacterDisplayList(i);
			}

			FT_Done_Face(face);
		}

		Font::~Font(void){
			glDeleteLists(list_base, 128);
			glDeleteTextures(128, textures);
			delete [] textures;
		}

		void Font::CreateCharacterDisplayList(char ch){
			FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);

			//Move the face's glyph into a Glyph object.
			FT_Glyph glyph;
			FT_Get_Glyph(face->glyph, &glyph);

			//Convert the glyph to a bitmap.
			FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
			FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
			FT_Bitmap &bitmap = bitmap_glyph->bitmap;

			int width = EG::Math::Utility::NextPowerOfTwo(bitmap.width);
			int height = EG::Math::Utility::NextPowerOfTwo(bitmap.rows);

			//Allocate memory for the texture data.
			GLubyte* expanded_data = new GLubyte[ 2 * width * height];

			for(int j=0; j <height;j++) {
				for(int i=0; i < width; i++){
					expanded_data[2 * (i + j * width)]= expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
				}
			}

			//Now we just setup some texture paramaters.
			glBindTexture(GL_TEXTURE_2D, textures[ch]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
			delete [] expanded_data;

			//So now we can create the display list
			glNewList(list_base+ch,GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, textures[ch]);

			glPushMatrix();

			// Moved commendted translate to here
			//glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(bitmap_glyph->left, 0, 0);
			//glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);
			glTranslatef(0, -bitmap_glyph->top, 0);

			float x = (float)bitmap.width / (float)width;
			float y = (float)bitmap.rows / (float)height;

			glBegin(GL_QUADS);
			glTexCoord2d(0, y);
			glVertex2f(0, bitmap.rows);
			glTexCoord2d(0, 0);
			glVertex2f(0, 0);
			glTexCoord2d(x, 0);
			glVertex2f(bitmap.width, 0);
			glTexCoord2d(x, y);
			glVertex2f(bitmap.width, bitmap.rows);
			glEnd();
			glPopMatrix();
			glTranslatef(face->glyph->advance.x >> 6, 0, 0);
			glEndList();
		}

		FontManager *FontManager::self_instance = 0;
		FontManager *FontManager::SelfInstance(void){
			if (!self_instance){
				self_instance = new FontManager();
			}
			return self_instance;
		}

		FontManager::FontManager(void){
			FT_Init_FreeType(&library);
			//AddFont("default", "Common/Fonts/Vera.ttf", 10.0f);
			AddFont("default", "Common/Fonts/DejaVuSansMono.ttf", 10.0f);
		}
		FontManager::~FontManager(void){
			FT_Done_FreeType(library);
		}

		bool FontManager::AddFont(std::string font_name, std::string font_path, float height){
			if (fonts.count(font_name) > 0){
				return false;
			}
			Font *font = new Font(font_path, height, &library);
			fonts[font_name] = font;
			return true;
		}

		unsigned int FontManager::RemoveFont(std::string font_name){
			if (fonts.count(font_name) > 0){
				fonts.erase(font_name);
				return 1;
			}
			return 0;
		}

		Font *FontManager::GetFont(std::string font_name){
			if (fonts.count(font_name) > 0){
				return fonts[font_name];
			}
			return fonts["default"];
		}*/
	}
}
