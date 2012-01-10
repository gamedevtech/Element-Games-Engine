#include "../GraphicsSubsystem.h"

namespace EG{
	namespace Graphics{
		// Singleton Stuff
		GraphicsSubsystem *GraphicsSubsystem::instance = NULL;
		GraphicsSubsystem *GraphicsSubsystem::Instance(void){
			if (instance == NULL){
				instance = new GraphicsSubsystem();
			}
			return instance;
		}

		// Constructors
		GraphicsSubsystem::GraphicsSubsystem(void){
			using_tessellation = false;
		}
		GraphicsSubsystem::~GraphicsSubsystem(void){
			//
		}

		void GraphicsSubsystem::Initialize(unsigned int _viewport_width, unsigned int _viewport_height){
			viewport_width = _viewport_width;
			viewport_height = _viewport_height;
			opengl_interface = new OpenGLInterface();
			opengl_interface->Initialize(viewport_width, viewport_height);
			using_tessellation = false;
		}

		void GraphicsSubsystem::Resize(unsigned int _viewport_width, unsigned int _viewport_height){
			viewport_width = _viewport_width;
			viewport_height = _viewport_height;
			opengl_interface->Resize(viewport_width, viewport_height);
		}

		void GraphicsSubsystem::BeginFrame(void){
			opengl_interface->Begin3D();
		}

		void GraphicsSubsystem::EndFrame(void){
			opengl_interface->End3D();
		}

		void GraphicsSubsystem::PushMatrix(void){
			opengl_interface->PushMatrix();
		}

		void GraphicsSubsystem::PopMatrix(void){
			opengl_interface->PopMatrix();
		}

		unsigned int GraphicsSubsystem::GetVersionMajor(void){
			return opengl_interface->GetVersionMajor();
		}

		unsigned int GraphicsSubsystem::GetVersionMinor(void){
			return opengl_interface->GetVersionMinor();
		}

		bool GraphicsSubsystem::CheckVersion(unsigned int major, unsigned int minor){
			if (GetVersionMajor() >= major && GetVersionMinor() >= minor){
				return true;
			}
			return false;
		}

		void GraphicsSubsystem::OverrideVersion(int major, int minor){
			opengl_interface->OverrideVersion(major, minor);
		}

		bool GraphicsSubsystem::CheckErrors(std::string prefix){
			return opengl_interface->ErrorCheck(prefix);
		}

		// Shaders
		void GraphicsSubsystem::ShaderCreateShaderObjectIds(unsigned int *object_ids, bool geometry, bool tessellation_control, bool tessellation_evaluation){
			opengl_interface->ShaderCreateShaderObjectIds(object_ids, geometry, tessellation_control, tessellation_evaluation);
		}

		void GraphicsSubsystem::ShaderSetShaderSource(unsigned int *object_ids, char **vertex_source, int vertex_lines, int *vertex_line_sizes, char **fragment_source, int fragment_lines, int *fragment_line_sizes, bool geometry, char **geometry_source, int geometry_lines, int *geometry_line_sizes, bool tessellation_control, char **tessellation_control_source, int tessellation_control_lines, int *tessellation_control_line_sizes, bool tessellation_evaluation, char **tessellation_evaluation_source, int tessellation_evaluation_lines, int *tessellation_evaluation_line_sizes){
			opengl_interface->ShaderSetShaderSource(object_ids, vertex_source, vertex_lines, vertex_line_sizes, fragment_source, fragment_lines, fragment_line_sizes, geometry, geometry_source, geometry_lines, geometry_line_sizes, tessellation_control, tessellation_control_source, tessellation_control_lines, tessellation_control_line_sizes, tessellation_evaluation, tessellation_evaluation_source, tessellation_evaluation_lines, tessellation_evaluation_line_sizes);
		}

		void GraphicsSubsystem::ShaderCompile(unsigned int *object_ids, bool geometry_flag, bool tessellation_control_flag, bool tessellation_evaluation_flag, unsigned int fragment_outputs){
			opengl_interface->ShaderCompile(object_ids, geometry_flag, tessellation_control_flag, tessellation_evaluation_flag, fragment_outputs);
		}

		void GraphicsSubsystem::ShaderBind(unsigned int program_object_id){
			opengl_interface->ShaderBind(program_object_id);
		}

		void GraphicsSubsystem::ShaderRelease(unsigned int *program_object_id, unsigned int *vertex_object_id, unsigned int *fragment_object_id, unsigned int *geometry_object_id, unsigned int *tessellation_control_object_id, unsigned int *tessellation_evaluation_object_id){
			opengl_interface->ShaderRelease(program_object_id, vertex_object_id, fragment_object_id, geometry_object_id, tessellation_control_object_id, tessellation_evaluation_object_id);
		}

		std::string GraphicsSubsystem::ShaderProgramLog(unsigned int program_object_id){
			return opengl_interface->ShaderProgramLog(program_object_id);
		}

		std::string GraphicsSubsystem::ShaderShaderLog(unsigned int shader_object_id){
			return opengl_interface->ShaderShaderLog(shader_object_id);
		}

		int GraphicsSubsystem::ShaderGetUniformLocation(unsigned int program_object_id, const char *variable_name){
			return opengl_interface->ShaderGetUniformLocation(program_object_id, variable_name);
		}

		int GraphicsSubsystem::ShaderGetAttributeLocation(unsigned int program_object_id, const char *variable_name){
			return opengl_interface->ShaderGetAttributeLocation(program_object_id, variable_name);
		}

		void GraphicsSubsystem::ShaderSetInt(unsigned int variable_location, int value0){
			opengl_interface->ShaderSetInt(variable_location, value0);
		}

		void GraphicsSubsystem::ShaderSetInt2(unsigned int variable_location, int value0, int value1){
			opengl_interface->ShaderSetInt2(variable_location, value0, value1);
		}

		void GraphicsSubsystem::ShaderSetInt3(unsigned int variable_location, int value0, int value1, int value2){
			opengl_interface->ShaderSetInt3(variable_location, value0, value1, value2);
		}

		void GraphicsSubsystem::ShaderSetInt4(unsigned int variable_location, int value0, int value1, int value2, int value3){
			opengl_interface->ShaderSetInt4(variable_location, value0, value1, value2, value3);
		}

		void GraphicsSubsystem::ShaderSetIntV(unsigned int variable_location, int size, int *data){
			opengl_interface->ShaderSetIntV(variable_location, size, data);
		}

		void GraphicsSubsystem::ShaderSetFloat(unsigned int variable_location, float value0){
			opengl_interface->ShaderSetFloat(variable_location, value0);
		}

		void GraphicsSubsystem::ShaderSetFloat2(unsigned int variable_location, float value0, float value1){
			opengl_interface->ShaderSetFloat2(variable_location, value0, value1);
		}

		void GraphicsSubsystem::ShaderSetFloat3(unsigned int variable_location, float value0, float value1, float value2){
			opengl_interface->ShaderSetFloat3(variable_location, value0, value1, value2);
		}

		void GraphicsSubsystem::ShaderSetFloat4(unsigned int variable_location, float value0, float value1, float value2, float value3){
			opengl_interface->ShaderSetFloat4(variable_location, value0, value1, value2, value3);
		}

		void GraphicsSubsystem::ShaderSetFloatV(unsigned int variable_location, int size, float *data){
			opengl_interface->ShaderSetFloatV(variable_location, size, data);
		}

		void GraphicsSubsystem::ShaderSetMatrix2(unsigned int variable_location, float *data){
			opengl_interface->ShaderSetMatrix2(variable_location, data);
		}

		void GraphicsSubsystem::ShaderSetMatrix3(unsigned int variable_location, float *data){
			opengl_interface->ShaderSetMatrix3(variable_location, data);
		}

		void GraphicsSubsystem::ShaderSetMatrix4(unsigned int variable_location, float *data){
			opengl_interface->ShaderSetMatrix4(variable_location, data);
		}

		void GraphicsSubsystem::SetUsingTessellation(bool _using_tessellation){
			using_tessellation = _using_tessellation;
		}

		void GraphicsSubsystem::GenerateMeshBuffer(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool has_vertices, float *vertices, bool has_texcoords, float *texcoords, bool has_normals, float *normals, bool has_binormals, float *binormals, bool has_bitangents, float *bitangents){
			opengl_interface->GenerateMeshBuffer(vertex_array_object_id, vertex_buffer_object_ids, vertex_count, has_vertices, vertices, has_texcoords, texcoords, has_normals, normals, has_binormals, binormals, has_bitangents, bitangents);
		}

		void GraphicsSubsystem::DrawMesh(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool vertices, bool texcoords, bool normals, bool binormals, bool bitangents){
			opengl_interface->DrawMesh(vertex_array_object_id, vertex_buffer_object_ids, vertex_count, using_tessellation, vertices, texcoords, normals, binormals, bitangents);
		}

		void GraphicsSubsystem::SetActiveTexture(unsigned int texture_index){
			opengl_interface->SetActiveTexture(texture_index);
		}

		unsigned int GraphicsSubsystem::CreateTexture(unsigned char *pixels, int width, int height, bool filtering, float anisotropy){
			return opengl_interface->CreateTexture(pixels, width, height, filtering, anisotropy);
		}

		unsigned int GraphicsSubsystem::CreateCubeMap(unsigned char **pixels, int width, int height, bool filtering, float anisotropy){
			return opengl_interface->CreateCubeMap(pixels, width, height, filtering, anisotropy);
		}

		void GraphicsSubsystem::BindTexture(unsigned int texture_id, unsigned int texture_index){
			opengl_interface->BindTexture(texture_id, texture_index);
		}

		void GraphicsSubsystem::BindCubeMap(unsigned int texture_id, unsigned int texture_index){
			opengl_interface->BindCubeMap(texture_id, texture_index);
		}

		void GraphicsSubsystem::GetTextureDataFloats(float *texture_data, unsigned int texture_index){
			opengl_interface->GetTextureDataFloats(texture_data, texture_index);
		}

		void GraphicsSubsystem::CreateOffscreenBuffer(unsigned int *buffer_id, unsigned int *textures, unsigned int *depth_texture, unsigned int width, unsigned int height, unsigned int color_buffer_count, bool use_depth, unsigned int texture_filtering){
			opengl_interface->CreateOffscreenBuffer(buffer_id, textures, depth_texture, width, height, color_buffer_count, use_depth, texture_filtering);
		}

		/*void GraphicsSubsystem::BindOffscreenBuffer(unsigned int *buffer_id){
			current_draw_buffer = opengl_interface->GetCurrentDrawBuffer();
			opengl_interface->BindOffscreenBuffer(buffer_id);
		}

		void GraphicsSubsystem::UnbindOffscreenBuffer(void){
			opengl_interface->UnbindOffscreenBuffer();
			opengl_interface->SetDrawBuffer(current_draw_buffer);
		}*/

		void GraphicsSubsystem::StartOffscreenRender(unsigned int *buffer_id, unsigned int buffer_index, unsigned int width, unsigned int height){
			opengl_interface->StartOffscreenRender(buffer_id, buffer_index, width, height);
		}

		void GraphicsSubsystem::EndOffscreenRender(void){
			opengl_interface->EndOffscreenRender();
		}

		void GraphicsSubsystem::StartMultiBufferOffscreenRender(unsigned int *buffer_id, unsigned int buffer_count, int *buffers, unsigned int width, unsigned int height){
			opengl_interface->StartMultiBufferOffscreenRender(buffer_id, buffer_count, buffers, width, height);
		}

		void GraphicsSubsystem::EndMultiBufferOffscreenRender(void){
			opengl_interface->EndMultiBufferOffscreenRender();
		}

        unsigned int GraphicsSubsystem::GetViewportWidth(void){
            return viewport_width;
        }

        unsigned int GraphicsSubsystem::GetViewportHeight(void){
            return viewport_height;
        }
	}
}
