#include "../GUI.h"
#include "../../Graphics/GraphicsSubsystem.h"
#include "../StringMethods.h"
#include <sstream>

namespace EG{
    namespace GUI{
        std::string WebResourceResponse::Call(std::map<std::string, std::string> args){
            return "";
        }

        // Singleton Stuff
        GUI *GUI::instance = NULL;
        GUI *GUI::Instance(void){
            if (instance == NULL){
                instance = new GUI();
            }
            return instance;
        }

        GUI::GUI(void){
            initialized = false;
        }

        GUI::~GUI(void){
            awe_webview_destroy(web_view);
            awe_webcore_shutdown();
        }

        void GUI::Initialize(std::string base_directory, std::string url){
            //awe_webcore_initialize_default();
            awe_webcore_initialize(true, true, true, awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), AWE_LL_NORMAL, false, awe_string_empty(), true, awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), true, 0, false, false, awe_string_empty());
            //awe_webcore_clear_cache();
            //awe_webcore_clear_cookies();
            awe_string *tempstr = awe_string_create_from_ascii(base_directory.c_str(), base_directory.size());
            awe_webcore_set_base_directory(tempstr);
            awe_string_destroy(tempstr);
            width = graphics->GetViewportWidth();
            height = graphics->GetViewportHeight();
            buffer = new unsigned char[width * height * 4];
            mime_type = awe_string_create_from_ascii("application/json", 16);

            web_view = awe_webcore_create_webview(width, height, false);
            tempstr = awe_string_create_from_ascii(url.c_str(), url.size());
            awe_string *tempstr2 = awe_string_create_from_ascii("default", 7);
            awe_webview_load_file(web_view, tempstr, tempstr2);
            texture = new EG::Graphics::Texture(buffer, width, height);
            awe_string_destroy(tempstr);
            awe_string_destroy(tempstr2);

            awe_webview_set_callback_resource_request(web_view, resource_interceptor);
            awe_webview_set_callback_js_console_message(web_view, js_console_callback);
            awe_webview_set_transparent(web_view, true);

            initialized = true;
        }

        bool GUI::GetInitialized(void){
            return initialized;
        }

        void GUI::Update(void){
            /*unsigned int temp_width = graphics->GetViewportWidth();
            unsigned int temp_height = graphics->GetViewportHeight();
            if (width != temp_width || height != temp_height) {
                width = temp_width;
                height = temp_height;
                std::cout << "Resizing Gui To: " << width << ", " << height << std::endl;
                //awe_webview_resize(web_view, width, height, true, 100000);
                awe_webview_resize(web_view, width, height, false, 100000);
            }*/
            awe_webcore_update();
        }

        void GUI::Render(void){
            awe_webview_focus(web_view);
            if (awe_webview_is_dirty(web_view)){
                const awe_renderbuffer *b = awe_webview_render(web_view);
                const unsigned char *bdata = awe_renderbuffer_get_buffer(b);
                for (unsigned int x = 0; x < width; x++){
                    for (unsigned int y = 0; y < height; y++){
                        int source_index = width * y + x;
                        int destination_index = width * (height - (y + 1)) + x;
                        const unsigned char *source = &(bdata[source_index * 4]);
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

        unsigned int GUI::GetTextureId(void){
            return texture->GetId();
        }

        void GUI::ExecuteScript(std::string script){
            awe_string *tempstr = awe_string_create_from_ascii(script.c_str(), script.size());
            awe_string *tempstr2 = awe_string_create_from_ascii("", 0);
            awe_webview_execute_javascript(web_view, tempstr, tempstr2);
            awe_string_destroy(tempstr);
            awe_string_destroy(tempstr2);
        }

        void GUI::InjectMouseMove(unsigned int x, unsigned int y){
            awe_webview_inject_mouse_move(web_view, x, y);
        }

        void GUI::InjectMouseDown(awe_mousebutton button){
            awe_webview_inject_mouse_down(web_view, button);
        }

        void GUI::InjectMouseUp(awe_mousebutton button){
            awe_webview_inject_mouse_up(web_view, button);
        }

        void GUI::InjectKeyPress(char key_code){
            awe_webkeyboardevent key_event;
#ifdef WIN32
            wchar16 wkey_code;
#else
            wchar_t wkey_code;
#endif
            awe_string *awe_key_codes = awe_string_create_from_ascii(&key_code, 1);
            awe_string_to_wide(awe_key_codes, &wkey_code, 2);
            key_event.text[0] = wkey_code;
            key_event.text[1] = NULL;
            key_event.virtual_key_code = key_code;
            key_event.virtual_key_code = NULL;
            key_event.unmodified_text[0] = key_code;
            key_event.unmodified_text[1] = NULL;
            key_event.native_key_code = 0;
            key_event.modifiers = 0;
            key_event.type = AWE_WKT_CHAR;
            key_event.is_system_key = false;
            //std::cout << int(key_code) << ": " << int(wkey_code) << std::endl;
            awe_webview_inject_keyboard_event(web_view, key_event);
            key_event.type = AWE_WKT_KEYDOWN;
            awe_webview_inject_keyboard_event(web_view, key_event);
            key_event.type = AWE_WKT_KEYUP;
            awe_webview_inject_keyboard_event(web_view, key_event);
            awe_string_destroy(awe_key_codes);
        }

        awe_resource_response *GUI::ResourceInterceptor(awe_webview *caller, awe_resource_request *request){
            std::string url;
            awe_string *awe_url = awe_resource_request_get_url(request);
            unsigned int string_length = awe_string_get_length(awe_url);
            char *c_url = new char[string_length + 1];
            awe_string_to_utf8(awe_url, c_url, string_length);
            c_url[string_length] = '\0';
            url = c_url;
            awe_string_destroy(awe_url);
            delete []c_url;

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

            if (resources.count(path)){
                std::string response = resources[path]->Call(args);
                unsigned char *text = reinterpret_cast<unsigned char *>(const_cast<char *>(response.c_str()));
                return awe_resource_response_create(response.size(), text, mime_type);
            }
            return NULL;
        }

        void GUI::AddResponseHandler(std::string url, WebResourceResponse *handler){
            resources[url] = handler;
        }

        awe_resource_response *resource_interceptor(awe_webview *caller, awe_resource_request *request){
            return gui->ResourceInterceptor(caller, request);
        }

        void js_console_callback(awe_webview *caller, const awe_string *message, int line_number, const awe_string *source){
            unsigned int string_length = awe_string_get_length(message);
            unsigned int source_length = awe_string_get_length(source);
            char *c_url = new char[string_length + 1];
            char *s = new char[source_length + 1];
            awe_string_to_utf8(message, c_url, string_length);
            awe_string_to_utf8(source, s, source_length);
            c_url[string_length] = '\0';
            s[source_length] = '\0';
            std::stringstream out;
            out << "JSConsole (" << line_number << "): " << c_url << std::endl << "Source: " << s;
            std::cout << out.str() << std::endl;
            delete []c_url;
        }
    }
}
