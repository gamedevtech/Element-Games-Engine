#ifndef EG_GUI_GUI_H
#define EG_GUI_GUI_H

#include <string>

#include <Awesomium/WebCore.h>
#include "../Math/Math.h"
#include "../Utility/Dictionary.h"
#include "../Graphics/OffscreenBuffer.h"
#include "../Graphics/Texture.h"

namespace EG{
    namespace GUI{
        class WebBuffer{
            public:
                WebBuffer(int _width, int _height, Awesomium::WebView *_web_view);
                ~WebBuffer(void);

                void Load(std::string url);
                void Render(void);
                unsigned int GetTextureId(void);
		void ExecuteScript(std::string script);
            private:
                EG::Graphics::Texture *texture;
                unsigned char *buffer;
                int bpp, rowspan, width, height;
                Awesomium::WebView *web_view;
        };

        class GUI{
            public:
                GUI(std::string url);
                ~GUI(void);

                //void InjectKey(void);
		void ExecuteScript(std::string script);
                void Update(void);
                void Render(void);
                unsigned int GetTextureId(void);
            private:
                WebBuffer *web_buffer;
                Awesomium::WebCore *web_core;
        };
    }
}

#endif
