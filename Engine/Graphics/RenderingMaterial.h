#ifndef EG_GRAPHICS_RENDERING_MATERIAL_H
#define EG_GRAPHICS_RENDERING_MATERIAL_H

#include <map>
#include <unordered_map>
#include <string>
#include "../Math/Math.h"

namespace EG{
    namespace Graphics{
        class RenderingMaterial{
            public:
                enum RenderingMaterialTextureType{
                    RENDERING_MATERIAL_TEXTURE_DECAL = 0,
                    RENDERING_MATERIAL_TEXTURE_NORMAL = 1,
                    RENDERING_MATERIAL_TEXTURE_HEIGHT = 2,
                    RENDERING_MATERIAL_TEXTURE_SPECULAR = 3
                };
                enum RenderingPhaseShaderType{
                    RENDERING_PHASE_PREPASS_SHADER = 0,
                    RENDERING_PHASE_LIGHTING_SHADER = 1,
                    RENDERING_PHASE_TEXTURED_SHADER = 2
                };
                enum RendererType{
                    RENDERER_BASIC = 0,
                    RENDERER_MULTIPASS = 1,
                    RENDERER_DEFERRED = 2
                };
                enum BlendingMode{
                    BLEND_OFF = 0,
                    BLEND_ALPHA = 1,
                    BLEND_COLOR = 2,
                    BLEND_ADDITIVE = 3,
                    BLEND_ALPHA_PARTICLE = 4,
                    BLEND_COLOR_PARTICLE = 5,
                    BLEND_ONE_ONE_MINUS_SRC_ALPHA = 6
                };
                enum CullingMode{
                    CULL_OFF = 0,
                    CULL_FRONT = 1,
                    CULL_BACK = 2
                };
                enum CullWinding{
                    CULL_CW = 0,
                    CULL_CCW = 1
                };

                RenderingMaterial(void);
                ~RenderingMaterial(void);

                void SetLit(bool _lit);
                void SetTranslucent(bool _translucent);
                void SetCastsShadows(bool _casts_shadows);
                void SetAmbient(float _ambient);
                void SetDiffuse(float _diffuse);
                void SetSpecular(float _specular);
                void SetSpecularExponent(float _specular_exponent);
                void SetColor(glm::vec4 _color);
                void SetDepthMask(bool _depth_mask = true);
                void SetDepthTest(bool _depth_test = true);
                void SetCullWinding(EG::Graphics::RenderingMaterial::CullWinding _cull_winding);
                void SetTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type, std::string texture_id);
                void SetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type, std::string texture_id);
                void SetShaderOverride(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase, std::string shader_id);

                bool GetLit(void);
                bool GetTranslucent(void);
                bool GetCastsShadows(void);
                float GetAmbient(void);
                float GetDiffuse(void);
                float GetSpecular(void);
                float GetSpecularExponent(void);
                glm::vec4 GetColor(void);
                bool GetDepthMask(void);
                bool GetDepthTest(void);
                EG::Graphics::RenderingMaterial::CullWinding GetCullWinding(void);
                std::string GetTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type);
                std::string GetCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type);
                bool HasShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase);
                std::string GetShader(EG::Graphics::RenderingMaterial::RendererType renderer_type, EG::Graphics::RenderingMaterial::RenderingPhaseShaderType rendering_phase);

                bool HasTexture(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type);
                bool HasCubeMap(EG::Graphics::RenderingMaterial::RenderingMaterialTextureType type);

                void SetBlendingMode(EG::Graphics::RenderingMaterial::BlendingMode _blending_mode);
                EG::Graphics::RenderingMaterial::BlendingMode GetBlendingMode(void);
                void SetCullingMode(EG::Graphics::RenderingMaterial::CullingMode _culling_mode);
                EG::Graphics::RenderingMaterial::CullingMode GetCullingMode(void);
            private:
                bool lit, translucent, casts_shadows, depth_mask, depth_test;
                float ambient, diffuse, specular;
                float specular_exponent;
                EG::Graphics::RenderingMaterial::CullingMode culling_mode;
                EG::Graphics::RenderingMaterial::CullWinding cull_winding;
                glm::vec4 color;
                EG::Graphics::RenderingMaterial::BlendingMode blending_mode;
                std::unordered_map<EG::Graphics::RenderingMaterial::RenderingMaterialTextureType, std::string, std::hash<unsigned int> > textures;
                std::unordered_map<EG::Graphics::RenderingMaterial::RenderingMaterialTextureType, std::string, std::hash<unsigned int> > cube_maps;
                // TODO: Put shader info in here too so people can override the default shaders for the various techniquess
                std::unordered_map<EG::Graphics::RenderingMaterial::RendererType, std::unordered_map<EG::Graphics::RenderingMaterial::RenderingPhaseShaderType, std::string, std::hash<unsigned int> >, std::hash<unsigned int> > shaders;
        };
    }
}

#endif
