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
		RocketRenderInterface::RocketRenderInterface(void){
			//
		}
		RocketRenderInterface::~RocketRenderInterface(void){
			//
		}
		void RocketRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation){
			// Need to write simple immediate mode drawing code here!
			glPushMatrix();
			glTranslatef(translation.x, translation.y, 0);

			glVertexPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex), &vertices[0].position);
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rocket::Core::Vertex), &vertices[0].colour);

			if (!texture){
				glDisable(GL_TEXTURE_2D);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}else{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex), &vertices[0].tex_coord);
			}

			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

			glPopMatrix();
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

		// GUI Storage/Interface Class
		RocketInterface::RocketInterface(EG::Utility::Time *time){
			std::cout << "Setting Up libRocket." << std::endl;
			render_interface = new RocketRenderInterface();
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
			Rocket::Core::ElementDocument* document = context->LoadDocument("Assets/GUIs/demo.rml");
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
