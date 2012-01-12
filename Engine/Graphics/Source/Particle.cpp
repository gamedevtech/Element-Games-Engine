#include "../Particle.h"

namespace EG{
	namespace Graphics{
		Particle::Particle(std::string name) : EG::Game::Object(name){
			//
		}
		Particle::~Particle(void){
			//
		}

		ParticleController::ParticleController(void){
			//
		}
		ParticleController::~ParticleController(void){
			//
		}

		ParticleEmitter::ParticleEmitter(void){
			//
		}
		ParticleEmitter::~ParticleEmitter(void){
			//
		}
		void ParticleEmitter::Emit(void){
			//
		}

		ParticleSystem::ParticleSystem(ParticleController *_controller, ParticleEmitter *_emitter){
			//
		}
		ParticleSystem::~ParticleSystem(void){
			//
		}
		void ParticleSystem::Update(void){
			//
		}
	}
}
