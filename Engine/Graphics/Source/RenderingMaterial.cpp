#include "../RenderingMaterial.h"

namespace EG{
	namespace Graphics{
		RenderingMaterial::RenderingMaterial(void){
			lit = true;
			translucent = false;
			casts_shadows = true;
			ambient = 0.0f;
			diffuse = 1.0f;
			specular = 1.0f;
			specular_exponent = 16.0f;
			color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		RenderingMaterial::~RenderingMaterial(void){
			//
		}

		void RenderingMaterial::SetLit(bool _lit){
			lit = _lit;
		}

		void RenderingMaterial::SetTranslucent(bool _translucent){
			translucent = _translucent;
		}

		void RenderingMaterial::SetCastsShadows(bool _casts_shadows){
			casts_shadows = _casts_shadows;
		}

		void RenderingMaterial::SetAmbient(float _ambient){
			ambient = _ambient;
		}

		void RenderingMaterial::SetDiffuse(float _diffuse){
			diffuse = _diffuse;
		}

		void RenderingMaterial::SetSpecular(float _specular){
			specular = _specular;
		}

		void RenderingMaterial::SetSpecularExponent(float _specular_exponent){
			specular_exponent = _specular_exponent;
		}

		void RenderingMaterial::SetColor(glm::vec4 _color){
			color = _color;
		}

		void RenderingMaterial::SetTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type, std::string texture_id){
			textures[type] = texture_id;
		}

		void RenderingMaterial::SetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type, std::string texture_id){
			cube_maps[type] = texture_id;
		}

		bool RenderingMaterial::GetLit(void){
			return lit;
		}

		bool RenderingMaterial::GetTranslucent(void){
			return translucent;
		}

		bool RenderingMaterial::GetCastsShadows(void){
			return casts_shadows;
		}

		float RenderingMaterial::GetAmbient(void){
			return ambient;
		}

		float RenderingMaterial::GetDiffuse(void){
			return diffuse;
		}

		float RenderingMaterial::GetSpecular(void){
			return specular;
		}

		float RenderingMaterial::GetSpecularExponent(void){
			return specular_exponent;
		}

		glm::vec4 RenderingMaterial::GetColor(void){
			return color;
		}

		std::string RenderingMaterial::GetTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
			if (textures.count(type) > 0){
				return textures[type];
			}
			return "";
		}

		std::string RenderingMaterial::GetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
			if (cube_maps.count(type) > 0){
				return cube_maps[type];
			}
			return "";
		}

		bool RenderingMaterial::HasTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
			if (textures.count(type) > 0){
				return true;
			}
			return false;
		}

		bool RenderingMaterial::HasCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
			if (cube_maps.count(type) > 0){
				return true;
			}
			return false;
		}

		void RenderingMaterial::SetShaderOverride(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase, std::string shader_id){
			shaders[renderer_type][rendering_phase] = shader_id;
		}

		bool RenderingMaterial::HasShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase){
			if (shaders.count(renderer_type) > 0){
				if (shaders[renderer_type].count(rendering_phase) > 0){
					return true;
				}
			}
			return false;
		}

		std::string RenderingMaterial::GetShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase){
			if (shaders.count(renderer_type) > 0){
				if (shaders[renderer_type].count(rendering_phase) > 0){
					return shaders[renderer_type][rendering_phase];
				}
			}
			return "";
		}
	}
}
