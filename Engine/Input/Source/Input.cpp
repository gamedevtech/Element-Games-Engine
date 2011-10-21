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
			key_toggles.Clean();
			key_presses.Clean();
			key_releases.Clean();
			mouse_toggles.Clean();
			mouse_presses.Clean();
			mouse_releases.Clean();
			character_buffer.clear();
			mouse_movement_deltas = glm::vec2(0.0f, 0.0f);
			// text_entered.clear();
			// mouse_wheel_delta = 0.0f;
		}

		void Input::KeyPressed(EG::Input::Key key){
			key_downs.Set(key, true);
			key_presses.Set(key, true);
		}

		void Input::KeyReleased(EG::Input::Key key){
			key_releases.Set(key, true);
			if (key_downs.Has(key)){
				key_downs.Set(key, false);
				key_toggles.Set(key, true);
			}
		}

		void Input::MouseButtonPressed(EG::Input::Mouse mouse){
			mouse_downs.Set(mouse, true);
			mouse_presses.Set(mouse, true);
		}

		void Input::MouseButtonReleased(EG::Input::Mouse mouse){
			mouse_releases.Set(mouse, true);
			if (mouse_downs.Has(mouse)){
				mouse_downs.Set(mouse, false);
				mouse_toggles.Set(mouse, true);
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
			if (key_downs.Has(key)){
				return key_downs.Get(key);
			}
			return false;
		}

		bool Input::IsKeyToggled(EG::Input::Key key){
			if (key_toggles.Has(key)){
				return key_toggles.Get(key);
			}
			return false;
		}

		bool Input::IsKeyReleased(EG::Input::Key key){
			if (key_releases.Has(key)){
				return key_releases.Get(key);
			}
			return false;
		}

		bool Input::IsKeyPressed(EG::Input::Key key){
			if (key_presses.Has(key)){
				return key_presses.Get(key);
			}
			return false;
		}

		bool Input::IsMouseDown(EG::Input::Mouse mouse){
			if (mouse_downs.Has(mouse)){
				return mouse_downs.Get(mouse);
			}
			return false;
		}

		bool Input::IsMouseReleased(EG::Input::Mouse mouse){
			if (mouse_releases.Has(mouse)){
				return mouse_releases.Get(mouse);
			}
			return false;
		}

		bool Input::IsMouseToggled(EG::Input::Mouse mouse){
			if (mouse_toggles.Has(mouse)){
				return mouse_toggles.Get(mouse);
			}
			return false;
		}

		bool Input::IsMousePressed(EG::Input::Mouse mouse){
			if (mouse_presses.Has(mouse)){
				return mouse_presses.Get(mouse);
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
