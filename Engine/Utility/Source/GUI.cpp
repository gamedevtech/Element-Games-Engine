#include "../GUI.h"
#include "../../Graphics/GraphicsSubsystem.h"

namespace EG{
	namespace GUI{
		WebListener::WebListener(void){
			//
		}
		WebListener::~WebListener(void){
			//
		}
		void WebListener::onJavascriptConsoleMessage (Awesomium::WebView *caller, const std::wstring &message, int lineNumber, const std::wstring &source){
			std::cout << message.c_str() << std::endl;
		}
		void WebListener::onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args){
			if (callbacks.count(objectName) > 0){
				if (callbacks.at(objectName).count(callbackName) > 0){
					callbacks.at(objectName).at(callbackName)->Call(args);
				}
			}
		}
		void WebListener::AddCallback(std::wstring callback_object_name, std::wstring callback_name, ListenerCallback *callback){
			callbacks.at(callback_object_name).insert(std::pair<std::wstring, ListenerCallback *>(callback_name, callback));
		}
		void WebListener::onBeginNavigation(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName){}
		void WebListener::onBeginLoading(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType){}
		void WebListener::onFinishLoading(Awesomium::WebView* caller){}
		void WebListener::onReceiveTitle(Awesomium::WebView* caller, const std::wstring& title, const std::wstring& frameName){}
		void WebListener::onChangeTooltip(Awesomium::WebView* caller, const std::wstring& tooltip){}
		void WebListener::onChangeCursor(Awesomium::WebView* caller, Awesomium::CursorType cursor){}
		void WebListener::onChangeKeyboardFocus(Awesomium::WebView* caller, bool isFocused){}
		void WebListener::onChangeTargetURL(Awesomium::WebView* caller, const std::string& url){}
		void WebListener::onOpenExternalLink(Awesomium::WebView* caller, const std::string& url, const std::wstring& source){}
		void WebListener::onRequestDownload(Awesomium::WebView* caller, const std::string& url){}
		void WebListener::onWebViewCrashed(Awesomium::WebView* caller){}
		void WebListener::onPluginCrashed(Awesomium::WebView* caller, const std::wstring& pluginName){}
		void WebListener::onRequestMove(Awesomium::WebView* caller, int x, int y){}
		void WebListener::onGetPageContents(Awesomium::WebView* caller, const std::string& url, const std::wstring& contents){}
		void WebListener::onDOMReady(Awesomium::WebView* caller){}
		void WebListener::onRequestFileChooser(Awesomium::WebView* caller, bool selectMultipleFiles, const std::wstring& title, const std::wstring& defaultPath){}
		void WebListener::onGetScrollData(Awesomium::WebView* caller, int contentWidth, int contentHeight, int preferredWidth, int scrollX, int scrollY){}
		void WebListener::onGetFindResults(Awesomium::WebView* caller, int requestID, int numMatches, const Awesomium::Rect& selection, int curMatch, bool finalUpdate){}
		void WebListener::onUpdateIME(Awesomium::WebView* caller, Awesomium::IMEState imeState, const Awesomium::Rect& caretRect){}
		void WebListener::onShowContextMenu(Awesomium::WebView* caller, int mouseX, int mouseY, Awesomium::MediaType type, int mediaState, const std::string& linkURL, const std::string& srcURL, const std::string& pageURL, const std::string& frameURL, const std::wstring& selectionText, bool isEditable, int editFlags){}
		void WebListener::onRequestLogin(Awesomium::WebView* caller, int requestID, const std::string& requestURL, bool isProxy, const std::wstring& hostAndPort, const std::wstring& scheme, const std::wstring& realm){}
		void WebListener::onChangeHistory(Awesomium::WebView* caller, int backCount, int forwardCount){}
		void WebListener::onFinishResize(Awesomium::WebView* caller, int width, int height){}
		void WebListener::onShowJavascriptDialog(Awesomium::WebView* caller, int requestID, int dialogFlags, const std::wstring& message, const std::wstring& defaultPrompt, const std::string& frameURL){}

		WebBuffer::WebBuffer(int _width, int _height, Awesomium::WebView *_web_view){
			width = _width;
			height = _height;
			web_view = _web_view;
			web_view->setTransparent(true);
			listener = new WebListener();
			web_view->setListener(listener);
			bpp = 4;

			rowspan = width * bpp;
			buffer = new unsigned char[rowspan * height];
			texture = new EG::Graphics::Texture(buffer, width, height);
		}
		WebBuffer::~WebBuffer(void){
			web_view->destroy();
		}
		void WebBuffer::Load(std::string url){
			web_view->loadFile(url);
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
		void WebBuffer::InjectMouseMove(unsigned int x, unsigned int y){
			web_view->injectMouseMove(x, y);
		}
		void WebBuffer::InjectMouseDown(Awesomium::MouseButton button){
			web_view->injectMouseDown(button);
		}
		void WebBuffer::InjectMouseUp(Awesomium::MouseButton button){
			web_view->injectMouseUp(button);
		}
		void WebBuffer::InjectKeyPress(Awesomium::WebKeyboardEvent keyboard_event){
			web_view->injectKeyboardEvent(keyboard_event);
		}
		void WebBuffer::InjectKeyPress(int key_code){
			Awesomium::WebKeyboardEvent key_event;
			char* buf = new char[20];
			key_event.virtualKeyCode = key_code;
			Awesomium::getKeyIdentifierFromVirtualKeyCode(key_event.virtualKeyCode, &buf);
			strcpy(key_event.keyIdentifier, buf);
			delete[] buf;
			key_event.modifiers = 0;
			key_event.nativeKeyCode = 0;
			key_event.type = Awesomium::WebKeyboardEvent::TYPE_KEY_DOWN;
			web_view->injectKeyboardEvent(key_event);
			key_event.type = Awesomium::WebKeyboardEvent::TYPE_KEY_UP;
			web_view->injectKeyboardEvent(key_event);
		}
		void WebBuffer::AddCallback(std::wstring callback_object_name, std::wstring callback_name, ListenerCallback *callback){
			web_view->createObject(callback_object_name);
			web_view->setObjectCallback(callback_object_name, callback_name);
			listener->AddCallback(callback_object_name, callback_name, callback);
		}

		GUI::GUI(std::string base_directory, std::string url){
			Awesomium::WebCoreConfig conf;
			conf.setEnablePlugins(true);
			conf.setSaveCacheAndCookies(false);
			conf.setLogLevel(Awesomium::LOG_VERBOSE);
			web_core = new Awesomium::WebCore(conf);
			Awesomium::WebView *web_view = web_core->createWebView(graphics->GetViewportWidth(), graphics->GetViewportHeight());
			web_buffer = new WebBuffer(graphics->GetViewportWidth(), graphics->GetViewportHeight(), web_view);
			web_core->setBaseDirectory(base_directory);
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
		void GUI::InjectMouseMove(unsigned int x, unsigned int y){
			web_buffer->InjectMouseMove(x, y);
		}
		void GUI::InjectMouseDown(Awesomium::MouseButton button){
			web_buffer->InjectMouseDown(button);
		}
		void GUI::InjectMouseUp(Awesomium::MouseButton button){
			web_buffer->InjectMouseUp(button);
		}
		void GUI::InjectKeyPress(Awesomium::WebKeyboardEvent keyboard_event){
			web_buffer->InjectKeyPress(keyboard_event);
		}
		void GUI::InjectKeyPress(int key_press){
			web_buffer->InjectKeyPress(key_press);
		}
		void GUI::AddCallback(std::wstring callback_object_name, std::wstring callback_name, ListenerCallback *callback){
			web_buffer->AddCallback(callback_object_name, callback_name, callback);
		}
	}
}
