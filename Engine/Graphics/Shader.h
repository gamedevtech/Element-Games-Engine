#ifndef EG_GRAPHICS_SHADER_H
#define EG_GRAPHICS_SHADER_H

#include <string>
#include <vector>

namespace EG{
    namespace Graphics{
        class ShaderSource{
            public:
                ShaderSource(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path = "", std::string tessellation_control_shader_path = "", std::string tessellation_evaluation_shader_path = "");
                ~ShaderSource(void);

                char **vertex_shader_source;
                char **fragment_shader_source;
                char **geometry_shader_source;
                char **tessellation_control_shader_source;
                char **tessellation_evaluation_shader_source;
                int *vertex_shader_line_sizes;
                int *fragment_shader_line_sizes;
                int *geometry_shader_line_sizes;
                int *tessellation_control_shader_line_sizes;
                int *tessellation_evaluation_shader_line_sizes;
                int vertex_shader_line_count;
                int fragment_shader_line_count;
                int geometry_shader_line_count;
                int tessellation_control_shader_line_count;
                int tessellation_evaluation_shader_line_count;
            private:
                std::vector<std::string> LoadTextFile(std::string file_path);
        };
    }
}

#endif
