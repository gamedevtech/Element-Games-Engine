#include "../RocketInterface.h"

#include <iostream>
#include "../../Graphics/GraphicsSubsystem.h"

namespace EG{
	namespace Utility{
		// System Interface Subclass
		RocketSystemInterface::RocketSystemInterface(void){
			//
		}
		RocketSystemInterface::~RocketSystemInterface(void){
			//
		}
		void RocketSystemInterface::SetTimer(EG::Utility::Time *_time){
			time = _time;
		}
		float RocketSystemInterface::GetElapsedTime(void){
			return time->GetElapsedTime();
		}

		// Render Interface Subclass
		RocketRenderInterface::RocketRenderInterface(EG::Graphics::ShaderManager *_shaders){
			shaders = _shaders;
		}
		RocketRenderInterface::~RocketRenderInterface(void){
			//
		}

		void RocketRenderInterface::SetVertex(Rocket::Core::Vertex *vertex){
			SetColor(vertex->colour);
			glTexCoord2f(vertex->tex_coord.x, vertex->tex_coord.y);
			glVertex4f(vertex->position.x, vertex->position.y, 0.0f, 1.0f);
		}

		void RocketRenderInterface::SetColor(Rocket::Core::Colourb color){
			//std::cout << int(color.red) << ' ' << int(color.green) << ' ' << int(color.blue) << ' ' << int(color.alpha) << std::endl;
			glColor4f(int(color.red) / 256.0f, int(color.green) / 256.0f, int(color.blue) / 256.0f, int(color.alpha) / 256.0f);
		}

		void RocketRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation){
			//shaders->SetMatrix4("model_matrix", glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f)), glm::vec3(translation.x, translation.y, 0.0f)));
			shaders->SetMatrix4("model_matrix", glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, 0.0f)));

			if (texture){
				glEnable(GL_TEXTURE_2D);
				shaders->SetInt("use_decal", 1);
				glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
			}else{
				shaders->SetInt("use_decal", 0);
			}

			glBegin(GL_TRIANGLES);
			for (int i = 0; i < num_indices; i++){
				SetVertex(&(vertices[indices[i]]));
			}
			glEnd();
		}
		void RocketRenderInterface::EnableScissorRegion(bool enabled){
			if (enabled){
				glEnable(GL_SCISSOR_TEST);
			}else{
				glDisable(GL_SCISSOR_TEST);
			}
		}
		void RocketRenderInterface::SetScissorRegion(int x, int y, int w, int h){
			glScissor(x, 500 - (y + h), w, h);
		}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

		// Called by Rocket when a texture is required by the library.
		bool RocketRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
		{
			Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
			Rocket::Core::FileHandle file_handle = file_interface->Open(source);
			if (!file_handle)
			{
				return false;
			}

			file_interface->Seek(file_handle, 0, SEEK_END);
			size_t buffer_size = file_interface->Tell(file_handle);
			file_interface->Seek(file_handle, 0, SEEK_SET);

			char* buffer = new char[buffer_size];
			file_interface->Read(buffer, buffer_size, file_handle);
			file_interface->Close(file_handle);

			TGAHeader header;
			memcpy(&header, buffer, sizeof(TGAHeader));

			int color_mode = header.bitsPerPixel / 8;
			int image_size = header.width * header.height * 4; // We always make 32bit textures

			if (header.dataType != 2)
			{
				Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
				return false;
			}

			// Ensure we have at least 3 colors
			if (color_mode < 3)
			{
				Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
				return false;
			}

			const char* image_src = buffer + sizeof(TGAHeader);
			unsigned char* image_dest = new unsigned char[image_size];

			// Targa is BGR, swap to RGB and flip Y axis
			for (long y = 0; y < header.height; y++)
			{
				long read_index = y * header.width * color_mode;
				long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
				for (long x = 0; x < header.width; x++)
				{
					image_dest[write_index] = image_src[read_index+2];
					image_dest[write_index+1] = image_src[read_index+1];
					image_dest[write_index+2] = image_src[read_index];
					if (color_mode == 4)
						image_dest[write_index+3] = image_src[read_index+3];
					else
						image_dest[write_index+3] = 255;

					write_index += 4;
					read_index += color_mode;
				}
			}

			texture_dimensions.x = header.width;
			texture_dimensions.y = header.height;

			bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);

			delete [] image_dest;
			delete [] buffer;

			return success;
		}

		// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
		bool RocketRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
		{
			GLuint texture_id = 0;
			glGenTextures(1, &texture_id);
			if (texture_id == 0)
			{
				printf("Failed to generate textures\n");
				return false;
			}

			glBindTexture(GL_TEXTURE_2D, texture_id);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			texture_handle = (Rocket::Core::TextureHandle) texture_id;

			return true;
		}

		// Called by Rocket when a loaded texture is no longer required.
		void RocketRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
		{
			glDeleteTextures(1, (GLuint*) &texture_handle);
		}

		// GUI Storage/Interface Class
		RocketInterface::RocketInterface(EG::Utility::Time *time, EG::Graphics::ShaderManager *shaders){
			std::cout << "Setting Up libRocket." << std::endl;
			render_interface = new RocketRenderInterface(shaders);
			Rocket::Core::SetRenderInterface(render_interface);
			std::cout << "Rocket Renderer Set" << std::endl;
			system_interface = new RocketSystemInterface();
			system_interface->SetTimer(time);
			Rocket::Core::SetSystemInterface(system_interface);
			std::cout << "Rocket System Interface Set" << std::endl;
			Rocket::Core::Initialise();
			Rocket::Controls::Initialise();
			std::cout << "Rocket Initialized." << std::endl;
			context = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(800, 500));
			std::cout << "Rocket Context Created!" << std::endl;
			Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("Assets/GUIs/Delicious-Roman.otf"));
			Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("Assets/GUIs/Delicious-Italic.otf"));
			Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("Assets/GUIs/Delicious-Bold.otf"));
			Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("Assets/GUIs/Delicious-BoldItalic.otf"));
			Rocket::Core::ElementDocument* document = context->LoadDocument("Assets/GUIs/test.rml");
			if (document != NULL){
				document->Show();
			}
			std::cout << "Rocket Document Loaded" << std::endl;
		}

		RocketInterface::~RocketInterface(void){
			//
		}
		void RocketInterface::Update(void){
			context->Update();
		}
		void RocketInterface::Draw(void){
			context->Render();
		}
	}
}
