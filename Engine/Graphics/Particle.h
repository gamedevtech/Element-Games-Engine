#ifndef EG_GRAPHICS_PARTICLE_H
#define EG_GRAPHICS_PARTICLE_H

#include <list>
#include "../Math/Math.h"
#include "../Game/Object.h"

namespace EG{
	namespace Graphics{
		class Particle : public EG::Game::Object{
			public:
				Particle(std::string name = "");
				~Particle(void);
			protected:
				// particle status
		};

		class ParticleController{
			public:
				ParticleController(void);
				~ParticleController(void);

				virtual void ControlParticle(Particle *particle) = 0;
			protected:
				//
		};

		class ParticleEmitter{
			public:
				ParticleEmitter(void);
				~ParticleEmitter(void);

				void Emit(void);
			protected:
				float emission_velicity;
				glm::vec3 initial_velocity;
				glm::vec3 initial_velocity_mod;
		};

		class ParticleSystem{
			public:
				ParticleSystem(ParticleController *_controller, ParticleEmitter *_emitter);
				~ParticleSystem(void);

				void Update(void);
			protected:
				ParticleController *controller;
				ParticleEmitter *emitter;
				std::list<Particle *> particles;
		};
	}
}

#endif
