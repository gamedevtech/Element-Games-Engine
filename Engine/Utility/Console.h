#ifndef EG_UTILITY_CONSOLE_H
#define EG_UTILITY_CONSOLE_H

#include <string>
#include <vector>
#include "Font.h"
#include "../Math/Math.h"
#include "../Graphics/ShaderManager.h"

namespace EG{
    namespace Utility{
        class ConsoleLine {
            public:
                ConsoleLine(std::string _str = "", glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float _fade_time = 1.0f, float _alive_time = 3.0f);
                ~ConsoleLine(void);

                void Update(float dt);
                bool IsAlive(void);
                std::string GetString(void);
                glm::vec4 GetColor(void);
            private:
                bool alive;
                std::string str;
                float alive_time, fade_time, time;
                glm::vec4 color;
        };

        class Console {
            public:
                Console(void);
                ~Console(void);

                void Update(float dt);
                void Draw(EG::Graphics::ShaderManager *shaders, glm::vec3 bottom_left_position);
                void Print(std::string str, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                void Print(float val, std::string prefix = "", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                void Print(glm::vec2 val, std::string prefix = "", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                void Print(glm::vec3 val, std::string prefix = "", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                void Print(glm::vec4 val, std::string prefix = "", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                void Print(glm::mat4 val, std::string prefix = "", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            private:
                std::vector<ConsoleLine *> lines;
                unsigned int max_lines;
                float fade_time;
        };
    }
}

#endif
