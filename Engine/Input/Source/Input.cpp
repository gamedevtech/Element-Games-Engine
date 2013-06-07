#include "../Input.h"

namespace EG{
    namespace Input{
        Input::Input(void){
            //
        }

        Input::~Input(void){
            /*delete key_toggles;
            delete key_presses;
            delete key_downs;
            delete mouse_toggles;
            delete mouse_presses;
            delete mouse_downs;*/
        }

        void Input::Update(void){
            key_toggles.clear();
                        key_presses.clear();
            key_releases.clear();
            mouse_toggles.clear();
            mouse_presses.clear();
            mouse_releases.clear();
            character_buffer.clear();
            mouse_movement_deltas = glm::vec2(0.0f, 0.0f);
            // text_entered.clear();
            // mouse_wheel_delta = 0.0f;
        }

        void Input::KeyPressed(EG::Input::Key key){
            key_downs[key] = true;
            key_presses[key] = true;
        }

        void Input::KeyReleased(EG::Input::Key key){
            key_releases[key] = true;
            if (key_downs.count(key)){
                key_downs[key] = false;
                key_toggles[key] = true;
            }
        }

        void Input::MouseButtonPressed(EG::Input::Mouse mouse){
            mouse_downs[mouse] = true;
            mouse_presses[mouse] = true;
        }

        void Input::MouseButtonReleased(EG::Input::Mouse mouse){
            mouse_releases[mouse] = true;
            if (mouse_downs.count(mouse)){
                mouse_downs[mouse] = false;
                mouse_toggles[mouse] = true;
            }
        }

        void Input::MouseMoved(glm::vec2 mouse_position){
            mouse_last_position = mouse_current_position;
            mouse_current_position = mouse_position;
            mouse_movement_deltas = mouse_last_position - mouse_current_position;
            mouse_movement_deltas = glm::vec2(((-mouse_movement_deltas.x) * 3.5f), (mouse_movement_deltas.y * 3.5f));
        }

        void Input::SetTextEntered(char c){
            character_buffer.push_back(c);
        }

        bool Input::IsKeyDown(EG::Input::Key key){
            if (key_downs.count(key)){
                return key_downs[key];
            }
            return false;
        }

        bool Input::IsKeyToggled(EG::Input::Key key){
            if (key_toggles.count(key)){
                return key_toggles[key];
            }
            return false;
        }

        bool Input::IsKeyReleased(EG::Input::Key key){
            if (key_releases.count(key)){
                return key_releases[key];
            }
            return false;
        }

        bool Input::IsKeyPressed(EG::Input::Key key){
            if (key_presses.count(key)){
                return key_presses[key];
            }
            return false;
        }

        bool Input::IsMouseDown(EG::Input::Mouse mouse){
            if (mouse_downs.count(mouse)){
                return mouse_downs[mouse];
            }
            return false;
        }

        bool Input::IsMouseReleased(EG::Input::Mouse mouse){
            if (mouse_releases.count(mouse)){
                return mouse_releases[mouse];
            }
            return false;
        }

        bool Input::IsMouseToggled(EG::Input::Mouse mouse){
            if (mouse_toggles.count(mouse)){
                return mouse_toggles[mouse];
            }
            return false;
        }

        bool Input::IsMousePressed(EG::Input::Mouse mouse){
            if (mouse_presses.count(mouse)){
                return mouse_presses[mouse];
            }
            return false;
        }

        glm::vec2 Input::GetMousePosition(void){
            return mouse_current_position;
        }

        glm::vec2 Input::GetMouseDelta(void){
            return mouse_movement_deltas;
        }

        std::vector<char> Input::GetTextEntered(void){
            return character_buffer;
        }
    }
}
