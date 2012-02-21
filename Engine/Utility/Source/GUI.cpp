#include "../GUI.h"
#include "../../Graphics/GraphicsSubsystem.h"
#include "../StringMethods.h"

namespace EG{
	namespace GUI{
		std::wstring StringToWString(const std::string& s){
			std::wstring temp(s.length(),L' ');
			std::copy(s.begin(), s.end(), temp.begin());
			return temp;
		}

		std::string WStringToString(const std::wstring& s){
			std::string temp(s.length(), ' ');
			std::copy(s.begin(), s.end(), temp.begin());
			return temp;
		}

		WebListener::WebListener(void){
			//
		}
		WebListener::~WebListener(void){
			//
		}
		void WebListener::onJavascriptConsoleMessage (Awesomium::WebView *caller, const std::wstring &message, int lineNumber, const std::wstring &source){
			std::wcout << message << std::endl;
		}
		void WebListener::onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args){
			//http://forums.wolfire.com/viewtopic.php?f=2&t=6658
			if (callbacks.count(objectName) > 0){
				if (callbacks.at(objectName).count(callbackName) > 0){
					Awesomium::JSValue result;
					callbacks.at(objectName).at(callbackName)->Call(args, result);
					caller->executeJavascript(L"window.set_response(" + result.toString() + L");");
					//caller->setObjectProperty(L"backend", L"result", result);
				}
			}
		}
		void WebListener::AddCallback(std::wstring callback_object_name, std::wstring callback_name, ListenerCallback *callback){
			callbacks[callback_object_name][callback_name] = callback;
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

		std::string WebResourceResponse::Call(std::map<std::string, std::string> args){
			return "";
		}

		Awesomium::ResourceResponse* WebResources::onRequest(Awesomium::WebView *caller, Awesomium::ResourceRequest *request){
			std::string url = request->getURL();
			//std::cout << "onRequest: " << url << std::endl;
			unsigned int fspos = url.find_last_of('/');
			unsigned int qpos = url.find_last_of('?');
			std::string path = "";
			std::string argstr = "";
				if (qpos < 0){
					path = url.substr(fspos + 1);
			}else{
				path = url.substr(fspos + 1, qpos - fspos - 1);
				argstr = url.substr(qpos + 1);
			}
			std::vector<std::string> argtokens = EG::Utility::StringMethods::Tokenize(argstr, "&");
			std::vector<std::string>::iterator token_iter = argtokens.begin();
			std::map<std::string, std::string> args;
			while (token_iter != argtokens.end()){
				std::string token = *token_iter;
				unsigned int epos = token.find_first_of('=');
				std::string name = token.substr(0, epos);
				std::string value = token.substr(epos + 1);
				args[name] = value;
				++token_iter;
			}

			if ((callbacks.count(path)) > 0){
				std::string response = callbacks[path]->Call(args);
				unsigned char *text = reinterpret_cast<unsigned char *>(const_cast<char *>(response.c_str()));
				Awesomium::ResourceResponse *resource_response = Awesomium::ResourceResponse::Create(response.size(), text, "application/json");
				return resource_response;
			}
			return NULL;
		}
		void WebResources::onResponse(Awesomium::WebView *caller, const std::string &url, int statusCode, const Awesomium::ResourceResponseMetrics &metrics){
			//std::cout << "onResponse: " << url << std::endl;
		}
			void WebResources::AddResponseHandler(std::string url, WebResourceResponse *handler){
					callbacks[url] = handler;
			}

		WebBuffer::WebBuffer(int _width, int _height, Awesomium::WebView *_web_view){
			width = _width;
			height = _height;
			web_view = _web_view;
			web_view->setTransparent(true);
			listener = new WebListener();
			web_view->setListener(listener);
			resources = new WebResources();
			web_view->setResourceInterceptor(resources);
			bpp = 4;
			web_view->createObject(L"backend");

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
		void WebBuffer::AddCallback(std::wstring callback_name, ListenerCallback *callback){
			web_view->setObjectCallback(L"backend", callback_name);
			listener->AddCallback(L"backend", callback_name, callback);
		}
		void WebBuffer::AddResponseHandler(std::string url, WebResourceResponse *handler){
			resources->AddResponseHandler(url, handler);
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
		void GUI::AddCallback(std::wstring callback_name, ListenerCallback *callback){
			web_buffer->AddCallback(callback_name, callback);
		}
		void GUI::AddResponseHandler(std::string url, WebResourceResponse *handler){
			web_buffer->AddResponseHandler(url, handler);
		}
	}
}
