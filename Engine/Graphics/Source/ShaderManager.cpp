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
            CreateUniformStringTranslations();
        }
        ShaderManager::~ShaderManager(void){
            EG::Utility::StringDictionaryKeysIterator shader_iterator = program_objects.GetKeysBegin();
            while (shader_iterator != program_objects.GetKeysEnd()){
                unsigned int *ids = new unsigned int[6];
                ids[0] = program_objects.Get(*shader_iterator);
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

            InterpretShaderVariables(sources, shader_id);

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

        void ShaderManager::AddEngineUniformUsage(std::string shader_id, EG::Graphics::ShaderManager::EngineUniforms variable_name){
            if (program_objects.Has(shader_id)){
                unsigned int id = program_objects.Get(shader_id);
                if (!(engine_shader_uniforms.Has(id))){
                    std::vector<EG::Graphics::ShaderManager::EngineUniforms> *new_list = new std::vector<EG::Graphics::ShaderManager::EngineUniforms>;
                    engine_shader_uniforms.Set(id, new_list);
                }
                engine_shader_uniforms.Get(id)->push_back(variable_name);
            }
        }

        void ShaderManager::AddObjectUniformUsage(std::string shader_id, std::string variable_name, ShaderUniformTypes variable_type){
            std::cout << shader_id << ": " << variable_name << std::endl;
            if (program_objects.Has(shader_id)){
                unsigned int id = program_objects.Get(shader_id);
                if (!(object_shader_uniforms.Has(id))){
                    std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> > *new_list = new std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> >;
                    object_shader_uniforms.Set(id, new_list);
                }
                object_shader_uniforms.Get(id)->push_back(std::pair<std::string, ShaderUniformTypes>(variable_name, variable_type));
            }
        }

        std::vector<EG::Graphics::ShaderManager::EngineUniforms> *ShaderManager::GetEngineUniforms(std::string shader_id){
            if (program_objects.Has(shader_id)){
                unsigned int id = program_objects.Get(shader_id);
                if (engine_shader_uniforms.Has(id)){
                    return engine_shader_uniforms.Get(id);
                }
            }
            return NULL;
        }

         std::vector<std::pair<std::string, EG::Graphics::ShaderManager::ShaderUniformTypes> > *ShaderManager::GetObjectUniforms(std::string shader_id){
            if (program_objects.Has(shader_id)){
                unsigned int id = program_objects.Get(shader_id);
                if (object_shader_uniforms.Has(id)){
                    return object_shader_uniforms.Get(id);
                }
            }
            return NULL;
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

        // Type, Name
        std::vector<std::pair<std::string, std::string> > ShaderManager::FindUniforms(char **source, int *sizes, int line_count){
            std::vector<std::pair<std::string, std::string> > results;
            for (unsigned int i = 0; i < line_count; i++){
                unsigned int line_size = sizes[i];
                char *line_c_str = source[i];
                std::string line(line_c_str);
                int pos = line.find("void");
                if (pos != -1){
                    break;
                }
                pos = line.find("uniform");
                if (pos != -1){
                    int type_pos = line.find_first_not_of(' ', pos + 7);
                    int after_type_pos = line.find_first_of(' ', type_pos);
                    std::string type = line.substr(type_pos, after_type_pos - type_pos);
                    int name_pos = line.find_first_not_of(' ', after_type_pos);
                    int semi_pos = line.find_first_of(';', after_type_pos);
                    std::string name = line.substr(name_pos, semi_pos - name_pos);
                    results.push_back(std::pair<std::string, std::string>(type, name));
                    //std::cout << type << ":" << name << std::endl;
                }
            }
            return results;
        }

        void ShaderManager::StoreShaderUniforms(std::string shader_id, std::vector<std::pair<std::string, std::string> > params){
            std::vector<std::pair<std::string, std::string> >::iterator params_iter = params.begin();
            while (params_iter != params.end()){
                std::pair<std::string, std::string> variable = (*params_iter);
                std::string type = variable.first;
                std::string name = variable.second;
                if (engine_uniform_string_translations.Has(name)){
                    AddEngineUniformUsage(shader_id, engine_uniform_string_translations.Get(name));
                } else if (uniform_type_translations.Has(type)){
                    AddObjectUniformUsage(shader_id, name, uniform_type_translations.Get(type));
                }
                ++params_iter;
            }
        }

        void ShaderManager::InterpretShaderVariables(EG::Graphics::ShaderSource *sources, std::string shader_id) {
            std::vector<std::pair<std::string, std::string> > results;
            if (sources->vertex_shader_line_count > 0) {
                results = FindUniforms(sources->vertex_shader_source, sources->vertex_shader_line_sizes, sources->vertex_shader_line_count);
                StoreShaderUniforms(shader_id, results);
            }
            if (sources->fragment_shader_line_count> 0) {
                results = FindUniforms(sources->fragment_shader_source, sources->fragment_shader_line_sizes, sources->fragment_shader_line_count);
                StoreShaderUniforms(shader_id, results);
            }
            if (sources->geometry_shader_line_count > 0) {
                results = FindUniforms(sources->geometry_shader_source, sources->geometry_shader_line_sizes, sources->geometry_shader_line_count);
                StoreShaderUniforms(shader_id, results);
            }
            if (sources->tessellation_control_shader_line_count > 0) {
                results = FindUniforms(sources->tessellation_control_shader_source, sources->tessellation_control_shader_line_sizes, sources->tessellation_control_shader_line_count);
                StoreShaderUniforms(shader_id, results);
            }
            if (sources->tessellation_evaluation_shader_line_count > 0) {
                results = FindUniforms(sources->tessellation_evaluation_shader_source, sources->tessellation_evaluation_shader_line_sizes, sources->tessellation_evaluation_shader_line_count);
                StoreShaderUniforms(shader_id, results);
            }
        }

        void ShaderManager::CreateUniformStringTranslations(void){
            engine_uniform_string_translations.Set("projection_matrix", EG::Graphics::ShaderManager::ENGINE_PROJECTION_MATRIX);
            // replace both of these with just projection matrix
            engine_uniform_string_translations.Set("perspective_matrix", EG::Graphics::ShaderManager::ENGINE_PERSPECTIVE_MATRIX);
            engine_uniform_string_translations.Set("ortho_matrix", EG::Graphics::ShaderManager::ENGINE_ORTHO_MATRIX);
            engine_uniform_string_translations.Set("camera_matrix", EG::Graphics::ShaderManager::ENGINE_CAMERA_MATRIX);
            engine_uniform_string_translations.Set("camera_position", EG::Graphics::ShaderManager::ENGINE_CAMERA_POSITION);
            engine_uniform_string_translations.Set("view_matrix", EG::Graphics::ShaderManager::ENGINE_VIEW_MATRIX);
            engine_uniform_string_translations.Set("model_matrix", EG::Graphics::ShaderManager::ENGINE_MODEL_MATRIX);
            engine_uniform_string_translations.Set("normal_matrix", EG::Graphics::ShaderManager::ENGINE_NORMAL_MATRIX);
            engine_uniform_string_translations.Set("light_position", EG::Graphics::ShaderManager::ENGINE_LIGHT_POSITION);
            engine_uniform_string_translations.Set("light_radius", EG::Graphics::ShaderManager::ENGINE_LIGHT_RADIUS);
            engine_uniform_string_translations.Set("light_attenuation", EG::Graphics::ShaderManager::ENGINE_LIGHT_ATTENUATION);
            engine_uniform_string_translations.Set("light_color", EG::Graphics::ShaderManager::ENGINE_LIGHT_COLOR);
            engine_uniform_string_translations.Set("decal_map", EG::Graphics::ShaderManager::ENGINE_DECAL_MAP);
            engine_uniform_string_translations.Set("height_map", EG::Graphics::ShaderManager::ENGINE_HEIGHT_MAP);
            engine_uniform_string_translations.Set("normal_map", EG::Graphics::ShaderManager::ENGINE_NORMAL_MAP);
            engine_uniform_string_translations.Set("specular_map", EG::Graphics::ShaderManager::ENGINE_SPECULAR_MAP);
            engine_uniform_string_translations.Set("depth_map", EG::Graphics::ShaderManager::ENGINE_DEPTH_MAP);
            engine_uniform_string_translations.Set("normal_mapping_enabled", EG::Graphics::ShaderManager::ENGINE_NORMAL_MAPPING_ENABLED);
            engine_uniform_string_translations.Set("shadow_mapping_enabled", EG::Graphics::ShaderManager::ENGINE_SHADOW_MAPPING_ENABLED);
            engine_uniform_string_translations.Set("material_color", EG::Graphics::ShaderManager::ENGINE_MATERIAL_COLOR);
            engine_uniform_string_translations.Set("material_specularity", EG::Graphics::ShaderManager::ENGINE_MATERIAL_SPECULARITY);
            engine_uniform_string_translations.Set("receives_lighting", EG::Graphics::ShaderManager::ENGINE_RECEIVES_LIGHTING);
            engine_uniform_string_translations.Set("shadow_mapping_bias", EG::Graphics::ShaderManager::ENGINE_SHADOW_MAPPING_BIAS);
            engine_uniform_string_translations.Set("shadow_mapping_size", EG::Graphics::ShaderManager::ENGINE_SHADOW_MAPPING_SIZE);
            engine_uniform_string_translations.Set("position_buffer", EG::Graphics::ShaderManager::ENGINE_POSITION_BUFFER);
            engine_uniform_string_translations.Set("normal_buffer", EG::Graphics::ShaderManager::ENGINE_NORMAL_BUFFER);
            engine_uniform_string_translations.Set("transparent_buffer", EG::Graphics::ShaderManager::ENGINE_TRANSPARENT_BUFFER);
            engine_uniform_string_translations.Set("color_buffer", EG::Graphics::ShaderManager::ENGINE_COLOR_BUFFER);
            engine_uniform_string_translations.Set("bloom_buffer", EG::Graphics::ShaderManager::ENGINE_BLOOM_BUFFER);
            engine_uniform_string_translations.Set("ssao_buffer", EG::Graphics::ShaderManager::ENGINE_SSAO_BUFFER);
            engine_uniform_string_translations.Set("luminance", EG::Graphics::ShaderManager::ENGINE_LUMINANCE);
            engine_uniform_string_translations.Set("bloom_scale", EG::Graphics::ShaderManager::ENGINE_BLOOM_SCALE);
            engine_uniform_string_translations.Set("ssao_enabled", EG::Graphics::ShaderManager::ENGINE_SSAO_ENABLED);
            engine_uniform_string_translations.Set("bloom_enabled", EG::Graphics::ShaderManager::ENGINE_BLOOM_ENABLED);
            engine_uniform_string_translations.Set("luminance_scale", EG::Graphics::ShaderManager::ENGINE_LUMINANCE_SCALE);
            engine_uniform_string_translations.Set("resolution", EG::Graphics::ShaderManager::ENGINE_RESOLUTION);
            engine_uniform_string_translations.Set("near_far", EG::Graphics::ShaderManager::ENGINE_NEAR_FAR);
            uniform_type_translations.Set("sampler2D", EG::Graphics::ShaderManager::UNIFORM_TEXTURE);
            uniform_type_translations.Set("samplerCube", EG::Graphics::ShaderManager::UNIFORM_CUBEMAP);
            uniform_type_translations.Set("int", EG::Graphics::ShaderManager::UNIFORM_INT);
            uniform_type_translations.Set("ivec2", EG::Graphics::ShaderManager::UNIFORM_INT2);
            uniform_type_translations.Set("ivec3", EG::Graphics::ShaderManager::UNIFORM_INT3);
            uniform_type_translations.Set("ivec4", EG::Graphics::ShaderManager::UNIFORM_INT4);
            uniform_type_translations.Set("float", EG::Graphics::ShaderManager::UNIFORM_FLOAT);
            uniform_type_translations.Set("vec2", EG::Graphics::ShaderManager::UNIFORM_FLOAT2);
            uniform_type_translations.Set("vec3", EG::Graphics::ShaderManager::UNIFORM_FLOAT3);
            uniform_type_translations.Set("vec4", EG::Graphics::ShaderManager::UNIFORM_FLOAT4);
            uniform_type_translations.Set("mat2", EG::Graphics::ShaderManager::UNIFORM_MAT2);
            uniform_type_translations.Set("mat3", EG::Graphics::ShaderManager::UNIFORM_MAT3);
            uniform_type_translations.Set("mat4", EG::Graphics::ShaderManager::UNIFORM_MAT4);
        }
    }
}
