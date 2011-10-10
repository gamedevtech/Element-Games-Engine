#include "../Window.h"

namespace EG{
	namespace Utility{
		Window::Window(float _resolution_width, float _resolution_height, unsigned int _bits_per_pixel, bool _fullscreen, std::string _window_title){
			resolution_width = _resolution_width;
			resolution_height = _resolution_height;
			bits_per_pixel = _bits_per_pixel;
			fullscreen = _fullscreen;
			window_title = _window_title;

			sfml_interface->Initialize(resolution_width, resolution_height, bits_per_pixel, fullscreen, window_title);
		}

		Window::~Window(void){
			//
		}

		bool Window::IsOpened(void){
			return sfml_interface->IsWindowOpened();
		}

		void Window::Update(void){
			sfml_interface->UpdateWindow();
			if (sfml_interface->WindowResized()){
				graphics->Resize(sfml_interface->GetResolutionWidth(), sfml_interface->GetResolutionHeight());
			}
		}

		void Window::Display(void){
			sfml_interface->DisplayWindow();
		}

		void Window::Close(void){
			sfml_interface->Close();
		}

		float Window::GetResolutionWidth(void){
			return resolution_width;
		}

		float Window::GetResolutionHeight(void){
			return resolution_height;
		}
	}
}
