#ifndef EG_GRAPHICS_SHADER_MANAGER_H
#define EG_GRAPHICS_SHADER_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>

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

				ShaderManager(void);
				~ShaderManager(void);

				bool Add(std::string shader_id, std::string vertex_path, std::string fragment_path, std::string geometry_path = "", std::string tessellation_control_path = "", std::string tessellation_evaluation_path = "", unsigned int fragment_outputs = 1);
				void ReleaseShader(unsigned int *program_object_id, unsigned int *vertex_object_id, unsigned int *fragment_object_id, unsigned int *geometry_object_id = NULL, unsigned int *tessellation_control_object_id = NULL, unsigned int *tessellation_evaluation_object_id = NULL);
				//void Bind(const char *shader_id);
				void Bind(std::string shader_id);
				void Unbind(void);
				bool IsBound(void){
					return shader_bound;
				}
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
				void SetMatrix4(const char *variable_name, float *data);
				void SetInt2(const char *variable_name, glm::ivec2 values);
				void SetInt3(const char *variable_name, glm::ivec3 values);
				void SetInt4(const char *variable_name, glm::ivec4 values);
				void SetFloat2(const char *variable_name, glm::vec2 values);
				void SetFloat3(const char *variable_name, glm::vec3 values);
				void SetFloat4(const char *variable_name, glm::vec4 values);
				void SetMatrix2(const char *variable_name, glm::mat2 matrix);
				void SetMatrix3(const char *variable_name, glm::mat3 matrix);
				void SetMatrix4(const char *variable_name, glm::mat4 matrix);
			private:
				bool shader_bound;
				unsigned int current_program_object_id;
				std::map<std::string, unsigned int> program_objects;
				std::map<unsigned int, unsigned int> vertex_shader_objects;
				std::map<unsigned int, unsigned int> fragment_shader_objects;
				std::map<unsigned int, unsigned int> geometry_shader_objects;
				std::map<unsigned int, unsigned int> tessellation_control_shader_objects;
				std::map<unsigned int, unsigned int> tessellation_evaluation_shader_objects;
				std::map<unsigned int, std::map<std::string, int> > variable_locations;

				void PrintProgramLog(unsigned int program_object_id);
				void PrintShaderLog(unsigned int shader_object_id);
				std::ofstream shader_log;
		};
	}
}

#endif
