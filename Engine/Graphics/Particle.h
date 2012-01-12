#ifndef EG_GRAPHICS_PARTICLE_H
#define EG_GRAPHICS_PARTICLE_H

#include <list>
#include "../Math/Math.h"
#include "../Game/Object.h"

namespace EG{
	namespace Graphics{
		class Particle : public EG::Game::Object{
			public:
				Particle(void);
				~Particle(void);

                bool Alive(void);
			protected:
				bool alive;
		};

		class ParticleController{
			public:
				ParticleController(void);
				~ParticleController(void);

				void ControlParticle(Particle *particle);
			protected:
				//
		};

		class ParticleEmitter{
			public:
				ParticleEmitter(Particle *_particle_template, float _emission_velocity);
				~ParticleEmitter(void);

				void Emit(std::list<Particle *> *particles, float frame_time);
                Particle *CreateParticle(void);
			protected:
                Particle *particle_template;
				float emission_velocity;
				glm::vec3 initial_velocity;
				glm::vec3 initial_velocity_mod;

                float time_counter;
                float time_between_particles;
		};

		class ParticleSystem{
			public:
				ParticleSystem(ParticleController *_controller, ParticleEmitter *_emitter);
				~ParticleSystem(void);

				void Update(float frame_time);
			protected:
				ParticleController *controller;
				ParticleEmitter *emitter;
				std::list<Particle *> particles;

                // Object add list
                // Object del list
		};
	}
}

#endif
