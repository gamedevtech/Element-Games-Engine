#include "../Particle.h"

namespace EG{
    namespace Graphics{
        Particle::Particle(void) : EG::Game::Object(){
            alive = true;
            distance_from_camera = 0.0f;
        }
        Particle::~Particle(void){
            attributes.clear();
        }
        void Particle::SetAlive(bool _alive){
            alive = _alive;
        }
        bool Particle::Alive(void){
            return alive;
        }
        void Particle::SetDistanceFromCamera(float _distance_from_camera){
            distance_from_camera = _distance_from_camera;
        }
        float Particle::GetDistanceFromCamera(void){
            return distance_from_camera;
        }
        void Particle::SetAttribute(std::string key, float value){
            attributes[key] = value;
        }
        float Particle::GetAttribute(std::string key){
            if (attributes.count(key)){
                return attributes[key];
            }
            return 0.0f;
        }

        ParticleController::ParticleController(void){
            //
        }
        ParticleController::~ParticleController(void){
            //
        }

        ParticleEmitter::ParticleEmitter(float _emission_velocity){
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
                Particle *particle = new Particle();
                CreateParticle(particle);
                particles->push_back(particle);
            }
        }

        bool compare_distance(Particle *first, Particle *second){
            if (first->GetDistanceFromCamera() > second->GetDistanceFromCamera()){
                return true;
            }
            return false;
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

                // NOTE: Erase should advance the iterator for us
                // NOTE: If this doesn't work you have to store a temporary copy of the iter, delete that, and then increment, you can't delete the current iter and then increment!
                // http://stackoverflow.com/questions/596162/can-you-remove-elements-from-a-stdlist-while-iterating-through-it
                if (!(p->Alive())){
                    piter = particles.erase(piter);
                    delete p;
                    // NOTE: Not Detaching Physics!
                }else{
                    controller->ControlParticle(p, frame_time);
                    ++piter;
                }
            }
            // TODO : Sort particles here by distance!
            particles.sort(compare_distance);
        }
        std::list<Particle *> *ParticleSystem::GetParticles(void){
            return &particles;
        }
    }
}
