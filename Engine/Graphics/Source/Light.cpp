#include "../Light.h"

namespace EG{
	namespace Graphics{
		Light::Light(void){
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			attenuation = glm::vec3(0.8f, 0.125f, 0.000001f);
			position = glm::vec3(0.0f, 0.0f, 0.0f);
			direction = glm::vec3(0.0f, 0.0f, 0.0f);
			spot_angle = 0.0f;
			casts_shadows = false;
			shadowing_initialized = false;
			radius = 500.0f;
			shadow_map_resolution = 2056.0f;
		}

		Light::~Light(void){
			//
		}

		void Light::SetColor(glm::vec3 _color){
			color = _color;
		}

		void Light::SetAttenuation(glm::vec3 _attenuation){
			attenuation = _attenuation;
		}

		void Light::SetPosition(glm::vec3 _position){
			position = _position;
		}

		void Light::SetDirection(glm::vec3 _direction){
			direction = _direction;
		}

		void Light::SetCastsShadows(bool _casts_shadows){
			casts_shadows = _casts_shadows;
		}

		void Light::SetRadius(float _radius){
			radius = _radius;
		}

		void Light::SetSpotAngle(float _angle){
			spot_angle = _angle;
		}

		glm::vec3 Light::GetColor(void){
			return color;
		}

		glm::vec3 Light::GetAttenuation(void){
			return attenuation;
		}

		glm::vec3 Light::GetPosition(void){
			return position;
		}

		glm::vec3 Light::GetDirection(void){
			return direction;
		}

		bool Light::GetCastsShadows(void){
			return casts_shadows;
		}

		float Light::GetRadius(void){
			return radius;
		}

		float Light::GetSpotAngle(void){
			return spot_angle;
		}

		void Light::Update(void){
			if (!shadowing_initialized){
				shadowing_initialized = true;
				float angle = spot_angle;
				if (spot_angle == 0.0f){
					angle = 90.0f;//180.0f;
				}
				projection_matrix = glm::perspective(angle, shadow_map_resolution /shadow_map_resolution, 0.1f, radius);
				shadow_buffer = new EG::Graphics::OffscreenBuffer(int(shadow_map_resolution), int(shadow_map_resolution), 1, true, EG::Graphics::OffscreenBuffer::OFFSCREEN_BUFFER_FILTERING_LINEAR);
			}
			glm::vec3 temp = direction;
			if (direction.x > 0.0f || direction.y > 0.0f || direction.z > 0.0f){
				temp = position + direction;
			}
			view_matrix = glm::gtc::matrix_transform::lookAt(position, temp, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		glm::mat4 Light::GetProjectionMatrix(void){
			return projection_matrix;
		}

		glm::mat4 Light::GetViewMatrix(void){
			return view_matrix;
		}

		EG::Graphics::OffscreenBuffer *Light::GetShadowBuffer(void){
			return shadow_buffer;
		}

		float Light::GetShadowMapResolution(void){
			return shadow_map_resolution;
		}
	}
}
