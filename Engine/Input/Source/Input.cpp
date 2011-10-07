#include "../Input.h"

namespace EG{
	namespace Input{
		Input::Input(void){
			//
		}

		Input::~Input(void){
			//
		}

		void Input::Update(void){
			key_toggles.clear();
			key_presses.clear();
			mouse_toggles.clear();
			mouse_presses.clear();
			mouse_movement_deltas = glm::vec2(0.0f, 0.0f);
			// text_entered.clear();
			// mouse_wheel_delta = 0.0f;
		}

		void Input::KeyPressed(EG::Input::Key key){
			key_downs[key] = true;
			key_presses[key] = true;
		}

		void Input::KeyReleased(EG::Input::Key key){
			if (key_downs.count(key) > 0){
				key_downs[key] = false;
				key_toggles[key] = true;
			}
		}

		void Input::MouseButtonPressed(EG::Input::Mouse mouse){
			mouse_downs[mouse] = true;
			mouse_presses[mouse] = true;
		}

		void Input::MouseButtonReleased(EG::Input::Mouse mouse){
			if (mouse_downs.count(mouse) > 0){
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

		bool Input::IsKeyDown(EG::Input::Key key){
			if (key_downs.count(key) > 0){
				if (key_downs[key]){
					return true;
				}
			}
			return false;
		}

		bool Input::IsKeyToggled(EG::Input::Key key){
			if (key_toggles[key]){
				return true;
			}
			return false;
		}

		bool Input::IsKeyPressed(EG::Input::Key key){
			if (key_presses[key]){
				return true;
			}
			return false;
		}

		bool Input::IsMouseDown(EG::Input::Mouse mouse){
			if (mouse_downs.count(mouse) > 0){
				if (mouse_downs[mouse]){
					return true;
				}
			}
			return false;
		}

		bool Input::IsMouseToggled(EG::Input::Mouse mouse){
			if (mouse_toggles[mouse]){
				return true;
			}
			return false;
		}

		bool Input::IsMousePressed(EG::Input::Mouse mouse){
			if (mouse_presses[mouse]){
				return true;
			}
			return false;
		}

		glm::vec2 Input::GetMousePosition(void){
			return mouse_current_position;
		}

		glm::vec2 Input::GetMouseDelta(void){
			return mouse_movement_deltas;
		}
	}
}
