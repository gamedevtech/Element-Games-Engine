#include "../RocketInterface.h"

#include <iostream>

namespace EG{
	namespace Utility{
		RocketInterface::RocketInterface(void){
// 			ShellRenderInterfaceOpenGL opengl_renderer;
// 			Rocket::Core::SetRenderInterface(&opengl_renderer);
//
// 			ShellSystemInterface system_interface;
// 			Rocket::Core::SetSystemInterface(&system_interface);

			//Rocket::Core::Initialise();

/*			Rocket::Core::Context *context = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(800, 500));
			if (context != NULL){
				std::cout << "Context Created" << std::endl;
				context->GetName();
				std::cout << "Dongle" << std::endl;
				std::cout << context->GetName().CString() << std::endl;
				Rocket::Core::Element *document = context->LoadDocument("Assets/GUIs/sandbox.rml");
				std::cout << "RML Loaded" << std::endl;
			}else{
				std::cout << "Context Could Not Be Created!" << std::endl;
			}*/
		}

		RocketInterface::~RocketInterface(void){
			//
		}
	}
}
