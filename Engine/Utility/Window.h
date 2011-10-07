#ifndef EG_UTILITY_WINDOW_H
#define EG_UTILITY_WINDOW_H

#include <string>
#include "../Media/SFMLInterface.h"
#include "../Graphics/GraphicsSubsystem.h"

namespace EG{
	namespace Utility{
		class Window{
			public:
				Window(float _resolution_width, float _resolution_height, unsigned int _bits_per_pixel, bool _fullscreen, std::string _window_title);
				~Window(void);

				bool IsOpened(void);
				void Update(void);
				void Display(void);

				float GetResolutionWidth(void);
				float GetResolutionHeight(void);
			private:
				float resolution_width, resolution_height;
				unsigned int bits_per_pixel;
				bool fullscreen;
				std::string window_title;
		};
	}
}

#endif
