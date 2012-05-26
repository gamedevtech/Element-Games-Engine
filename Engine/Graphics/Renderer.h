#ifndef EG_GRAPHICS_RENDERER_H
#define EG_GRAPHICS_RENDERER_H

#include "Light.h"
#include "Camera.h"
#include "Particle.h"
#include "ShaderManager.h"
#include "GraphicsSubsystem.h"
#include "RenderingMaterial.h"
#include "../Game/Scene.h"
#include "../Utility/Font.h"
#include "../Utility/GUI.h"
#include "../Utility/Time.h"

namespace EG{
    namespace Graphics{
        class Renderer{
            public:
                Renderer(void);
                ~Renderer(void);

                void Initialize(void);
                void Render(EG::Game::Scene *scene, EG::Utility::Time *time);
                void RenderLitObject(EG::Game::Scene *scene, EG::Graphics::Light *light, EG::Game::Object *object);
                void RenderObject(EG::Game::Scene *scene, EG::Game::Object *object);

                // TEMP, SHOULD GO IN SCENE MANAGER WHICH SHOULD HOLD A CAMERA MANAGER
                EG::Graphics::ShaderManager *GetShaderManager(void){
                    return shaders;
                }
            protected:
                bool initialized;

                std::string current_shader_id;
                void BindCustomShaderUniforms(EG::Game::Object *object, std::string shader_id);
                void BindShaderBeginUniforms(std::string shader_id, EG::Game::Scene *scene, EG::Graphics::Light *light);
                void BindEngineShaderUniforms(EG::Game::Object *object, std::string shader_id, std::string phase, glm::mat4 transform, EG::Graphics::RenderingMaterial *material, EG::Game::Scene *scene);

                // Settings, Not used here though, but in inherited renderers, like the deferred renderer.
                unsigned int ssao_enabled, bloom_enabled, normal_mapping_enabled, shadows_enabled, dof_enabled;
                glm::mat4 orthographics_projection_matrix;
                glm::mat4 shadow_mapping_bias;
                float luminance_scale;
                float bloom_scale;
                float luminance;

                // TEMP, SHOULD GO IN SCENE MANAGER WHICH SHOULD HOLD A CAMERA MANAGER
                EG::Graphics::ShaderManager *shaders;
        };
    }
}

#endif
