#ifndef EG_GRAPHICS_PARTICLE_H
#define EG_GRAPHICS_PARTICLE_H

#include <list>
#include "../Utility/StringDictionary.h"
#include "../Math/Math.h"
#include "../Game/Object.h"

namespace EG{
    namespace Graphics{
        class Particle : public EG::Game::Object{
            public:
                Particle(void);
                ~Particle(void);

                void SetAlive(bool _alive);
                bool Alive(void);
                void SetAttribute(std::string key, float value);
                float GetAttribute(std::string key);
            protected:
                bool alive;
                EG::Utility::StringDictionary<float> attributes;
        };

        class ParticleController{
            public:
                ParticleController(void);
                ~ParticleController(void);

                virtual void ControlParticle(Particle *particle, float frame_time) = 0;
            protected:
                //
        };

        class ParticleEmitter{
            public:
                ParticleEmitter(float _emission_velocity);
                ~ParticleEmitter(void);

                virtual void Emit(std::list<Particle *> *particles, float frame_time);
                virtual void CreateParticle(Particle *out) = 0;
            protected:
                float emission_velocity;
                glm::vec3 initial_velocity;
                glm::vec3 initial_velocity_mod;

                float time_counter;
                float time_between_particles;
        };

        // TODO: Initial Particle Spawn
        class ParticleSystem{
            public:
                ParticleSystem(ParticleController *_controller, ParticleEmitter *_emitter);
                ~ParticleSystem(void);

                void Update(float frame_time);
                std::list<Particle *> *GetParticles(void);
            protected:
                ParticleController *controller;
                ParticleEmitter *emitter;
                std::list<Particle *> particles;
        };
    }
}

#endif
