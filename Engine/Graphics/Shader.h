#ifndef EG_GRAPHICS_SHADER_H
#define EG_GRAPHICS_SHADER_H

#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h> 

namespace EG{
    namespace Graphics{
        class ShaderFiles{
            public:
                ShaderFiles(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path = "", std::string tessellation_control_shader_path = "", std::string tessellation_evaluation_shader_path = "") {
                    vertex = vertex_shader_path;
                    fragment = fragment_shader_path;
                    geometry = geometry_shader_path;
                    tessellation_control = tessellation_control_shader_path;
                    tessellation_evaluation = tessellation_evaluation_shader_path;
                    init = false;
                    vertex_changed = false;
                    fragment_changed = false;
                    geometry_changed = false;
                    tessellation_control_changed = false;
                    tessellation_evaluation_changed = false;
                    StatFiles();
                }

                void StatFiles(void) {
                    if (init) {
                        previous_vertex_stats = vertex_stats;
                        previous_fragment_stats = fragment_stats;
                        previous_geometry_stats = geometry_stats;
                        previous_tessellation_control_stats = tessellation_control_stats;
                        previous_tessellation_evaluation_stats = tessellation_evaluation_stats;
                    }
                    stat(vertex.c_str(), &vertex_stats);
                    stat(fragment.c_str(), &fragment_stats);
                    stat(geometry.c_str(), &geometry_stats);
                    stat(tessellation_control.c_str(), &tessellation_control_stats);
                    stat(tessellation_evaluation.c_str(), &tessellation_evaluation_stats);
                    if (!init) {
                        previous_vertex_stats = vertex_stats;
                        previous_fragment_stats = fragment_stats;
                        previous_geometry_stats = geometry_stats;
                        previous_tessellation_control_stats = tessellation_control_stats;
                        previous_tessellation_evaluation_stats = tessellation_evaluation_stats;
                        init = true;
                    }
                }

                void Update(void) {
                    vertex_changed = false;
                    fragment_changed = false;
                    geometry_changed = false;
                    tessellation_control_changed = false;
                    tessellation_evaluation_changed = false;
                    StatFiles();

                    if (previous_vertex_stats.st_mtime != vertex_stats.st_mtime) {
                        vertex_changed = true;
                    }
                    if (previous_fragment_stats.st_mtime != fragment_stats.st_mtime) {
                        fragment_changed = true;
                    }
                    if (previous_geometry_stats.st_mtime != geometry_stats.st_mtime) {
                        geometry_changed = true;
                    }
                    if (previous_tessellation_control_stats.st_mtime != tessellation_control_stats.st_mtime) {
                        tessellation_control_changed = true;
                    }
                    if (previous_tessellation_evaluation_stats.st_mtime != tessellation_evaluation_stats.st_mtime) {
                        tessellation_evaluation_changed = true;
                    }
                }

                bool init;

                std::string vertex;
                std::string fragment;
                std::string geometry;
                std::string tessellation_control;
                std::string tessellation_evaluation;

                struct stat previous_vertex_stats;
                struct stat previous_fragment_stats;
                struct stat previous_geometry_stats;
                struct stat previous_tessellation_control_stats;
                struct stat previous_tessellation_evaluation_stats;

                struct stat vertex_stats;
                struct stat fragment_stats;
                struct stat geometry_stats;
                struct stat tessellation_control_stats;
                struct stat tessellation_evaluation_stats;

                bool vertex_changed;
                bool fragment_changed;
                bool geometry_changed;
                bool tessellation_control_changed;
                bool tessellation_evaluation_changed;
        };

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
