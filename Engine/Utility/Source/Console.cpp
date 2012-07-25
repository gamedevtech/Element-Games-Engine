#include "../Console.h"

#include <sstream>

namespace EG{
    namespace Utility{
        ConsoleLine::ConsoleLine(std::string _str, glm::vec4 _color, float _fade_time, float _alive_time) {
            str = _str;
            color = _color;
            fade_time = _fade_time;
            alive_time = _alive_time;
            alive = true;
            time = alive_time;
        }

        ConsoleLine::~ConsoleLine(void) {
            //
        }

        void ConsoleLine::Update(float dt) {
            time -= dt;
            if (time < 0.0f) {
                alive = false;
                color.a = 0.0f;
            } else {
                if (time < fade_time) {
                    color.a = time / fade_time;
                }
            }
        }

        bool ConsoleLine::IsAlive(void) {
            return alive;
        }

        std::string ConsoleLine::GetString(void) {
            return str;
        }

        glm::vec4 ConsoleLine::GetColor(void) {
            return color;
        }

        Console::Console(void) {
            //
        }

        Console::~Console(void) {
            //
        }

        void Console::Update(float dt) {
            std::vector<std::vector<ConsoleLine *>::iterator> to_remove;
            std::vector<ConsoleLine *>::iterator iter = lines.begin();
            while (iter != lines.end()) {
                ConsoleLine *line = (*iter);
                line->Update(dt);
                if (!(line->IsAlive())) {
                    to_remove.push_back(iter);
                }
                ++iter;
            }

            std::vector<std::vector<ConsoleLine *>::iterator>::iterator remiter = to_remove.begin();
            while (remiter != to_remove.end()) {
                lines.erase((*remiter));
                ++remiter;
            }
        }

        void Console::Draw(EG::Graphics::ShaderManager *shaders, glm::vec3 bottom_left_position) {
            glm::mat4 trans = glm::translate(bottom_left_position);
            float y = 20.0f * lines.size();
            std::vector<ConsoleLine *>::iterator iter = lines.begin();
            while (iter != lines.end()) {
                ConsoleLine *line = (*iter);

                glm::mat4 offset = glm::translate(trans, glm::vec3(0.0f, y, 0.0f));
                shaders->SetMatrix4("model_matrix", offset);
                shaders->SetFloat4("material_color", line->GetColor());
                font_manager->Draw(line->GetString());

                y -= 20.0f;
                ++iter;
            }
        }

        void Console::Print(std::string str, glm::vec4 color) {
            lines.push_back(new ConsoleLine(str, color));
        }

        void Console::Print(float val, std::string prefix, glm::vec4 color) {
            std::stringstream out;
            out << prefix << val;
            lines.push_back(new ConsoleLine(out.str(), color));
        }

        void Console::Print(glm::vec2 val, std::string prefix, glm::vec4 color) {
            std::stringstream out;
            out << prefix << val.x << ", " << val.y;
            lines.push_back(new ConsoleLine(out.str(), color));
        }

        void Console::Print(glm::vec3 val, std::string prefix, glm::vec4 color) {
            std::stringstream out;
            out << prefix << val.x << ", " << val.y << ", " << val.z;
            lines.push_back(new ConsoleLine(out.str(), color));
        }

        void Console::Print(glm::vec4 val, std::string prefix, glm::vec4 color) {
            std::stringstream out;
            out << prefix << val.x << ", " << val.y << ", " << val.z << ", " << val.w;
            lines.push_back(new ConsoleLine(out.str(), color));
        }

        void Console::Print(glm::mat4 val, std::string prefix, glm::vec4 color) {
            //
        }
    }
}
