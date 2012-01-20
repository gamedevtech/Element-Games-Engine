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
            blending_mode = EG::Graphics::RenderingMaterial::BLEND_OFF;
        }

        RenderingMaterial::~RenderingMaterial(void){
            std::vector<EG::Graphics::RenderingMaterial::RendererType>::iterator shiter = shaders.GetKeysBegin();
            while (shiter != shaders.GetKeysEnd()){
                delete shaders.Get(*shiter);
                ++shiter;
            }
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
            textures.Set(type, texture_id);
        }

        void RenderingMaterial::SetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type, std::string texture_id){
            cube_maps.Set(type, texture_id);
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
            if (textures.Has(type)){
                return textures.Get(type);
            }
            return "";
        }

        std::string RenderingMaterial::GetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            if (cube_maps.Has(type)){
                return cube_maps.Get(type);
            }
            return "";
        }

        bool RenderingMaterial::HasTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            return textures.Has(type);
        }

        bool RenderingMaterial::HasCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            return cube_maps.Has(type);
        }

        void RenderingMaterial::SetShaderOverride(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase, std::string shader_id){
            if (!(shaders.Has(renderer_type))){
                shaders.Set(renderer_type, new EG::Utility::Dictionary<EG::Graphics::RenderingMaterial::RenderingPhaseShaderType, std::string>());
            }
            shaders.Get(renderer_type)->Set(rendering_phase, shader_id);
        }

        bool RenderingMaterial::HasShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase){
            if (shaders.Has(renderer_type)){
                return shaders.Get(renderer_type)->Has(rendering_phase);
            }
            return false;
        }

        std::string RenderingMaterial::GetShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase){
            if (shaders.Has(renderer_type)){
                if (shaders.Get(renderer_type)->Has(rendering_phase)){
                    return shaders.Get(renderer_type)->Get(rendering_phase);
                }
            }
            return "";
        }

        void RenderingMaterial::SetBlendingMode(EG::Graphics::RenderingMaterial::BlendingMode _blending_mode){
            blending_mode = _blending_mode;
        }

        EG::Graphics::RenderingMaterial::BlendingMode RenderingMaterial::GetBlendingMode(void){
            return blending_mode;
        }
    }
}
