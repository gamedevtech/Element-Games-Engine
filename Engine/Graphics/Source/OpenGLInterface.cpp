#include "../OpenGLInterface.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

namespace EG{
    namespace Graphics{
        OpenGLInterface::OpenGLInterface(void){
            //
        }
        OpenGLInterface::~OpenGLInterface(void){
            //
        }

        void OpenGLInterface::Initialize(unsigned int viewport_width, unsigned int viewport_height/*, float near, float far*/){
            glewInit();
            #ifndef WIN32
                int x = 0;
                char *y[1];
                glutInit(&x, y);
            #endif

            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glClearDepth(1.0f);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            /*glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60.0f, viewport_width / float(viewport_height), near, far);*/
            Resize(viewport_width, viewport_height);

            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glDisable(GL_BLEND);

            const GLubyte *gl_version_string = glGetString(GL_VERSION);
            version_string = ((const char *)(gl_version_string));

            unsigned int first_position = version_string.find_first_of(' ');
            std::string version_numbers_string = version_string.substr(0, first_position);
            version_major = atoi(version_numbers_string.substr(0, 1).c_str());
            version_minor = atoi(version_numbers_string.substr(2, 1).c_str());
            std::cout << "OpenGL Version: " << version_major << '.' << version_minor << std::endl;
            GLint n;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
            std::cout << "Max Vertex Attribs: " << n << std::endl;
            glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &n);
            std::cout << "Max Vertex Uniform Components: " << n << std::endl;
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &n);
            std::cout << "Max FBO Color Attachments: " << n << std::endl;
        }

        void OpenGLInterface::Resize(unsigned int viewport_width, unsigned int viewport_height){
            glViewport(0, 0, viewport_width, viewport_height);
        }

        void OpenGLInterface::Begin3D(/*unsigned int viewport_width, unsigned int viewport_height, float near, float far*/void){
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            //glEnable(GL_LIGHTING);
            //glEnable(GL_DEPTH_TEST);
            //glEnable(GL_CULL_FACE);
            //glEnable(GL_TEXTURE_2D);
            //glEnable(GL_NORMALIZE);
            glClearDepth(1.0f);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);

            /*glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60.0f, viewport_width / float(viewport_height), near, far);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();*/

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }

        void OpenGLInterface::End3D(void){
            glPopAttrib();
            ErrorCheck("Post Frame");
        }

        /*void OpenGLInterface::Begin2D(glm::mat4 orthographics_projection_matrix){
            //
        }

        void OpenGLInterface::End2D(void){
            //
        }*/

        void OpenGLInterface::PushMatrix(void){
            glPushMatrix();
        }

        void OpenGLInterface::PopMatrix(void){
            glPopMatrix();
        }

        std::string OpenGLInterface::GetVersionString(void){
            return version_string;
        }

        unsigned int OpenGLInterface::GetVersionMajor(void){
            return version_major;
        }

        unsigned int OpenGLInterface::GetVersionMinor(void){
            return version_minor;
        }

        void OpenGLInterface::OverrideVersion(int major, int minor){
            version_major = major;
            version_minor = minor;
        }

        bool OpenGLInterface::ErrorCheck(std::string prefix){
            GLenum result = glGetError();
            switch (result){
                case GL_INVALID_ENUM:{
                    std::cout << prefix.c_str() << ": GLERROR: Invalid enumeration parameter." << std::endl;
                }break;
                case GL_INVALID_VALUE:{
                    std::cout << prefix.c_str() << ": GLERROR: Invalid value parameter." << std::endl;
                }break;
                case GL_INVALID_OPERATION:{
                    std::cout << prefix.c_str() << ": GLERROR: Invalid method called." << std::endl;
                }break;
                case GL_STACK_OVERFLOW:{
                    std::cout << prefix.c_str() << ": GLERROR: Stack overflow." << std::endl;
                }break;
                case GL_STACK_UNDERFLOW:{
                    std::cout << prefix.c_str() << ": GLERROR: Stack underflow." << std::endl;
                }break;
                case GL_OUT_OF_MEMORY:{
                    std::cout << prefix.c_str() << ": GLERROR: Out of memory." << std::endl;
                }break;
                case GL_NO_ERROR:{
                    return false;
                }break;
                default:{
                    std::cout << prefix.c_str() << ": GLERROR: Unknown error." << std::endl;
                }break;
            }
            return true;
        }

        // Shaders
        void OpenGLInterface::ShaderCreateShaderObjectIds(unsigned int *object_ids, bool geometry, bool tessellation_control, bool tessellation_evaluation){
            object_ids[0] = glCreateShader(GL_VERTEX_SHADER);
            object_ids[1] = glCreateShader(GL_FRAGMENT_SHADER);
            if (version_major >= 3 && version_minor >= 2 && geometry){
                object_ids[2] = glCreateShader(GL_GEOMETRY_SHADER);
            }
            if (version_major >= 4 && version_minor >= 0){
                if (tessellation_control){
                    object_ids[4] = glCreateShader(GL_TESS_CONTROL_SHADER);
                }
                if (tessellation_evaluation){
                    object_ids[5] = glCreateShader(GL_TESS_EVALUATION_SHADER);
                }
            }
            ErrorCheck("After Shader Objects Created");
        }

        void OpenGLInterface::ShaderSetShaderSource(unsigned int *object_ids, char **vertex_source, int vertex_lines, int *vertex_line_sizes, char **fragment_source, int fragment_lines, int *fragment_line_sizes, bool geometry, char **geometry_source, int geometry_lines, int *geometry_line_sizes, bool tessellation_control, char **tessellation_control_source, int tessellation_control_lines, int *tessellation_control_line_sizes, bool tessellation_evaluation, char **tessellation_evaluation_source, int tessellation_evaluation_lines, int *tessellation_evaluation_line_sizes){
            const GLchar **gl_vertex_shader_source = (const GLchar **)(vertex_source);
            const GLchar **gl_fragment_shader_source = (const GLchar **)(fragment_source);
            glShaderSource(object_ids[0], vertex_lines, gl_vertex_shader_source, vertex_line_sizes);
            glShaderSource(object_ids[1], fragment_lines, gl_fragment_shader_source, fragment_line_sizes);
            if (version_major >= 3 && version_minor >= 2 && geometry){
                const GLchar **gl_geometry_shader_source = (const GLchar **)(geometry_source);
                glShaderSource(object_ids[2], geometry_lines, gl_geometry_shader_source, geometry_line_sizes);
            }
            if (version_major >= 4 && version_minor >= 0){
                if (tessellation_control){
                    const GLchar **gl_tessellation_control_shader_source = (const GLchar **)(tessellation_control_source);
                    glShaderSource(object_ids[4], tessellation_control_lines, gl_tessellation_control_shader_source, tessellation_control_line_sizes);
                }
                if (tessellation_evaluation){
                    const GLchar **gl_tessellation_evaluation_shader_source = (const GLchar **)(tessellation_evaluation_source);
                    glShaderSource(object_ids[5], tessellation_evaluation_lines, gl_tessellation_evaluation_shader_source, tessellation_evaluation_line_sizes);
                }
            }
            ErrorCheck("After Shader Source Loaded");
        }

        void OpenGLInterface::ShaderCompile(unsigned int *object_ids, bool geometry, bool tessellation_control, bool tessellation_evaluation, unsigned int fragment_outputs){
            glCompileShader(object_ids[0]);
            glCompileShader(object_ids[1]);
            if (version_major >= 3 && version_minor >= 2 && geometry){
                glCompileShader(object_ids[2]);
            }
            if (version_major >= 4 && version_minor >= 0){
                if (tessellation_control){
                    glCompileShader(object_ids[4]);
                }
                if (tessellation_evaluation){
                    glCompileShader(object_ids[5]);
                }
            }
            ErrorCheck("After Shaders Compiled");

            object_ids[3] = glCreateProgramObjectARB();

            // We attach each shader we just loaded to our program object
            glAttachShader(object_ids[3], object_ids[0]);
            glAttachShader(object_ids[3], object_ids[1]);
            if (version_major >= 3 && version_minor >= 2 && geometry){
                glAttachShader(object_ids[3], object_ids[2]);
            }
            if (version_major >= 4 && version_minor >= 0){
                if (tessellation_control){
                    glAttachShader(object_ids[3], object_ids[4]);
                }
                if (tessellation_evaluation){
                    glAttachShader(object_ids[3], object_ids[5]);
                }
            }
            ErrorCheck("After Shaders Attached");

            //// http://developer.download.nvidia.com/opengl/specs/GL_EXT_geometry_shader4.txt
            // TODO: Read up on this shit!
            if (version_major >= 3 && version_minor >= 2 && geometry){
                //geometry shader parameters, max can be kept 1024 and not be worried about at all
                glProgramParameteriEXT(object_ids[3], GL_GEOMETRY_VERTICES_OUT_EXT, 1024);
                glProgramParameteriEXT(object_ids[3], GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
                glProgramParameteriEXT(object_ids[3], GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);
            }

            glBindAttribLocation(object_ids[3], 0, "vertex_position");
            glBindAttribLocation(object_ids[3], 1, "vertex_normal");
            glBindAttribLocation(object_ids[3], 2, "vertex_texcoord");
            glBindAttribLocation(object_ids[3], 3, "vertex_binormal");
            glBindAttribLocation(object_ids[3], 4, "vertex_bitangent");
            glBindAttribLocation(object_ids[3], 5, "vertex_weights");
            glBindAttribLocation(object_ids[3], 6, "vertex_weight_indices");
            if (fragment_outputs == 1){
                glBindFragDataLocation(object_ids[3], 0, "fragment_output");
            }else{
                for (unsigned int i = 0; i < fragment_outputs; i++){
                    std::stringstream variable_name;
                    variable_name << "fragment" << i;
                    glBindFragDataLocation(object_ids[3], i, variable_name.str().c_str());
                }
            }

            // Our last init function is to link our program object with OpenGL
            glLinkProgram(object_ids[3]);
            ErrorCheck("After Shader Linked");
        }

        void OpenGLInterface::ShaderBind(unsigned int program_object_id){
            glUseProgram(program_object_id);
            ErrorCheck("After Shader Bound");
        }

        void OpenGLInterface::ShaderRelease(unsigned int *program_object_id, unsigned int *vertex_object_id, unsigned int *fragment_object_id, unsigned int *geometry_object_id, unsigned int *tessellation_control_object_id, unsigned int *tessellation_evaluation_object_id){
            // If our vertex shader pointer is valid, free it
            if (*vertex_object_id){
                glDetachShader(*program_object_id, *vertex_object_id);
                glDeleteShader(*vertex_object_id);
                *vertex_object_id = 0;
            }

            // If our fragment shader pointer is valid, free it
            if (*fragment_object_id){
                glDetachShader(*program_object_id, *fragment_object_id);
                glDeleteShader(*fragment_object_id);
                *fragment_object_id = 0;
            }

            // If out geometry shader pointer is valid, free it
            if (*geometry_object_id){
                glDetachShader(*program_object_id, *geometry_object_id);
                glDeleteShader(*geometry_object_id);
                *geometry_object_id = 0;
            }

            if (*tessellation_control_object_id){
                glDetachShader(*program_object_id, *tessellation_control_object_id);
                glDeleteShader(*tessellation_control_object_id);
            }

            if (*tessellation_evaluation_object_id){
                glDetachShader(*program_object_id, *tessellation_evaluation_object_id);
                glDeleteShader(*tessellation_evaluation_object_id);
            }

            // If our program object pointer is valid, free it
            if (*program_object_id){
                glDeleteShader(*program_object_id);
                *program_object_id = 0;
            }
        }

        std::string OpenGLInterface::ShaderProgramLog(unsigned int program_object_id){
            int info_log_length = 0;
            int characters_written  = 0;
            char *info_log;
            std::string out;
            glGetProgramiv(program_object_id, GL_INFO_LOG_LENGTH, &info_log_length);

            info_log = (char *)malloc(info_log_length);
            if (version_major >= 2 && version_minor >= 0){
                glGetProgramInfoLog(program_object_id, info_log_length, &characters_written, info_log);
            }else{
                glGetInfoLogARB(program_object_id, info_log_length, &characters_written, info_log);
            }
            out = std::string(info_log);
            free(info_log);
            return out;
        }

        std::string OpenGLInterface::ShaderShaderLog(unsigned int shader_object_id){
            int info_log_length = 0;
            int characters_written  = 0;
            char *info_log;
            std::string out;
            glGetShaderiv(shader_object_id, GL_INFO_LOG_LENGTH, &info_log_length);

            info_log = (char *)malloc(info_log_length);
            if (version_major >= 2 && version_minor >= 0){
                glGetShaderInfoLog(shader_object_id, info_log_length, &characters_written, info_log);
            }else{
                glGetInfoLogARB(shader_object_id, info_log_length, &characters_written, info_log);
            }
            out = std::string(info_log);
            free(info_log);
            return out;
        }

        int OpenGLInterface::ShaderGetUniformLocation(unsigned int program_object_id, const char *variable_name){
            return glGetUniformLocation(program_object_id, variable_name);
        }

        int OpenGLInterface::ShaderGetAttributeLocation(unsigned int program_object_id, const char *variable_name){
            return glGetAttribLocation(program_object_id, variable_name);
        }

        void OpenGLInterface::ShaderSetInt(unsigned int variable_location, int value0){
            glUniform1i(variable_location, value0);
        }

        void OpenGLInterface::ShaderSetInt2(unsigned int variable_location, int value0, int value1){
            glUniform2i(variable_location, value0, value1);
        }

        void OpenGLInterface::ShaderSetInt3(unsigned int variable_location, int value0, int value1, int value2){
            glUniform3i(variable_location, value0, value1, value2);
        }

        void OpenGLInterface::ShaderSetInt4(unsigned int variable_location, int value0, int value1, int value2, int value3){
            glUniform4i(variable_location, value0, value1, value2, value3);
        }

        void OpenGLInterface::ShaderSetIntV(unsigned int variable_location, int size, int *data){
            glUniform1iv(variable_location, size, data);
        }

        void OpenGLInterface::ShaderSetFloat(unsigned int variable_location, float value0){
            glUniform1f(variable_location, value0);
        }

        void OpenGLInterface::ShaderSetFloat2(unsigned int variable_location, float value0, float value1){
            glUniform2f(variable_location, value0, value1);
        }

        void OpenGLInterface::ShaderSetFloat3(unsigned int variable_location, float value0, float value1, float value2){
            glUniform3f(variable_location, value0, value1, value2);
        }

        void OpenGLInterface::ShaderSetFloat4(unsigned int variable_location, float value0, float value1, float value2, float value3){
            glUniform4f(variable_location, value0, value1, value2, value3);
        }

        void OpenGLInterface::ShaderSetFloatV(unsigned int variable_location, int size, float *data){
            glUniform1fv(variable_location, size, data);
        }

        void OpenGLInterface::ShaderSetMatrix2(unsigned int variable_location, float *data){
            glUniformMatrix2fv(variable_location, 1, GL_FALSE, data);
        }

        void OpenGLInterface::ShaderSetMatrix3(unsigned int variable_location, float *data){
            glUniformMatrix3fv(variable_location, 1, GL_FALSE, data);
        }

        void OpenGLInterface::ShaderSetMatrix4(unsigned int variable_location, float *data, unsigned int count){
            glUniformMatrix4fv(variable_location, count, GL_FALSE, data);
        }

        void OpenGLInterface::GenerateMeshBuffer(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool has_vertices, float *vertices, bool has_texcoords, float *texcoords, bool has_normals, float *normals, bool has_binormals, float *binormals, bool has_bitangents, float *bitangents, bool has_skeleton, float *weight_vertex_indices, float *weights){
            unsigned int count = vertex_count * 4 * sizeof(float);

            glGenVertexArrays(1, vertex_array_object_id);
            glBindVertexArray(*vertex_array_object_id);

            // NOTE: If any are missing in sequence use
            // glVertexAttrib3f(GLuint)<vertex_attribute_index>, value0, value1, value2);
            // <vertex_attribute_index> is the index of the attribute in order of attributes
            // 		So if texcoords are missing, it would be 2

            // Vertices
            if (has_vertices){
                glGenBuffers(1, &vertex_buffer_object_ids[VBO_VERTICES]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_VERTICES]);
                glBufferData(GL_ARRAY_BUFFER, count, vertices, GL_STATIC_DRAW);
                glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(0);
            }

            // Texcoords
            if (has_texcoords){
                glGenBuffers(1, &vertex_buffer_object_ids[VBO_TEXCOORDS]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_TEXCOORDS]);
                glBufferData(GL_ARRAY_BUFFER, count, texcoords, GL_STATIC_DRAW);
                glVertexAttribPointer((GLuint)2, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(2);
            }

            // Normals
            if (has_normals){
                glGenBuffers(1, &vertex_buffer_object_ids[VBO_NORMALS]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_NORMALS]);
                glBufferData(GL_ARRAY_BUFFER, count, normals, GL_STATIC_DRAW);
                glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(1);
            }

            // Binormals
            if (has_binormals){
                glGenBuffers(1, &vertex_buffer_object_ids[VBO_BINORMALS]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_BINORMALS]);
                glBufferData(GL_ARRAY_BUFFER, count, binormals, GL_STATIC_DRAW);
                glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(3);
            }

            // Bitangents
            if (has_bitangents){
                glGenBuffers(1, &vertex_buffer_object_ids[VBO_BITANGENTS]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_BITANGENTS]);
                glBufferData(GL_ARRAY_BUFFER, count, bitangents, GL_STATIC_DRAW);
                glVertexAttribPointer((GLuint)4, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(4);
            }

            if (has_skeleton){
                glGenBuffers(1, &vertex_buffer_object_ids[VBO_WEIGHTS]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_WEIGHTS]);
                glBufferData(GL_ARRAY_BUFFER, count, weights, GL_STATIC_DRAW);
                glVertexAttribPointer((GLuint)5, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(5);

                glGenBuffers(1, &vertex_buffer_object_ids[VBO_WEIGHT_VERTEX_INDICES]);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_ids[VBO_WEIGHT_VERTEX_INDICES]);
                glBufferData(GL_ARRAY_BUFFER, count, weight_vertex_indices, GL_STATIC_DRAW);
                //glVertexAttribPointer((GLuint)6, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
                glVertexAttribPointer((GLuint)6, 4, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(6);
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            ErrorCheck("After >=GL3 Mesh Creation");
        }

        void OpenGLInterface::DrawMesh(unsigned int *vertex_array_object_id, unsigned int vertex_count, bool using_tessellation){
            glBindVertexArray(*vertex_array_object_id);
            // NOTE: Can I just always pass defaults like this, and the arrays will override?
            // It works, although, it is not a priority for card manufacturers to optimize, try removing to see a performance gain
            // TODO: Add another Render function which takes in specific subsets of info, like, just having texcoords, and verts.
            //glVertexAttrib4f((GLuint)0, 0.0f, 0.0f, 0.0f, 1.0f); // default vertices
            //glVertexAttrib4f((GLuint)1, 0.0f, 0.0f, 0.0f, 1.0f); // default texcoords
            //glVertexAttrib4f((GLuint)2, 0.0f, 0.0f, 0.0f, 1.0f); // default normals
            //glVertexAttrib4f((GLuint)3, 0.0f, 0.0f, 0.0f, 1.0f); // default binormals
            //glVertexAttrib4f((GLuint)4, 0.0f, 0.0f, 0.0f, 1.0f); // default bitangents
            if (using_tessellation){
                glDrawArrays(GL_PATCHES, 0, vertex_count);
            }else{
                glDrawArrays(GL_TRIANGLES, 0, vertex_count);
            }
            ErrorCheck("Post Drawing Mesh");
            glBindVertexArray(0);
        }

        bool OpenGLInterface::BasicShadersSupported(void){
            if (GLEW_ARB_vertex_program && GLEW_ARB_fragment_program && GLEW_ARB_shader_objects){
                return true;
            }
            return false;
        }

        void OpenGLInterface::SetActiveTexture(unsigned int texture_index){
            glActiveTextureARB(GL_TEXTURE0 + texture_index);
        }

        unsigned int OpenGLInterface::CreateTexture(unsigned char *pixels, int width, int height, bool filtering, float anisotropy){
            unsigned int id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            if (filtering){
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
            }else{
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }

            if (!filtering){
                glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            }else{
                gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            }

            return id;
        }

        unsigned int OpenGLInterface::CreateCubeMap(unsigned char **pixels, int width, int height, bool filtering, float anisotropy){
            // TODO: Check for non power of 2, if true, then make sure major version >= 2
            glEnable(GL_TEXTURE_CUBE_MAP_ARB);
            unsigned int id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, id); // NOTE: Might get away with GL_TEXTURE_CUBE_MAP
            if (filtering){
                glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
                glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
            }else{
                glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }

            // NOTE: Do I need these?
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            if (!filtering){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[3]);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[2]);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[4]);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[5]);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[1]);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[0]);
            }else{
                gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels[3]);
                gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels[2]);
                gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels[4]);
                gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels[5]);
                gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels[1]);
                gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels[0]);
            }

            glDisable(GL_TEXTURE_CUBE_MAP_ARB);
            return id;
        }

        void OpenGLInterface::BindTexture(unsigned int texture_id, unsigned int texture_index){
            SetActiveTexture(texture_index);
            glBindTexture(GL_TEXTURE_2D, texture_id);
        }

        void OpenGLInterface::BindCubeMap(unsigned int texture_id, unsigned int texture_index){
            SetActiveTexture(texture_index);
            glEnable(GL_TEXTURE_CUBE_MAP_ARB);
            glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, texture_id); // NOTE: Might get away with GL_TEXTURE_CUBE_MAP
        }

        void OpenGLInterface::CreateOffscreenBuffer(unsigned int *buffer_id, unsigned int *textures, unsigned int *depth_texture, unsigned int width, unsigned int height, unsigned int color_buffer_count, bool use_depth, unsigned int texture_filtering){
            if (!GLEW_EXT_framebuffer_object){
                std::cout << "FBO Unsupported!" << std::endl;
            }else{
                if (color_buffer_count < 0){
                    color_buffer_count = 0;
                }

                ErrorCheck("Before Gen FBO");
                glGenFramebuffersEXT(1, buffer_id);
                ErrorCheck("After Gen FBO");

                ErrorCheck("Before FBO Texture Creation.");
                glGenTextures(color_buffer_count, &textures[0]); // create (reference to) a new texture
                ErrorCheck("After FBO Texture Creation.");

                for(unsigned int i = 0; i < color_buffer_count; i++){
                    glBindTexture(GL_TEXTURE_2D, textures[i]);
                    switch (texture_filtering){
                        case 0:{
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        }break;
                        case 1:{
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        }break;
                    }
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, 0);
                }

                // generate the depth textures
                if (use_depth){
                    glGenTextures (1, &depth_texture[0]);
                    glBindTexture (GL_TEXTURE_2D, depth_texture[0]);
                    glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                    switch (texture_filtering){
                        case 0:{
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        }break;
                        case 1:{
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        }break;
                    }
                }

                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *buffer_id);
                for (unsigned int i = 0; i < color_buffer_count; i++){
                    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, textures[i], 0);
                }
                if (use_depth){
                    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth_texture[0], 0);
                }
                glDrawBuffer(GL_FALSE);
                glReadBuffer(GL_FALSE);

                // verify all is well and restore state
                GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
                switch (status){
                    case GL_FRAMEBUFFER_COMPLETE_EXT:
                        break;
                    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
                        std::cout << "FBO configuration unsupported (" << status << ")" << std::endl;
                        break;
                    default:
                        std::cout << "FBO programmer error" << std::endl;
                }
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            }
        }

        void OpenGLInterface::BindOffscreenBuffer(unsigned int *buffer_id){
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *buffer_id);
        }

        void OpenGLInterface::StoreCurrentDrawBuffer(void){
            GLint temp_draw_buffer;
            glGetIntegerv(GL_DRAW_BUFFER, &temp_draw_buffer);
            draw_buffer = temp_draw_buffer;
        }

        void OpenGLInterface::UnbindOffscreenBuffer(void){
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        }

        void OpenGLInterface::RestoreDrawBuffer(void){
            GLint temp_draw_buffer = draw_buffer;
            glDrawBuffer(temp_draw_buffer);
            glReadBuffer(temp_draw_buffer);
        }

        void OpenGLInterface::StartOffscreenRender(unsigned int *buffer_id, unsigned int buffer_index, unsigned int width, unsigned int height){
            StoreCurrentDrawBuffer();
            BindOffscreenBuffer(buffer_id);
            ErrorCheck("Offscreen Bind");
            glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ErrorCheck("Offscreen start viewport and colorbuff bit");
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            switch (buffer_index){
                case 0:{
                    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
                    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
                }break;
                case 1:{
                    glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
                    glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);
                }break;
                case 2:{
                    glDrawBuffer(GL_COLOR_ATTACHMENT2_EXT);
                    glReadBuffer(GL_COLOR_ATTACHMENT2_EXT);
                }break;
                case 3:{
                    glDrawBuffer(GL_COLOR_ATTACHMENT3_EXT);
                    glReadBuffer(GL_COLOR_ATTACHMENT3_EXT);
                }break;
            }
            ErrorCheck("Offscreen Start Draw/Read Buffer");
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glViewport(0, 0, width, height);
        }

        void OpenGLInterface::EndOffscreenRender(void){
            glPopAttrib();
            UnbindOffscreenBuffer();
            RestoreDrawBuffer();
        }

        void OpenGLInterface::StartMultiBufferOffscreenRender(unsigned int *buffer_id, unsigned int buffer_count, int *buffers, unsigned int width, unsigned int height){
            StoreCurrentDrawBuffer();
            BindOffscreenBuffer(buffer_id);
            ErrorCheck("Offscreen mrt start fbo bind");
            glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
            ErrorCheck("Offscreen mrt start viewport and colorbuff bit");
            //glDrawBuffer(GL_NONE);
            //glReadBuffer(GL_NONE);
            GLenum *gl_buffers = new GLenum[buffer_count];
            for (unsigned int i = 0; i < buffer_count; i++){
                switch (buffers[i]){
                case 0:{
                        gl_buffers[i] = GL_COLOR_ATTACHMENT0_EXT;
                    }break;
                case 1:{
                        gl_buffers[i] = GL_COLOR_ATTACHMENT1_EXT;
                    }break;
                case 2:{
                        gl_buffers[i] = GL_COLOR_ATTACHMENT2_EXT;
                    }break;
                case 3:{
                        gl_buffers[i] = GL_COLOR_ATTACHMENT3_EXT;
                    }break;
                }
            }
            glDrawBuffers(buffer_count, gl_buffers);
            ErrorCheck("Offscreen mrt start DrawBuffers0");
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            ErrorCheck("Offscreen mrt start Clear draw buffer");
            glViewport(0, 0, width, height);
            ErrorCheck("Offscreen mrt start viewport");
        }

        void OpenGLInterface::EndMultiBufferOffscreenRender(void){
            glPopAttrib();
            UnbindOffscreenBuffer();
            RestoreDrawBuffer();
        }

        void OpenGLInterface::GetTextureDataFloats(float *texture_data, unsigned int texture_index){
            SetActiveTexture(texture_index);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, texture_data);
        }

        enum BlendingMode{
            BLEND_OFF = 0,
            BLEND_ALPHA = 1,
            BLEND_COLOR = 2,
            BLEND_ADDITIVE = 3,
            BLEND_ALPHA_PARTICLE = 4,
            BLEND_COLOR_PARTICLE = 5,
            BLEND_ONE_ONE_MINUS_SRC_ALPHA = 6
        };
        void OpenGLInterface::SetBlendingMode(unsigned int blending_mode){
            if (blending_mode == 1){
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }else if (blending_mode == 2){
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
            }else if (blending_mode == 3){
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
            }else if (blending_mode == 4){
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }else if (blending_mode == 5){
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_COLOR, GL_ONE);
            }else if (blending_mode == 6){
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            }else{
                glDisable(GL_BLEND);
            }
        }
    }
}
