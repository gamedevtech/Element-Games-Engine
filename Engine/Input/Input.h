#ifndef EG_INPUT_INPUT_H
#define EG_INPUT_INPUT_H

#include <vector>
#include <unordered_map>
#include "../Math/Math.h"

namespace EG{
    namespace Input{
        typedef enum Key{
            a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, one, two, three, four, five, six, seven, eight, nine, zero,
            space, escape, up, down, left, right, back_space, tab, del, enter
        } Key;

        typedef enum Mouse{
            mouse_left, mouse_right, mouse_middle, mouse_scroll_up, mouse_scroll_down
        } Mouse;

        class Input{
            public:
                Input(void);
                ~Input(void);

                void Update(void);

                void KeyPressed(EG::Input::Key key);
                void KeyReleased(EG::Input::Key key);
                void MouseButtonPressed(EG::Input::Mouse mouse);
                void MouseButtonReleased(EG::Input::Mouse mouse);
                void MouseMoved(glm::vec2 mouse_position);
                void SetTextEntered(char c);

                bool IsKeyDown(EG::Input::Key key);
                bool IsKeyToggled(EG::Input::Key key);
                bool IsKeyPressed(EG::Input::Key key);
                bool IsKeyReleased(EG::Input::Key key);
                bool IsMouseDown(EG::Input::Mouse mouse);
                bool IsMouseToggled(EG::Input::Mouse mouse);
                bool IsMousePressed(EG::Input::Mouse mouse);
                bool IsMouseReleased(EG::Input::Mouse mouse);
                glm::vec2 GetMousePosition(void);
                glm::vec2 GetMouseDelta(void);
                std::vector<char> GetTextEntered(void);
            private:
                std::unordered_map<EG::Input::Key, bool, std::hash<unsigned int> > key_downs;
                std::unordered_map<EG::Input::Key, bool, std::hash<unsigned int> > key_toggles;
                std::unordered_map<EG::Input::Key, bool, std::hash<unsigned int> > key_presses;
                std::unordered_map<EG::Input::Key, bool, std::hash<unsigned int> > key_releases;

                glm::vec2 mouse_movement_deltas, mouse_last_position, mouse_current_position;
                std::unordered_map<EG::Input::Mouse, bool, std::hash<unsigned int> > mouse_downs;
                std::unordered_map<EG::Input::Mouse, bool, std::hash<unsigned int> > mouse_toggles;
                std::unordered_map<EG::Input::Mouse, bool, std::hash<unsigned int> > mouse_presses;
                std::unordered_map<EG::Input::Mouse, bool, std::hash<unsigned int> > mouse_releases;
                // float mouse_wheel_delta;
                bool reset_mouse;

                //std::string character_buffer;
                std::vector<char> character_buffer;
        };
    }
}

#endif
