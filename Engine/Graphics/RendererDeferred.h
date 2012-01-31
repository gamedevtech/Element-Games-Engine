#ifndef EG_GRAPHICS_RENDERER_DEFERRED_H
#define EG_GRAPHICS_RENDERER_DEFERRED_H

#include "Renderer.h"
#include "OffscreenBuffer.h"
#include "Light.h"

namespace EG{
    namespace Graphics{
        class RendererDeferred : public Renderer{
            public:
            enum DeferredOutputType{
                DEFERRED_OUTPUT_NORMAL, DEFERRED_OUTPUT_PREPASS, DEFERRED_OUTPUT_BLOOM, DEFERRED_OUTPUT_SSAO, DEFERRED_OUTPUT_HDR, DEFERRED_OUTPUT_SHADOW
            };
                RendererDeferred(void);
                ~RendererDeferred(void);

                void Initialize(void);
                void Render(EG::Game::Scene *scene, EG::Utility::Time *time);
                void CycleOutputType(void);
                void ToggleSSAO(void);
                void ToggleBloom(void);
                void ToggleNormalMapping(void);
                void ToggleShadowMapping(void);
                void ToggleDOF(void);
            private:
                EG::Graphics::OffscreenBuffer *deferred_buffer;
                EG::Graphics::OffscreenBuffer *light_buffer;
                EG::Graphics::OffscreenBuffer *bloom_buffer;
                EG::Graphics::OffscreenBuffer *hdr_buffer;
                EG::Graphics::OffscreenBuffer *ssao_buffer;
                EG::Graphics::OffscreenBuffer *composition_buffer;
                glm::mat4 orthographics_projection_matrix;
                EG::Graphics::Mesh *rectangle;
                EG::Graphics::Mesh *light_sphere;

                void Bloom(void);
                void SSAO(void);
                void ShadowMapping(EG::Game::Scene *scene);

                void RenderObject(EG::Game::Scene *scene, EG::Game::Object *object);
                void CalculateLighting(EG::Game::Scene *scene, EG::Graphics::Light *light);
                void Prepass(EG::Game::Scene *scene);
                void Lighting(EG::Game::Scene *scene);
                void ComposeScene(EG::Game::Scene *scene);
                void Overlays(EG::Game::Scene *scene, EG::Utility::Time *time);

                // Settings
                DeferredOutputType output_type;
                float frame_time;
                unsigned int ssao_enabled, bloom_enabled, normal_mapping_enabled, shadows_enabled, dof_enabled;
                unsigned int luminance_buffer_sample_min_x;
                unsigned int luminance_buffer_sample_min_y;
                unsigned int luminance_buffer_sample_max_x;
                unsigned int luminance_buffer_sample_max_y;
                float luminance_buffer[1024];
                float luminance;
                float bloom_scale;
                float luminance_scale;
                float target_luminance;
                float luminance_time_multiplier;
                float shadow_mapping_offset_0;
                float shadow_mapping_offset_1;
                glm::mat4 shadow_mapping_bias;

                // Debugging
                EG::Graphics::Light *debug_shadow_map_light;
        };
    }
}

#endif
