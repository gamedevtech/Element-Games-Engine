#ifndef EG_GUI_GUI_H
#define EG_GUI_GUI_H

#include <string>

#include <Awesomium/WebCore.h>
#include "../Math/Math.h"
#include "../Utility/Dictionary.h"
#include "../Graphics/OffscreenBuffer.h"

namespace EG{
	namespace GUI{
		class GUI{
			public:
				GUI(std::string url);
				~GUI(void);

				//void InjectKey(void);
				void Update(void);
				void Render(void);
				EG::Graphics::OffscreenBuffer *GetBuffer(void);
			private:
				Awesomium::WebView *web_view;
				Awesomium::WebCore *web_core;
				EG::Graphics::OffscreenBuffer *buffer;
		};
	}
}

#endif
