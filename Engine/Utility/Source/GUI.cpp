#include "../GUI.h"
#include "../../Graphics/GraphicsSubsystem.h"

namespace EG{
	namespace GUI{
		GUI::GUI(std::string url){
			web_core = new Awesomium::WebCore();
			web_view = web_core->createWebView(graphics->GetViewportWidth(), graphics->GetViewportHeight());
			web_view->loadURL(url);
			buffer = new EG::Graphics::OffscreenBuffer(graphics->GetViewportWidth(), graphics->GetViewportHeight(), 1, false, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_NONE);
		}
		GUI::~GUI(void){
			web_view->destroy();
			delete web_core;
		}
		void GUI::Update(void){
			web_core->update();
		}
		void GUI::Render(void){
			web_view->focus();
			const Awesomium::RenderBuffer* b = web_view->render();
			graphics->StartOffscreenRender(buffer->GetBufferId(), 0, graphics->GetViewportWidth(), graphics->GetViewportHeight());
			glDrawPixels(graphics->GetViewportWidth(), graphics->GetViewportHeight(), GL_BGRA, GL_UNSIGNED_BYTE, b->buffer);
			graphics->EndOffscreenRender();
		}
		EG::Graphics::OffscreenBuffer *GUI::GetBuffer(void){
			return buffer;
		}
	}
}
