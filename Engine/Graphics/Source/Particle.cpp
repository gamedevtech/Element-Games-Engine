#include "../Particle.h"

namespace EG{
	namespace Graphics{
		Particle::Particle(void) : EG::Game::Object(){
			alive = true;
		}
		Particle::~Particle(void){
			//
		}
        bool Particle::Alive(void){
            return alive;
        }

		ParticleController::ParticleController(void){
			//
		}
		ParticleController::~ParticleController(void){
			//
		}
		void ParticleController::ControlParticle(Particle *particle){
            //
        }

		ParticleEmitter::ParticleEmitter(Particle *_particle_template, float _emission_velocity){
            particle_template = _particle_template;
            emission_velocity = _emission_velocity;
			time_counter = 0.0f;
            time_between_particles = 1 / emission_velocity;
		}
		ParticleEmitter::~ParticleEmitter(void){
			//
		}
		void ParticleEmitter::Emit(std::list<Particle *> *particles, float frame_time){
            time_counter += frame_time;
            int particle_count = int(time_counter / time_between_particles);
            time_counter -= particle_count * time_between_particles;

            for (int i = 0; i < particle_count; i++){
                Particle *particle = CreateParticle();
                particles->push_back(particle);
            }
		}
		Particle *ParticleEmitter::CreateParticle(void){
            Particle *particle = new Particle();
            return particle;
        }

		ParticleSystem::ParticleSystem(ParticleController *_controller, ParticleEmitter *_emitter){
			controller = _controller;
            emitter = _emitter;
		}
		ParticleSystem::~ParticleSystem(void){
            particles.clear();
            delete controller;
            delete emitter;
		}
		void ParticleSystem::Update(float frame_time){
			emitter->Emit(&particles, frame_time);
            std::list<Particle *>::iterator piter = particles.begin();
            while (piter != particles.end()){
                Particle *p = (*piter);
                controller->ControlParticle(p);

                // NOTE: Erase should advance the iterator for us
                // NOTE: If this doesn't work you have to store a temporary copy of the iter, delete that, and then increment, you can't delete the current iter and then increment!
                // http://stackoverflow.com/questions/596162/can-you-remove-elements-from-a-stdlist-while-iterating-through-it
                if (!p->Alive()){
                    particles.erase(piter);
                }else{
                    ++piter;
                }
            }
		}
		std::list<Particle *> *Particles ParticleSystem::GetParticles(void){
			return *particles;
		}
	}
}
