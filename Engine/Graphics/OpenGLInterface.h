#ifndef EG_GRAPHICS_OPENGL_INTERFACE_H
#define EG_GRAPHICS_OPENGL_INTERFACE_H

#ifdef WIN32
#define GLEW_STATIC
#endif

#include "../Math/Math.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <string>

namespace EG{
	namespace Graphics{
		class OpenGLInterface{
			public:
				enum MeshBufferAttributeIndex{
					VBO_VERTICES = 0,
					VBO_TEXCOORDS = 1,
					VBO_NORMALS = 2,
					VBO_BINORMALS = 3,
					VBO_BITANGENTS = 4
				};

				OpenGLInterface(void);
				~OpenGLInterface(void);

				void Initialize(unsigned int viewport_width, unsigned int viewport_height/*, float near, float far*/);
				void Resize(unsigned int viewport_width, unsigned int viewport_height);

				void Begin3D(/*unsigned int viewport_width, unsigned int viewport_height, float near, float far*/void);
				void End3D(void);
				void Begin2D(glm::mat4 orthographics_projection_matrix);
				void End2D(void);
				void PushMatrix(void);
				void PopMatrix(void);

				std::string GetVersionString(void);
				unsigned int GetVersionMajor(void);
				unsigned int GetVersionMinor(void);
				void OverrideVersion(int major, int minor);

				bool ErrorCheck(std::string prefix);

				// Shader Interface
				void ShaderCreateShaderObjectIds(unsigned int *object_ids, bool geometry = false, bool tessellation_control = false, bool tessellation_evaluation = false);
				void ShaderSetShaderSource(unsigned int *object_ids, char **vertex_source, int vertex_lines, int *vertex_line_sizes, char **fragment_source, int fragment_lines, int *fragment_line_sizes, bool geometry = false, char **geometry_source = NULL, int geometry_lines = 0, int *geometry_line_sizes = NULL, bool tessellation_control = false, char **tessellation_control_source = NULL, int tessellation_control_lines = 0, int *tessellation_control_line_sizes = NULL, bool tessellation_evaluation = false, char **tessellation_evaluation_source = NULL, int tessellation_evaluation_lines = 0, int *tessellation_evaluation_line_sizes = NULL);
				void ShaderCompile(unsigned int *object_ids, bool geometry = false, bool tessellation_control = false, bool tessellation_evaluation = false, unsigned int fragment_outputs = 1);
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
				void ShaderSetMatrix4(unsigned int variable_location, float *data);
				bool BasicShadersSupported(void);

				void GenerateMeshBuffer(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool has_vertices, float *vertices, bool has_texcoords, float *texcoords, bool has_normals, float *normals, bool has_binormals, float *binormals, bool has_bitangents, float *bitangents);
				void DrawMesh(unsigned int *vertex_array_object_id, unsigned int *vertex_buffer_object_ids, unsigned int vertex_count, bool using_tessellation, bool vertices, bool texcoords, bool normals, bool binormals, bool bitangents);

				void SetActiveTexture(unsigned int texture_index = 0);
				void BindTexture(unsigned int texture_id, unsigned int texture_index = 0);
				void BindCubeMap(unsigned int texture_id, unsigned int texture_index = 0);
				void GetTextureDataFloats(float *texture_data, unsigned int texture_index = 0);
				unsigned int CreateTexture(unsigned char *pixels, int width, int height, bool filtering, float anisotropy);
				unsigned int CreateCubeMap(unsigned char **pixels, int width, int height, bool filtering, float anisotropy);
				void CreateOffscreenBuffer(unsigned int *buffer_id, unsigned int *textures, unsigned int *depth_texture, unsigned int width, unsigned int height, unsigned int color_buffer_count, bool use_depth, unsigned int texture_filtering);
				void BindOffscreenBuffer(unsigned int *buffer_id);
				void StoreCurrentDrawBuffer(void);
				void UnbindOffscreenBuffer(void);
				void RestoreDrawBuffer(void);
				void StartOffscreenRender(unsigned int *buffer_id, unsigned int buffer_index, unsigned int width, unsigned int height);
				void EndOffscreenRender(void);
				void StartMultiBufferOffscreenRender(unsigned int *buffer_id, unsigned int buffer_count, int *buffers, unsigned int width, unsigned int height);
				void EndMultiBufferOffscreenRender(void);
			private:
				std::string version_string;
				unsigned int version_major, version_minor;
				unsigned int draw_buffer;
		};
	}
}

#endif
