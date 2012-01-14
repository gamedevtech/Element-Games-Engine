#ifndef EG_GRAPHICS_RENDERER_H
#define EG_GRAPHICS_RENDERER_H

#include "Light.h"
#include "Camera.h"
#include "Particle.h"
#include "ShaderManager.h"
#include "GraphicsSubsystem.h"
#include "../Game/Scene.h"
#include "../Utility/Font.h"
#include "../Utility/RocketInterface.h"

namespace EG{
    namespace Graphics{
        class TestEmitter : public EG::Graphics::ParticleEmitter{
            public:
                TestEmitter(void);
                ~TestEmitter(void);
                void CreateParticle(EG::Graphics::Particle *p);
        };
        class TestController : public EG::Graphics::ParticleController{
            public:
                TestController(void);
                ~TestController(void);
                void ControlParticle(EG::Graphics::Particle *p, float frame_time);
        };

        class Renderer{
            public:
                Renderer(void);
                ~Renderer(void);

                void Initialize(void);
                void Render(EG::Game::Scene *scene, float frame_time);
                void RenderObject(EG::Game::Scene *scene, EG::Graphics::Light *light, EG::Game::Object *object);
                void RenderNonLitObject(EG::Game::Scene *scene, EG::Game::Object *object);

                // TEMP, SHOULD GO IN SCENE MANAGER WHICH SHOULD HOLD A CAMERA MANAGER
                EG::Graphics::Camera *GetCamera(void){
                    return camera;
                }
                EG::Graphics::ShaderManager *GetShaderManager(void){
                    return shaders;
                }

                void SetGUI(EG::Utility::RocketInterface *_gui);
            protected:
                // TEMP, SHOULD GO IN SCENE MANAGER WHICH SHOULD HOLD A CAMERA MANAGER
                EG::Graphics::Camera *camera;
                EG::Graphics::ShaderManager *shaders;
                glm::mat4 orthographics_projection_matrix;
                bool initialized;

                bool gui_set;
                EG::Utility::RocketInterface *gui;

                // Particles
                EG::Graphics::ParticleSystem *test_particles;
        };
    }
}

#endif
