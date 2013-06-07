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
            culling_mode = EG::Graphics::RenderingMaterial::CULL_OFF;
            cull_winding = EG::Graphics::RenderingMaterial::CULL_CW;
            depth_mask = true;
            depth_test = true;
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

        void RenderingMaterial::SetDepthTest(bool _depth_test) {
            depth_test = _depth_test;
        }

        void RenderingMaterial::SetCullWinding(EG::Graphics::RenderingMaterial::CullWinding _cull_winding) {
            cull_winding = _cull_winding;
        }

        void RenderingMaterial::SetDepthMask(bool _depth_mask) {
            depth_mask = _depth_mask;
        }

        bool RenderingMaterial::GetDepthMask(void) {
            return depth_mask;
        }

        bool RenderingMaterial::GetDepthTest(void) {
            return depth_test;
        }

        EG::Graphics::RenderingMaterial::CullWinding RenderingMaterial::GetCullWinding(void) {
            return cull_winding;
        }

        std::string RenderingMaterial::GetTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            if (textures.count(type)){
                return textures[type];
            }
            return "";
        }

        std::string RenderingMaterial::GetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            if (cube_maps.count(type)){
                return cube_maps[type];
            }
            return "";
        }

        bool RenderingMaterial::HasTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            return textures.count(type);
        }

        bool RenderingMaterial::HasCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type){
            return cube_maps.count(type);
        }

        void RenderingMaterial::SetShaderOverride(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase, std::string shader_id){
            shaders[renderer_type][rendering_phase] = shader_id;
        }

        bool RenderingMaterial::HasShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase){
            if (shaders.count(renderer_type)){
                return shaders[renderer_type].count(rendering_phase) > 0;
            }
            return false;
        }

        std::string RenderingMaterial::GetShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase){
            if (shaders.count(renderer_type)){
                if (shaders[renderer_type].count(rendering_phase)){
                    return shaders[renderer_type][rendering_phase];
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
        void RenderingMaterial::SetCullingMode(EG::Graphics::RenderingMaterial::CullingMode _culling_mode) {
            culling_mode = _culling_mode;
        }
        EG::Graphics::RenderingMaterial::CullingMode RenderingMaterial::GetCullingMode(void) {
            return culling_mode;
        }
    }
}
