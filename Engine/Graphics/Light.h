#ifndef EG_GRAPHICS_LIGHT_H
#define EG_GRAPHICS_LIGHT_H

#include "../Math/Math.h"
#include "OffscreenBuffer.h"
#include "Camera.h"

namespace EG{
	namespace Graphics{
		class Light{
			public:
				Light(void);
				~Light(void);

				void SetColor(glm::vec3 _color);
				void SetAttenuation(glm::vec3 _attenuation);
				void SetPosition(glm::vec3 _position);
				void SetDirection(glm::vec3 _direction);
				void SetCastsShadows(bool _casts_shadows);
				void SetRadius(float _radius);
				void SetSpotAngle(float _angle);

				glm::vec3 GetColor(void);
				glm::vec3 GetAttenuation(void);
				glm::vec3 GetPosition(void);
				glm::vec3 GetDirection(void);
				bool GetCastsShadows(void);
				float GetRadius(void);
				float GetSpotAngle(void);
				float GetShadowMapResolution(void);

				void Update(void);
				glm::mat4 GetProjectionMatrix(void);
				glm::mat4 GetViewMatrix(void);
				EG::Graphics::OffscreenBuffer *GetShadowBuffer(void);
			private:
				glm::vec3 color;

				glm::vec3 attenuation;
				float radius;

				glm::vec3 position;
				glm::vec3 direction;
				float spot_angle;

				bool casts_shadows;
				bool shadowing_initialized;
				float shadow_map_resolution;
				glm::mat4 projection_matrix, view_matrix;
				EG::Graphics::OffscreenBuffer *shadow_buffer;

				// Add Shape Texture
		};
	}
}

#endif
