#ifndef EG_GRAPHICS_GRAPHICS_SUBSYSTEM_H
#define EG_GRAPHICS_GRAPHICS_SUBSYSTEM_H

#include "OpenGLInterface.h"
#include "../Math/Math.h"

#define graphics EG::Graphics::GraphicsSubsystem::Instance()

namespace EG{
	namespace Graphics{
		class GraphicsSubsystem{
			public:
				GraphicsSubsystem(void);
				~GraphicsSubsystem(void);

				// Context Management
				void Initialize(unsigned int _viewport_width, unsigned int _viewport_height);
				void Resize(unsigned int _viewport_width, unsigned int _viewport_height);

				// 3D Scene
				void BeginFrame(void);
				void EndFrame(void);
				void PushMatrix(void);
				void PopMatrix(void);

				// Interface Version Helpers
				unsigned int GetVersionMajor(void);
				unsigned int GetVersionMinor(void);
				bool CheckVersion(unsigned int major, unsigned int minor = 0);
				void OverrideVersion(int major, int minor);

				// Check for Graphics Interface Errors
				bool CheckErrors(std::string prefix);

				// Shader Functions
				void ShaderCreateShaderObjectIds(unsigned int *object_ids, bool geometry = false, bool tessellation_control = false, bool tessellation_evaluation = false);
				void ShaderSetShaderSource(unsigned int *object_ids, char **vertex_source, int vertex_lines, int *vertex_line_sizes, char **fragment_source, int fragment_lines, int *fragment_line_sizes, bool geometry = false, char **geometry_source = NULL, int geometry_lines = 0, int *geometry_line_sizes = NULL, bool tessellation_control = false, char **tessellation_control_source = NULL, int tessellation_control_lines = 0, int *tessellation_control_line_sizes = NULL, bool tessellation_evaluation = false, char **tessellation_evaluation_source = NULL, int tessellation_evaluation_lines = 0, int *tessellation_evaluation_line_sizes = NULL);
				void ShaderCompile(unsigned int *object_ids, bool geometry_flag = false, bool tessellation_control_flag = false, bool tessellation_evaluation_flag = false, unsigned int fragment_outputs = 1);
				void ShaderBind(unsigned int program_object_id);
				void ShaderRelease(unsigned int *program_object_id, unsigned int *vertex_object_id, unsigned int *fragment_object_id, unsigned int *geometry_object_id = NULL, unsigned int *tessellation_control_object_id = NULL, unsigned int *tessellation_evaluation_object_id = NULL);
				std::string ShaderProgramLog(unsigned int program_object_id);
				std::string ShaderShaderLog(unsigned int shader_object_id);
				int ShaderGetUniformLocation(unsigned int program_object_id, const char *variable_name);
				int ShaderGetAttributeLocation(unsigned int program_object_id, const char *variable_name);
				void ShaderSetInt(unsigned int variable_location, int value0);
				void ShaderSetInt2(unsigned int variable_location, int value0, int value1);
				void ShaderSetInt3(unsigned int variable_location, int value0, int value1, int value2);
				void ShaderSetInt4(unsigned int variable_location, int value0, int value1, int value2, int value3);
				void ShaderSetIntV(unsigned int variable_location, int size, int *data);
				void ShaderSetFloat(unsigned int variable_location, float value0);
				void ShaderSetFloat2(unsigned int variable_location, float value0, float value1);
				void ShaderSetFloat3(unsigned int variable_location, float value0, float value1, float value2);
				void ShaderSetFloat4(unsigned int variable_location, float value0, float value1, float value2, float value3);
				void ShaderSetFloatV(unsigned int variable_location, int size, float *data);
				void ShaderSetMatrix2(unsigned int variable_location, float *data);
				void ShaderSetMatrix3(unsigned int variable_location, float *data);
				void ShaderSetMatrix4(unsigned int variable_location, float *data, unsigned int count = 1);

				// Mesh Methods
				void SetUsingTessellation(bool _using_tessellation);
				void GenerateMeshBuffer(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool has_vertices, float *vertices, bool has_texcoords, float *texcoords, bool has_normals, float *normals, bool has_binormals, float *binormals, bool has_bitangents, float *bitangents, bool has_skeleton, unsigned int *weight_vertex_indices, float *weights);
				void DrawMesh(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool vertices, bool texcoords, bool normals, bool binormals, bool bitangents, bool skeleton);

				// Textures
				void SetActiveTexture(unsigned int texture_index = 0);
				void BindTexture(unsigned int texture_id, unsigned int texture_index = 0);
				void BindCubeMap(unsigned int texture_id, unsigned int texture_index = 0);
				void GetTextureDataFloats(float *texture_data, unsigned int texture_index = 0);
				unsigned int CreateTexture(unsigned char *pixels, int width, int height, bool filtering, float anisotropy);
				unsigned int CreateCubeMap(unsigned char **pixels, int widht, int height, bool filtering, float anisotropy);

				// Offscreen Buffers
				void CreateOffscreenBuffer(unsigned int *buffer_id, unsigned int *textures, unsigned int *depth_texture, unsigned int width, unsigned int height, unsigned int color_buffer_count, bool use_depth, unsigned int texture_filtering);
				void StartOffscreenRender(unsigned int *buffer_id, unsigned int buffer_index, unsigned int width, unsigned int height);
				void EndOffscreenRender(void);
				void StartMultiBufferOffscreenRender(unsigned int *buffer_id, unsigned int buffer_count, int *buffers, unsigned int width, unsigned int height);
				void EndMultiBufferOffscreenRender(void);

                // Utility
                unsigned int GetViewportWidth(void);
                unsigned int GetViewportHeight(void);

                // Blending and Depth Testing
                void SetBlendingMode(unsigned int blending_mode = 0);

				// Singleton
				static GraphicsSubsystem *Instance(void);
			private:
				OpenGLInterface *opengl_interface;
				unsigned int viewport_width, viewport_height;
				bool using_tessellation;
				//unsigned int current_draw_buffer;

				// Singleton
				static GraphicsSubsystem* instance;
		};
	}
}

#endif
