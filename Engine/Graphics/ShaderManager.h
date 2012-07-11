#ifndef EG_GRAPHICS_SHADER_MANAGER_H
#define EG_GRAPHICS_SHADER_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "../Utility/StringDictionary.h"
#include "../Utility/UnsignedIntDictionary.h"
#include "Shader.h"
#include "GraphicsSubsystem.h"

namespace EG{
    namespace Graphics{
        class ShaderManager{
            public:
                enum ShaderObjectIdIndices{
                    VERTEX_SHADER_OBJECT = 0,
                    FRAGMENT_SHADER_OBJECT = 1,
                    GEOMETRY_SHADER_OBJECT = 2,
                    SHADER_PROGRAM_OBJECT = 3,
                    TESSELLATION_CONTROL_SHADER_OBJECT = 4,
                    TESSELLATION_EVALUATION_SHADER_OBJECT = 5
                };

                enum EngineUniforms{
                    ENGINE_PERSPECTIVE_MATRIX = 0,
                    ENGINE_ORTHO_MATRIX = 1,
                    ENGINE_CAMERA_MATRIX = 2,
                    ENGINE_MODEL_MATRIX = 3,
                    ENGINE_NORMAL_MATRIX = 4,
                    ENGINE_CAMERA_POSITION = 4,
                    ENGINE_LIGHT_POSITION = 5,
                    ENGINE_LIGHT_RADIUS = 6,
                    ENGINE_LIGHT_ATTENUATION = 7,
                    ENGINE_DECAL_MAP = 8,
                    ENGINE_NORMAL_MAP = 9,
                    ENGINE_HEIGHT_MAP = 10,
                    ENGINE_NORMAL_MAPPING_ENABLED = 11,
                    ENGINE_MATERIAL_COLOR = 12,
                    ENGINE_MATERIAL_SPECULARITY = 13,
                    ENGINE_RECEIVES_LIGHTING = 14,
                    ENGINE_LIGHT_COLOR = 15,
                    ENGINE_SHADOW_MAPPING_ENABLED = 16,
                    ENGINE_SHADOW_MAPPING_BIAS = 17,
                    ENGINE_SHADOW_MAPPING_SIZE = 18,
                    ENGINE_POSITION_BUFFER = 19,
                    ENGINE_NORMAL_BUFFER = 20,
                    ENGINE_TRANSPARENT_BUFFER = 21,
                    ENGINE_LIGHT_BUFFER = 22,
                    ENGINE_COLOR_BUFFER = 23,
                    ENGINE_BLOOM_BUFFER = 24,
                    ENGINE_SSAO_BUFFER = 25,
                    ENGINE_LUMINANCE = 26,
                    ENGINE_BLOOM_SCALE = 27,
                    ENGINE_SSAO_ENABLED = 28,
                    ENGINE_BLOOM_ENABLED = 29,
                    ENGINE_LUMINANCE_SCALE = 30,
                    ENGINE_DEPTH_MAP = 31,
                    ENGINE_RESOLUTION = 32,
                    ENGINE_NEAR_FAR = 33,
                    ENGINE_SPECULAR_MAP = 34,
                    ENGINE_PROJECTION_MATRIX = 35,
                    ENGINE_VIEW_MATRIX = 36,
                    ENGINE_BONE_TRANSFORMS = 37,
                    ENGINE_HAS_ANIMATIONS = 38,
                    ENGINE_INVERSE_VIEW_MATRIX = 39
                };

                enum ShaderUniformTypes{
                    UNIFORM_TEXTURE = 0,
                    UNIFORM_CUBEMAP = 1,
                    UNIFORM_INT = 2,
                    UNIFORM_INT2 = 3,
                    UNIFORM_INT3 = 4,
                    UNIFORM_INT4 = 5,
                    UNIFORM_FLOAT = 6,
                    UNIFORM_FLOAT2 = 7,
                    UNIFORM_FLOAT3 = 8,
                    UNIFORM_FLOAT4 = 9,
                    UNIFORM_MAT2 = 10,
                    UNIFORM_MAT3 = 11,
                    UNIFORM_MAT4 = 12
                };

                ShaderManager(void);
                ~ShaderManager(void);

                bool Add(std::string shader_id, std::string vertex_path, std::string fragment_path, std::string geometry_path = "", std::string tessellation_control_path = "", std::string tessellation_evaluation_path = "", unsigned int fragment_outputs = 1);
                void ReleaseShader(unsigned int *program_object_id, unsigned int *vertex_object_id, unsigned int *fragment_object_id, unsigned int *geometry_object_id = NULL, unsigned int *tessellation_control_object_id = NULL, unsigned int *tessellation_evaluation_object_id = NULL);
                //void Bind(const char *shader_id);
                bool Has(std::string shader_id);
                void Bind(std::string shader_id);
                void Unbind(void);
                bool IsBound(void){
                    return shader_bound;
                }
                void AddEngineUniformUsage(std::string shader_id, EG::Graphics::ShaderManager::EngineUniforms variable_name);
                void AddObjectUniformUsage(std::string shader_id, std::string variable_name, EG::Graphics::ShaderManager::ShaderUniformTypes variable_type);
                std::vector<EG::Graphics::ShaderManager::EngineUniforms> *GetEngineUniforms(std::string shader_id);
                std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> > *GetObjectUniforms(std::string shader_id);

                unsigned int GetVariableLocation(std::string variable_name, bool uniform_or_attribute = true);
                void SetInt(const char *variable_name, int value0);
                void SetInt2(const char *variable_name, int value0, int value1);
                void SetInt3(const char *variable_name, int value0, int value1, int value2);
                void SetInt4(const char *variable_name, int value0, int value1, int value2, int value3);
                void SetIntV(const char *variable_name, int size, int *data);
                void SetFloat(const char *variable_name, float value0);
                void SetFloat2(const char *variable_name, float value0, float value1);
                void SetFloat3(const char *variable_name, float value0, float value1, float value2);
                void SetFloat4(const char *variable_name, float value0, float value1, float value2, float value3);
                void SetFloatV(const char *variable_name, int size, float *data);
                void SetMatrix2(const char *variable_name, float *data);
                void SetMatrix3(const char *variable_name, float *data);
                void SetMatrix4(const char *variable_name, float *data, unsigned int count = 1);
                void SetInt2(const char *variable_name, glm::ivec2 values);
                void SetInt3(const char *variable_name, glm::ivec3 values);
                void SetInt4(const char *variable_name, glm::ivec4 values);
                void SetFloat2(const char *variable_name, glm::vec2 values);
                void SetFloat3(const char *variable_name, glm::vec3 values);
                void SetFloat4(const char *variable_name, glm::vec4 values);
                void SetMatrix2(const char *variable_name, glm::mat2 matrix);
                void SetMatrix3(const char *variable_name, glm::mat3 matrix);
                void SetMatrix4(const char *variable_name, glm::mat4 matrix);
                void SetMatrix4(const char *variable_name, std::vector<glm::mat4> matrix);
            private:
                bool shader_bound;
                unsigned int current_program_object_id;
                EG::Utility::StringDictionary<unsigned int> program_objects;
                EG::Utility::UnsignedIntDictionary<unsigned int> vertex_shader_objects;
                EG::Utility::UnsignedIntDictionary<unsigned int> fragment_shader_objects;
                EG::Utility::UnsignedIntDictionary<unsigned int> geometry_shader_objects;
                EG::Utility::UnsignedIntDictionary<unsigned int> tessellation_control_shader_objects;
                EG::Utility::UnsignedIntDictionary<unsigned int> tessellation_evaluation_shader_objects;
                EG::Utility::UnsignedIntDictionary<EG::Utility::StringDictionary<int> *> variable_locations;
                EG::Utility::UnsignedIntDictionary<std::vector<EG::Graphics::ShaderManager::EngineUniforms> *> engine_shader_uniforms;
                EG::Utility::UnsignedIntDictionary<std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> > *> object_shader_uniforms;

                void PrintProgramLog(unsigned int program_object_id);
                void PrintShaderLog(unsigned int shader_object_id);
                std::ofstream shader_log;

                void StoreShaderUniforms(std::string shader_id, std::vector<std::pair<std::string, std::string> > params);
                std::vector<std::pair<std::string, std::string> > FindUniforms(char **source, int *sizes, int line_count);
                void InterpretShaderVariables(EG::Graphics::ShaderSource *shader_source, std::string shader_id);
                EG::Utility::StringDictionary<EngineUniforms> engine_uniform_string_translations;
                EG::Utility::StringDictionary<ShaderUniformTypes> uniform_type_translations;
                void CreateUniformStringTranslations(void);
        };
    }
}

#endif
