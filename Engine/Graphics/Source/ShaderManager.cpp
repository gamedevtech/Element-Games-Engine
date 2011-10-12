#include "../ShaderManager.h"

#include <cstdlib>

namespace EG{
	namespace Graphics{
		ShaderManager::ShaderManager(void){
			current_program_object_id = 0;
			shader_bound = false;
			shader_log.open("shaders.log", std::fstream::out);
			shader_log.close();
			shader_log.open("shaders.log", std::fstream::out | std::fstream::app);
		}
		ShaderManager::~ShaderManager(void){
			//std::map<std::string, unsigned int>::iterator shader_iterator = program_objects.begin();
			EG::Utility::StringDictionaryKeysIterator shader_iterator = program_objects.GetKeysBegin();
			//while (shader_iterator != program_objects.end()){
			while (shader_iterator != program_objects.GetKeysEnd()){
				unsigned int *ids = new unsigned int[6];
				ids[0] = program_objects.Get(*shader_iterator);//(*shader_iterator).second;
				if (vertex_shader_objects.Has(ids[0])){
					ids[1] = vertex_shader_objects.Get(ids[0]);
				}else{
					ids[1] = 0;
				}
				if (fragment_shader_objects.Has(ids[0])){
					ids[2] = fragment_shader_objects.Get(ids[0]);
				}else{
					ids[2] = 0;
				}
				if (geometry_shader_objects.Has(ids[0])){
					ids[3] = geometry_shader_objects.Get(ids[0]);
				}else{
					ids[3] = 0;
				}
				if (tessellation_control_shader_objects.Has(ids[0])){
					ids[4] = tessellation_control_shader_objects.Get(ids[0]);
				}else{
					ids[4] = 0;
				}
				if (tessellation_evaluation_shader_objects.Has(ids[0])){
					ids[5] = tessellation_evaluation_shader_objects.Get(ids[0]);
				}else{
					ids[5] = 0;
				}
				ReleaseShader(&ids[0], &ids[1], &ids[2], &ids[3], &ids[4], &ids[5]);
				delete []ids;
				++shader_iterator;
			}
			program_objects.Clear();
			vertex_shader_objects.Clear();
			fragment_shader_objects.Clear();
			geometry_shader_objects.Clear();
			tessellation_control_shader_objects.Clear();
			tessellation_evaluation_shader_objects.Clear();
			variable_locations.Clear();
			shader_log.close();
		}

		bool ShaderManager::Add(std::string shader_id, std::string vertex_path, std::string fragment_path, std::string geometry_path, std::string tessellation_control_path, std::string tessellation_evaluation_path, unsigned int fragment_outputs){
			//std::cout << "Loading Shader: " << shader_id << std::endl;
			ShaderSource *sources = new ShaderSource(vertex_path, fragment_path, geometry_path, tessellation_control_path, tessellation_evaluation_path);

			unsigned int *object_ids = new unsigned int[6];
			object_ids[SHADER_PROGRAM_OBJECT] = 0;
			object_ids[VERTEX_SHADER_OBJECT] = 0;
			object_ids[FRAGMENT_SHADER_OBJECT] = 0;
			object_ids[GEOMETRY_SHADER_OBJECT] = 0;
			object_ids[TESSELLATION_CONTROL_SHADER_OBJECT] = 0;
			object_ids[TESSELLATION_EVALUATION_SHADER_OBJECT] = 0;

			bool geometry_flag = (sources->geometry_shader_line_count > 0) ? true : false;
			bool tessellation_control_flag = (sources->tessellation_control_shader_line_count > 0) ? true : false;
			bool tessellation_evaluation_flag = (sources->tessellation_evaluation_shader_line_count > 0) ? true : false;
			graphics->ShaderCreateShaderObjectIds(object_ids, geometry_flag, tessellation_control_flag, tessellation_evaluation_flag);
			graphics->ShaderSetShaderSource(object_ids, sources->vertex_shader_source, sources->vertex_shader_line_count, sources->vertex_shader_line_sizes, sources->fragment_shader_source, sources->fragment_shader_line_count, sources->fragment_shader_line_sizes, geometry_flag, sources->geometry_shader_source, sources->geometry_shader_line_count, sources->geometry_shader_line_sizes, tessellation_control_flag, sources->tessellation_control_shader_source, sources->tessellation_control_shader_line_count, sources->tessellation_control_shader_line_sizes, tessellation_evaluation_flag, sources->tessellation_evaluation_shader_source, sources->tessellation_evaluation_shader_line_count, sources->tessellation_evaluation_shader_line_sizes);
			graphics->ShaderCompile(object_ids, geometry_flag, tessellation_control_flag, tessellation_evaluation_flag, fragment_outputs);
			graphics->ShaderBind(object_ids[SHADER_PROGRAM_OBJECT]);
			shader_log << "-----------------------------------------------------------------------" << std::endl;
			shader_log << "Compilation of Shader: " << shader_id << std::endl;
			shader_log << "Prog: " << std::endl;
			PrintProgramLog(object_ids[SHADER_PROGRAM_OBJECT]);
			shader_log << "Frag: " << std::endl;
			PrintShaderLog(object_ids[FRAGMENT_SHADER_OBJECT]);
			shader_log << "Vert: " << std::endl;
			PrintShaderLog(object_ids[VERTEX_SHADER_OBJECT]);
			shader_log << "-----------------------------------------------------------------------" << std::endl << std::endl;
			graphics->CheckErrors("After Shader Log Printed");
			graphics->ShaderBind(0);

			//program_objects[shader_id] = object_ids[SHADER_PROGRAM_OBJECT];
			program_objects.Set(shader_id, object_ids[SHADER_PROGRAM_OBJECT]);
			vertex_shader_objects.Set(SHADER_PROGRAM_OBJECT, object_ids[VERTEX_SHADER_OBJECT]);
			fragment_shader_objects.Set(SHADER_PROGRAM_OBJECT, object_ids[FRAGMENT_SHADER_OBJECT]);
			if (object_ids[GEOMETRY_SHADER_OBJECT]){
				geometry_shader_objects.Set(SHADER_PROGRAM_OBJECT, object_ids[GEOMETRY_SHADER_OBJECT]);
			}
			if (object_ids[TESSELLATION_CONTROL_SHADER_OBJECT]){
				tessellation_control_shader_objects.Set(SHADER_PROGRAM_OBJECT, object_ids[TESSELLATION_CONTROL_SHADER_OBJECT]);
			}
			if (object_ids[TESSELLATION_EVALUATION_SHADER_OBJECT]){
				tessellation_evaluation_shader_objects.Set(SHADER_PROGRAM_OBJECT, object_ids[TESSELLATION_EVALUATION_SHADER_OBJECT]);
			}

			//delete sources; // Crashes when switching Rendering Pipelines

			return true;
		}

		void ShaderManager::ReleaseShader(unsigned int *program_object_id, unsigned int *vertex_object_id, unsigned int *fragment_object_id, unsigned int *geometry_object_id, unsigned int *tessellation_control_object_id, unsigned int *tessellation_evaluation_object_id){
			graphics->ShaderRelease(program_object_id, vertex_object_id, fragment_object_id, geometry_object_id, tessellation_control_object_id, tessellation_evaluation_object_id);
		}

		/*void ShaderManager::Bind(const char *shader_id){
			if (program_objects.count(shader_id) > 0){
				current_program_object_id = program_objects[shader_id];
				graphics->ShaderBind(current_program_object_id);
				shader_bound = true;
			}else{
				std::cout << "Shader " + std::string(shader_id) + " Failed To Bind, Unbinding All Shaders!" << std::endl;
				Unbind();
			}
		}*/

		void ShaderManager::Bind(std::string shader_id){
			unsigned int temp_program_object_id = program_objects.Get(shader_id);
			if (temp_program_object_id != 0){
				current_program_object_id = temp_program_object_id;
				graphics->ShaderBind(current_program_object_id);
				shader_bound = true;
			}else{
				std::cout << "Shader " + shader_id + " Failed To Bind, Unbinding All Shaders!" << std::endl;
				Unbind();
			}
		}

		void ShaderManager::Unbind(void){
			graphics->ShaderBind(0);
			shader_bound = false;
		}

		void ShaderManager::PrintProgramLog(unsigned int program_object_id){
			std::string log_output = graphics->ShaderProgramLog(program_object_id);
			shader_log << log_output << std::endl;
		}

		void ShaderManager::PrintShaderLog(unsigned int shader_object_id){
			std::string log_output = graphics->ShaderShaderLog(shader_object_id);
			shader_log << log_output << std::endl;
		}

		unsigned int ShaderManager::GetVariableLocation(std::string variable_name, bool uniform_or_attribute){
			if (shader_bound){
				bool create = false;
				if (!(variable_locations.Has(current_program_object_id))){
					EG::Utility::StringDictionary<int> *string_dict = new EG::Utility::StringDictionary<int>;
					variable_locations.Set(current_program_object_id, string_dict);
					create = true;
				}
				if (!create && (!(variable_locations.Get(current_program_object_id)->Has(variable_name)))){
					create = true;
				}
				if (create){
					int variable_location_id = -1;
					if (uniform_or_attribute){
						variable_location_id = graphics->ShaderGetUniformLocation(current_program_object_id, variable_name.c_str());
					}else{
						variable_location_id = graphics->ShaderGetAttributeLocation(current_program_object_id, variable_name.c_str());
					}
					variable_locations.Get(current_program_object_id)->Set(variable_name, variable_location_id);
					return variable_locations.Get(current_program_object_id)->Get(variable_name);
				}else{
					return variable_locations.Get(current_program_object_id)->Get(variable_name);
				}
			}
			return 0;
		}

		void ShaderManager::SetInt(const char *variable_name, int value0){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetInt(variable_location, value0);
			}
		}

		void ShaderManager::SetInt2(const char *variable_name, int value0, int value1){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetInt2(variable_location, value0, value1);
			}
		}

		void ShaderManager::SetInt3(const char *variable_name, int value0, int value1, int value2){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetInt3(variable_location, value0, value1, value2);
			}
		}

		void ShaderManager::SetInt4(const char *variable_name, int value0, int value1, int value2, int value3){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetInt4(variable_location, value0, value1, value2, value3);
			}
		}

		void ShaderManager::SetIntV(const char *variable_name, int size, int *data){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetIntV(variable_location, size, data);
			}
		}

		void ShaderManager::SetFloat(const char *variable_name, float value0){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetFloat(variable_location, value0);
			}
		}

		void ShaderManager::SetFloat2(const char *variable_name, float value0, float value1){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetFloat2(variable_location, value0, value1);
			}
		}

		void ShaderManager::SetFloat3(const char *variable_name, float value0, float value1, float value2){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetFloat3(variable_location, value0, value1, value2);
			}
		}

		void ShaderManager::SetFloat4(const char *variable_name, float value0, float value1, float value2, float value3){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetFloat4(variable_location, value0, value1, value2, value3);
			}
		}

		void ShaderManager::SetFloatV(const char *variable_name, int size, float *data){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetFloatV(variable_location, size, data);
			}
		}

		void ShaderManager::SetMatrix2(const char *variable_name, float *data){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetMatrix2(variable_location, data);
			}
		}

		void ShaderManager::SetMatrix3(const char *variable_name, float *data){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetMatrix3(variable_location, data);
			}
		}

		void ShaderManager::SetMatrix4(const char *variable_name, float *data){
			if (shader_bound){
				unsigned int variable_location = GetVariableLocation(variable_name);
				graphics->ShaderSetMatrix4(variable_location, data);
			}
		}

		void ShaderManager::SetInt2(const char *variable_name, glm::ivec2 values){
			SetInt2(variable_name, values.x, values.y);
		}

		void ShaderManager::SetInt3(const char *variable_name, glm::ivec3 values){
			SetInt3(variable_name, values.x, values.y, values.z);
		}

		void ShaderManager::SetInt4(const char *variable_name, glm::ivec4 values){
			SetInt4(variable_name, values.x, values.y, values.z, values.w);
		}

		void ShaderManager::SetFloat2(const char *variable_name, glm::vec2 values){
			SetFloat2(variable_name, values.x, values.y);
		}

		void ShaderManager::SetFloat3(const char *variable_name, glm::vec3 values){
			SetFloat3(variable_name, values.x, values.y, values.z);
		}

		void ShaderManager::SetFloat4(const char *variable_name, glm::vec4 values){
			SetFloat4(variable_name, values.x, values.y, values.z, values.w);
		}

		void ShaderManager::SetMatrix2(const char *variable_name, glm::mat2 matrix){
			SetMatrix2(variable_name, (float *)(glm::value_ptr(matrix)));
		}

		void ShaderManager::SetMatrix3(const char *variable_name, glm::mat3 matrix){
			SetMatrix3(variable_name, (float *)(glm::value_ptr(matrix)));
		}

		void ShaderManager::SetMatrix4(const char *variable_name, glm::mat4 matrix){
			SetMatrix4(variable_name, (float *)(glm::value_ptr(matrix)));
		}

	}
}
