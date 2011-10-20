#include "../SFMLInterface.h"

#include <iostream>

namespace EG{
	namespace Media{
		// Singleton Stuff
		SFMLInterface *SFMLInterface::instance = NULL;
		SFMLInterface *SFMLInterface::Instance(void){
			if (instance == NULL){
				instance = new SFMLInterface();
			}
			return instance;
		}

		// Constructors
		SFMLInterface::SFMLInterface(void){
			window_resized = false;
			input = NULL;
			opened = true;
		}

		SFMLInterface::~SFMLInterface(void){
			//
		}

		void SFMLInterface::Initialize(unsigned int _resolution_width, unsigned int _resolution_height, unsigned int _bits_per_pixel, bool _fullscreen, std::string _window_title){
			resolution_width = _resolution_width;
			resolution_height = _resolution_height;
			bits_per_pixel = _bits_per_pixel;
			fullscreen = _fullscreen;
			window_title = _window_title;
			clock = new sf::Clock();
			clock->Reset();

			if (fullscreen){
				sfml_application = new sf::RenderWindow(sf::VideoMode(resolution_width, resolution_height, bits_per_pixel), window_title, sf::Style::Fullscreen);
			}else{
				sfml_application = new sf::RenderWindow(sf::VideoMode(resolution_width, resolution_height, bits_per_pixel), window_title);
			}
		}

		bool SFMLInterface::IsWindowOpened(void){
			return opened;
		}

		void SFMLInterface::UpdateWindow(void){
			sfml_application->SetActive();
			// Process events
			sf::Event sfml_event;
			while (sfml_application->PollEvent(sfml_event)){
				if (sfml_event.Type == sf::Event::Closed || (sfml_event.Type == sf::Event::KeyPressed && sfml_event.Key.Code == sf::Keyboard::Escape)){
					opened = false;
				}else if (sfml_event.Type == sf::Event::Resized){
					resolution_width = sfml_event.Size.Width;
					resolution_height = sfml_event.Size.Height;
					window_resized = true;
				}else if (sfml_event.Type == sf::Event::KeyPressed){
					EG::Input::Key eg_key = key_translations.Get(sfml_event.Key.Code);
					input->KeyPressed(eg_key);
				}else if (sfml_event.Type == sf::Event::KeyReleased){
					input->KeyReleased(key_translations.Get(sfml_event.Key.Code));
				}else if (sfml_event.Type == sf::Event::MouseButtonPressed){
					input->MouseButtonPressed(mouse_translations.Get(sfml_event.MouseButton.Button));
				}else if (sfml_event.Type == sf::Event::MouseButtonReleased){
					input->MouseButtonReleased(mouse_translations.Get(sfml_event.MouseButton.Button));
				}else if (sfml_event.Type == sf::Event::MouseMoved){
					input->MouseMoved(glm::vec2(float(sfml_event.MouseMove.X), float(sfml_event.MouseMove.Y)));
				}else if (sfml_event.Type == sf::Event::MouseWheelMoved){
					//input->MouseWheelMoved(input_translations[sfml_event.Key.Code]);
				}else if (sfml_event.Type == sf::Event::TextEntered){
					//if (sfml_event.Text.Unicode < 128){
						std::string txt = "";
						txt += (static_cast<char>(sfml_event.Text.Unicode));
						input->SetTextEntered(txt);
					//}
				}
			}
		}

		void SFMLInterface::DisplayWindow(void){
			sfml_application->Display();
		}

		bool SFMLInterface::WindowResized(void){
			if (window_resized){
				window_resized = false;
				return true;
			}
			return false;
		}

		unsigned int SFMLInterface::GetResolutionWidth(void){
			return resolution_width;
		}

		unsigned int SFMLInterface::GetResolutionHeight(void){
			return resolution_height;
		}

		void SFMLInterface::SetInput(EG::Input::Input *_input){
			input = _input;
			key_translations.Set(sf::Keyboard::A, EG::Input::a);
			key_translations.Set(sf::Keyboard::B, EG::Input::b);
			key_translations.Set(sf::Keyboard::C, EG::Input::c);
			key_translations.Set(sf::Keyboard::D, EG::Input::d);
			key_translations.Set(sf::Keyboard::E, EG::Input::e);
			key_translations.Set(sf::Keyboard::F, EG::Input::f);
			key_translations.Set(sf::Keyboard::G, EG::Input::g);
			key_translations.Set(sf::Keyboard::H, EG::Input::h);
			key_translations.Set(sf::Keyboard::I, EG::Input::i);
			key_translations.Set(sf::Keyboard::J, EG::Input::j);
			key_translations.Set(sf::Keyboard::K, EG::Input::k);
			key_translations.Set(sf::Keyboard::L, EG::Input::l);
			key_translations.Set(sf::Keyboard::M, EG::Input::m);
			key_translations.Set(sf::Keyboard::N, EG::Input::n);
			key_translations.Set(sf::Keyboard::O, EG::Input::o);
			key_translations.Set(sf::Keyboard::P, EG::Input::p);
			key_translations.Set(sf::Keyboard::Q, EG::Input::q);
			key_translations.Set(sf::Keyboard::R, EG::Input::r);
			key_translations.Set(sf::Keyboard::S, EG::Input::s);
			key_translations.Set(sf::Keyboard::T, EG::Input::t);
			key_translations.Set(sf::Keyboard::U, EG::Input::u);
			key_translations.Set(sf::Keyboard::V, EG::Input::v);
			key_translations.Set(sf::Keyboard::W, EG::Input::w);
			key_translations.Set(sf::Keyboard::X, EG::Input::x);
			key_translations.Set(sf::Keyboard::Y, EG::Input::y);
			key_translations.Set(sf::Keyboard::Z, EG::Input::z);
			key_translations.Set(sf::Keyboard::Num1, EG::Input::one);
			key_translations.Set(sf::Keyboard::Num2, EG::Input::two);
			key_translations.Set(sf::Keyboard::Num3, EG::Input::three);
			key_translations.Set(sf::Keyboard::Num4, EG::Input::four);
			key_translations.Set(sf::Keyboard::Num5, EG::Input::five);
			key_translations.Set(sf::Keyboard::Num6, EG::Input::six);
			key_translations.Set(sf::Keyboard::Num7, EG::Input::seven);
			key_translations.Set(sf::Keyboard::Num8, EG::Input::eight);
			key_translations.Set(sf::Keyboard::Num9, EG::Input::nine);
			key_translations.Set(sf::Keyboard::Num0, EG::Input::zero);
			key_translations.Set(sf::Keyboard::Space, EG::Input::space);
			key_translations.Set(sf::Keyboard::Escape, EG::Input::escape);
			key_translations.Set(sf::Keyboard::Up, EG::Input::up);
			key_translations.Set(sf::Keyboard::Down, EG::Input::down);
			key_translations.Set(sf::Keyboard::Left, EG::Input::left);
			key_translations.Set(sf::Keyboard::Right, EG::Input::right);
			mouse_translations.Set(sf::Mouse::Left, EG::Input::mouse_left);
			mouse_translations.Set(sf::Mouse::Right, EG::Input::mouse_right);
			mouse_translations.Set(sf::Mouse::Middle, EG::Input::mouse_middle);
		}

		unsigned char *SFMLInterface::LoadImage(std::string file_path, bool *success, unsigned int *width, unsigned int *height){
			sf::Image sfml_image;
			if (sfml_image.LoadFromFile(file_path)){
				(*width) = sfml_image.GetWidth();
				(*height) = sfml_image.GetHeight();
				unsigned char *pixels = new unsigned char[(*width) * (*height) * 4];
				unsigned char *sixels = (unsigned char *)(sfml_image.GetPixelsPtr());
				for (unsigned int i = 0; i < (*height); i++){
					for (unsigned int j = 0; j < (*width); j++){
						//unsigned int source_index = (((*height) - i) * (*width) * 4) + (((*width) - j) * 4);
						unsigned int source_index = (i * (*width) * 4) + (j * 4);
						unsigned int dest_index = (i * (*width) * 4) + (j * 4);
						pixels[dest_index] = sixels[source_index];
						pixels[dest_index + 1] = sixels[source_index + 1];
						pixels[dest_index + 2] = sixels[source_index + 2];
						pixels[dest_index + 3] = sixels[source_index + 3];
					}
				}
				(*success) = true;
				return pixels;
			}
			(*success) = false;
			return NULL;
		}

		void SFMLInterface::ResetTime(void){
			clock->Reset();
		}

		unsigned int SFMLInterface::GetElapsedTime(void){
			return clock->GetElapsedTime();
		}

		void SFMLInterface::Close(void){
			sfml_application->Close();
		}
	}
}
