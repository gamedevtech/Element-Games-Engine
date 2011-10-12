#ifndef EG_MEDIA_SFML_INTERFACE_H
#define EG_MEDIA_SFML_INTERFACE_H

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include "../Input/Input.h"
#include "../Utility/Dictionary.h"

#define sfml_interface EG::Media::SFMLInterface::Instance()

namespace EG{
	namespace Media{
		class SFMLInterface{
			public:
				SFMLInterface(void);
				~SFMLInterface(void);

				// Window
				void Initialize(unsigned int _resolution_width, unsigned int _resolution_height, unsigned int _bits_per_pixel, bool _fullscreen, std::string _window_title);
				bool IsWindowOpened(void);
				void UpdateWindow(void);
				void DisplayWindow(void);
				bool WindowResized(void);
				unsigned int GetResolutionWidth(void);
				unsigned int GetResolutionHeight(void);
				void Close(void);

				// Input
				void SetInput(EG::Input::Input *_input);

				// Audio

				// Networking

				// Images
				unsigned char *LoadImage(std::string file_path, bool *success, unsigned int *width, unsigned int *height);

				// Time
				void ResetTime(void);
				unsigned int GetElapsedTime(void);

				static SFMLInterface *Instance(void);
			private:
				unsigned int resolution_width, resolution_height;
				unsigned int bits_per_pixel;
				bool fullscreen, window_resized;
				std::string window_title;
				bool opened;

				sf::RenderWindow *sfml_application;

				// Input
				EG::Input::Input *input;
				EG::Utility::Dictionary<sf::Keyboard::Key, EG::Input::Key> key_translations;
				EG::Utility::Dictionary<sf::Mouse::Button, EG::Input::Mouse> mouse_translations;

				// Time
				sf::Clock *clock;

				static SFMLInterface* instance;
		};
	}
}

#endif
