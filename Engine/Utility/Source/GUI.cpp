#include "../GUI.h"
#include "../../Graphics/GraphicsSubsystem.h"

namespace EG{
    namespace GUI{
        WebBuffer::WebBuffer(int _width, int _height, Awesomium::WebView *_web_view){
            width = _width;
            height = _height;
            web_view = _web_view;
            bpp = 4;

            rowspan = width * bpp;
            buffer = new unsigned char[rowspan * height];
            texture = new EG::Graphics::Texture(buffer, width, height);
        }

        WebBuffer::~WebBuffer(void){
            web_view->destroy();
        }

        void WebBuffer::Load(std::string url){
            web_view->loadURL(url);
        }

        void WebBuffer::Render(void){
            web_view->focus();
            if (web_view->isDirty()){
                const Awesomium::RenderBuffer* b = web_view->render();
		for (unsigned int x = 0; x < width; x++){
			for (unsigned int y = 0; y < height; y++){
				int source_index = width * y + x;
				int destination_index = width * (height - (y + 1)) + x;
				unsigned char *source = &(b->buffer[source_index * 4]);
				unsigned char *destination = &(buffer[destination_index * 4]);
				destination[2] = 255 - source[0];
				destination[1] = 255 - source[1];
				destination[0] = 255 - source[2];
				destination[3] = source[3];
			}
		}
		texture->UpdateImage(buffer);
            }
        }

        unsigned int WebBuffer::GetTextureId(void){
            return texture->GetId();
        }

	void WebBuffer::ExecuteScript(std::string script){
		web_view->executeJavascript(script);
	}

        GUI::GUI(std::string url){
            Awesomium::WebCoreConfig conf;
            conf.setEnablePlugins(true);
            conf.setSaveCacheAndCookies(false);
            conf.setLogLevel(Awesomium::LOG_VERBOSE);
            web_core = new Awesomium::WebCore(conf);
            Awesomium::WebView *web_view = web_core->createWebView(graphics->GetViewportWidth(), graphics->GetViewportHeight());
            web_buffer = new WebBuffer(graphics->GetViewportWidth(), graphics->GetViewportHeight(), web_view);
            web_buffer->Load(url);
            std::cout << "GUI: " << url << std::endl;
        }
        GUI::~GUI(void){
            delete web_buffer;
            delete web_core;
        }
        void GUI::Update(void){
            web_core->update();
        }
        void GUI::Render(void){
            web_buffer->Render();
        }
        unsigned int GUI::GetTextureId(void){
            return web_buffer->GetTextureId();
        }
	void GUI::ExecuteScript(std::string script){
		web_buffer->ExecuteScript(script);
	}
    }
}
