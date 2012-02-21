#ifndef EG_GUI_GUI_H
#define EG_GUI_GUI_H

#include <iostream>
#include <string>
#include <algorithm>

#include <Awesomium/WebCore.h>
#include "../Math/Math.h"
#include "../Utility/StringDictionary.h"
#include "../Graphics/OffscreenBuffer.h"
#include "../Graphics/Texture.h"

namespace EG{
	namespace GUI{
		std::wstring StringToWString(const std::string& s);
		std::string WStringToString(const std::wstring& s);

		class ListenerCallback{
			public:
				ListenerCallback(void){}
				~ListenerCallback(void){}

				virtual void Call(const Awesomium::JSArguments &args, Awesomium::JSValue &response) = 0;
		};

		class WebListener : public Awesomium::WebViewListener{
			public:
				WebListener(void);
				~WebListener(void);
				void AddCallback(std::wstring callback_object_name, std::wstring callback_name, ListenerCallback *callback);

				virtual void onBeginNavigation(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName);
				virtual void onBeginLoading(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType);
				virtual void onFinishLoading(Awesomium::WebView* caller);
				virtual void onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args);
				virtual void onReceiveTitle(Awesomium::WebView* caller, const std::wstring& title, const std::wstring& frameName);
				virtual void onChangeTooltip(Awesomium::WebView* caller, const std::wstring& tooltip);
				virtual void onChangeCursor(Awesomium::WebView* caller, Awesomium::CursorType cursor);
				virtual void onChangeKeyboardFocus(Awesomium::WebView* caller, bool isFocused);
				virtual void onChangeTargetURL(Awesomium::WebView* caller, const std::string& url);
				virtual void onOpenExternalLink(Awesomium::WebView* caller, const std::string& url, const std::wstring& source);
				virtual void onRequestDownload(Awesomium::WebView* caller, const std::string& url);
				virtual void onWebViewCrashed(Awesomium::WebView* caller);
				virtual void onPluginCrashed(Awesomium::WebView* caller, const std::wstring& pluginName);
				virtual void onRequestMove(Awesomium::WebView* caller, int x, int y);
				virtual void onGetPageContents(Awesomium::WebView* caller, const std::string& url, const std::wstring& contents);
				virtual void onDOMReady(Awesomium::WebView* caller);
				virtual void onRequestFileChooser(Awesomium::WebView* caller, bool selectMultipleFiles, const std::wstring& title, const std::wstring& defaultPath);
				virtual void onGetScrollData(Awesomium::WebView* caller, int contentWidth, int contentHeight, int preferredWidth, int scrollX, int scrollY);
				virtual void onJavascriptConsoleMessage(Awesomium::WebView *caller, const std::wstring &message, int lineNumber, const std::wstring &source);
				virtual void onGetFindResults(Awesomium::WebView* caller, int requestID, int numMatches, const Awesomium::Rect& selection, int curMatch, bool finalUpdate);
				virtual void onUpdateIME(Awesomium::WebView* caller, Awesomium::IMEState imeState, const Awesomium::Rect& caretRect);
				virtual void onShowContextMenu(Awesomium::WebView* caller, int mouseX, int mouseY, Awesomium::MediaType type, int mediaState, const std::string& linkURL, const std::string& srcURL, const std::string& pageURL, const std::string& frameURL, const std::wstring& selectionText, bool isEditable, int editFlags);
				virtual void onRequestLogin(Awesomium::WebView* caller, int requestID, const std::string& requestURL, bool isProxy, const std::wstring& hostAndPort, const std::wstring& scheme, const std::wstring& realm);
				virtual void onChangeHistory(Awesomium::WebView* caller, int backCount, int forwardCount);
				virtual void onFinishResize(Awesomium::WebView* caller, int width, int height);
				virtual void onShowJavascriptDialog(Awesomium::WebView* caller, int requestID, int dialogFlags, const std::wstring& message, const std::wstring& defaultPrompt, const std::string& frameURL);
			private:
				std::map<std::wstring, std::map<std::wstring, ListenerCallback *> > callbacks;
		};

		class WebResourceResponse{
			public:
				WebResourceResponse(void){}
				~WebResourceResponse(void){}
				virtual std::string Call(std::map<std::string, std::string> args);
		};

		class WebResources : public Awesomium::ResourceInterceptor{
			public:
				virtual Awesomium::ResourceResponse* onRequest(Awesomium::WebView *caller, Awesomium::ResourceRequest *request);
				virtual void onResponse (Awesomium::WebView *caller, const std::string &url, int statusCode, const Awesomium::ResourceResponseMetrics &metrics);
		                void AddResponseHandler(std::string url, WebResourceResponse *handler);
			private:
				std::map<std::string, WebResourceResponse *> callbacks;
		};

		class WebBuffer{
			public:
				WebBuffer(int _width, int _height, Awesomium::WebView *_web_view);
				~WebBuffer(void);

				void Load(std::string url);
				void Render(void);
				unsigned int GetTextureId(void);
				void ExecuteScript(std::string script);
				void InjectMouseMove(unsigned int x, unsigned int y);
				void InjectMouseDown(Awesomium::MouseButton button);
				void InjectMouseUp(Awesomium::MouseButton button);
				void InjectKeyPress(Awesomium::WebKeyboardEvent keyboard_event);
				void InjectKeyPress(int key_code);
				void AddCallback(std::wstring callback_name, ListenerCallback *callback);
                void AddResponseHandler(std::string url, WebResourceResponse *handler);
			private:
				EG::Graphics::Texture *texture;
				unsigned char *buffer;
				int bpp, rowspan, width, height;
				Awesomium::WebView *web_view;
				WebListener *listener;
				WebResources *resources;
		};

		class GUI{
			public:
				GUI(std::string base_directory, std::string url);
				~GUI(void);

				void InjectMouseMove(unsigned int x, unsigned int y);
				void InjectMouseDown(Awesomium::MouseButton button);
				void InjectMouseUp(Awesomium::MouseButton button);
				void InjectKeyPress(Awesomium::WebKeyboardEvent keyboard_event);
				void InjectKeyPress(int key_press);
				void ExecuteScript(std::string script);
				void Update(void);
				void Render(void);
				unsigned int GetTextureId(void);
				void AddCallback(std::wstring callback_name, ListenerCallback *callback);
                void AddResponseHandler(std::string url, WebResourceResponse *handler);
			private:
				WebBuffer *web_buffer;
				Awesomium::WebCore *web_core;
		};
	}
}

#endif
