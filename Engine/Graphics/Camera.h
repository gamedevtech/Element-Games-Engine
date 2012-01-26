#ifndef EG_GRAPHICS_CAMERA_H
#define EG_GRAPHICS_CAMERA_H

#include "../Math/Math.h"

namespace EG{
	namespace Graphics{
		class Camera{
			public:
				enum CameraType{
					CAMERA_FPS,
					CAMERA_DEFAULT
				};

				Camera(float _fov, glm::ivec2 _view_size, glm::vec2 _near_far);
				~Camera(void);

				void SetOffset(glm::vec3 _offset);
				void SetPosition(glm::vec3 _position);
				void SetOrientation(glm::quat _orientation);
				void SetViewSize(glm::ivec2 _view_size);
				void SetNearFar(glm::vec2 _near_far);
				void SetFOV(float _fov);
				void SetCameraType(EG::Graphics::Camera::CameraType _camera_type);
                                void SetViewMatrix(glm::mat4 _view_matrix);

				void ComputeViewMatrix(void);
				void ComputeProjectionMatrix(void);
				void Update(void);
				void Move(glm::vec3 movement);
				void Rotate(glm::vec3 angles);
				void RotateByMouse(glm::vec2 mouse_deltas);

				glm::vec3 GetPosition(void);
				glm::vec3 GetOffset(void);
				glm::quat GetOrientation(void);
				glm::ivec2 GetViewSize(void);
				glm::vec2 GetNearFar(void);
				float GetFOV(void);
				glm::mat4 GetViewMatrix(void);
				glm::mat4 GetInverseViewMatrix(void);
				glm::mat4 GetProjectionMatrix(void);
				glm::mat4 GetInverseProjectionMatrix(void);
			private:
				// View Matrix Stuff
				glm::vec3 offset;
				glm::vec3 position;
				glm::quat orientation;
				glm::mat4 view_matrix;
				glm::mat4 inverse_view_matrix;

				// Projection Matrix Stuff
				glm::ivec2 view_size;
				glm::vec2 near_far;
				float fov;
				glm::mat4 projection_matrix;
				glm::mat4 inverse_projection_matrix;

				// Other Camera Info
				CameraType camera_type;
				glm::vec2 fps_constraints;

				bool invert_y;
				bool mouse_x_roll;
		};
	}
}

#endif
