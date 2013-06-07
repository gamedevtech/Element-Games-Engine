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
            clock->restart();

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
            sfml_application->setActive();
            // Process events
            sf::Event sfml_event;
            while (sfml_application->pollEvent(sfml_event)){
                if (sfml_event.type == sf::Event::Closed || (sfml_event.type == sf::Event::KeyPressed && sfml_event.key.code == sf::Keyboard::Escape)){
                    opened = false;
                }else if (sfml_event.type == sf::Event::Resized){
                    resolution_width = sfml_event.size.width;
                    resolution_height = sfml_event.size.height;
                    window_resized = true;
                }else if (sfml_event.type == sf::Event::KeyPressed){
                    EG::Input::Key eg_key = key_translations[sfml_event.key.code];
                    input->KeyPressed(eg_key);
                }else if (sfml_event.type == sf::Event::KeyReleased){
                    input->KeyReleased(key_translations[sfml_event.key.code]);
                }else if (sfml_event.type == sf::Event::MouseButtonPressed){
                    input->MouseButtonPressed(mouse_translations[sfml_event.mouseButton.button]);
                }else if (sfml_event.type == sf::Event::MouseButtonReleased){
                    input->MouseButtonReleased(mouse_translations[sfml_event.mouseButton.button]);
                }else if (sfml_event.type == sf::Event::MouseMoved){
                    input->MouseMoved(glm::vec2(float(sfml_event.mouseMove.x), float(sfml_event.mouseMove.y)));
                }else if (sfml_event.type == sf::Event::MouseWheelMoved){
                    //input->MouseWheelMoved(input_translations[sfml_event.Key.Code]);
                }else if (sfml_event.type == sf::Event::TextEntered){
                    if (sfml_event.text.unicode > 31){ // 32 is a space
                        input->SetTextEntered(static_cast<char>(sfml_event.text.unicode));
                    }
                }
            }
        }

        void SFMLInterface::DisplayWindow(void){
            sfml_application->display();
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
            key_translations[sf::Keyboard::A] = EG::Input::a;
            key_translations[sf::Keyboard::B] = EG::Input::b;
            key_translations[sf::Keyboard::C] = EG::Input::c;
            key_translations[sf::Keyboard::D] = EG::Input::d;
            key_translations[sf::Keyboard::E] = EG::Input::e;
            key_translations[sf::Keyboard::F] = EG::Input::f;
            key_translations[sf::Keyboard::G] = EG::Input::g;
            key_translations[sf::Keyboard::H] = EG::Input::h;
            key_translations[sf::Keyboard::I] = EG::Input::i;
            key_translations[sf::Keyboard::J] = EG::Input::j;
            key_translations[sf::Keyboard::K] = EG::Input::k;
            key_translations[sf::Keyboard::L] = EG::Input::l;
            key_translations[sf::Keyboard::M] = EG::Input::m;
            key_translations[sf::Keyboard::N] = EG::Input::n;
            key_translations[sf::Keyboard::O] = EG::Input::o;
            key_translations[sf::Keyboard::P] = EG::Input::p;
            key_translations[sf::Keyboard::Q] = EG::Input::q;
            key_translations[sf::Keyboard::R] = EG::Input::r;
            key_translations[sf::Keyboard::S] = EG::Input::s;
            key_translations[sf::Keyboard::T] = EG::Input::t;
            key_translations[sf::Keyboard::U] = EG::Input::u;
            key_translations[sf::Keyboard::V] = EG::Input::v;
            key_translations[sf::Keyboard::W] = EG::Input::w;
            key_translations[sf::Keyboard::X] = EG::Input::x;
            key_translations[sf::Keyboard::Y] = EG::Input::y;
            key_translations[sf::Keyboard::Z] = EG::Input::z;
            key_translations[sf::Keyboard::Num1] = EG::Input::one;
            key_translations[sf::Keyboard::Num2] = EG::Input::two;
            key_translations[sf::Keyboard::Num3] = EG::Input::three;
            key_translations[sf::Keyboard::Num4] = EG::Input::four;
            key_translations[sf::Keyboard::Num5] = EG::Input::five;
            key_translations[sf::Keyboard::Num6] = EG::Input::six;
            key_translations[sf::Keyboard::Num7] = EG::Input::seven;
            key_translations[sf::Keyboard::Num8] = EG::Input::eight;
            key_translations[sf::Keyboard::Num9] = EG::Input::nine;
            key_translations[sf::Keyboard::Num0] = EG::Input::zero;
            key_translations[sf::Keyboard::Space] = EG::Input::space;
            key_translations[sf::Keyboard::Escape] = EG::Input::escape;
            key_translations[sf::Keyboard::Up] = EG::Input::up;
            key_translations[sf::Keyboard::Down] = EG::Input::down;
            key_translations[sf::Keyboard::Left] = EG::Input::left;
            key_translations[sf::Keyboard::Right] = EG::Input::right;
            key_translations[sf::Keyboard::BackSpace] = EG::Input::back_space;
            key_translations[sf::Keyboard::Delete] = EG::Input::del;
            key_translations[sf::Keyboard::Tab] = EG::Input::tab;
            key_translations[sf::Keyboard::Return] = EG::Input::enter;
            mouse_translations[sf::Mouse::Left] = EG::Input::mouse_left;
            mouse_translations[sf::Mouse::Right] = EG::Input::mouse_right;
            mouse_translations[sf::Mouse::Middle] = EG::Input::mouse_middle;
        }

        unsigned char *SFMLInterface::LoadImage(std::string file_path, bool *success, unsigned int *width, unsigned int *height){
            sf::Image sfml_image;
            if (sfml_image.loadFromFile(file_path)){
                sf::Vector2u size = sfml_image.getSize();
                (*width) = size.x;
                (*height) = size.y;
                unsigned char *pixels = new unsigned char[(*width) * (*height) * 4];
                unsigned char *sixels = (unsigned char *)(sfml_image.getPixelsPtr());
                for (unsigned int i = 0; i < (*height); i++){
                    for (unsigned int j = 0; j < (*width); j++){
                        unsigned int source_index = (((*height) - (i + 1)) * (*width) * 4) + (j * 4);
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
            clock->restart();
        }

        unsigned int SFMLInterface::GetElapsedTime(void){
            sf::Time time = clock->getElapsedTime();
            return time.asMilliseconds();
        }

        void SFMLInterface::Close(void){
            //sfml_application->close();
            opened = false;
        }
    }
}
