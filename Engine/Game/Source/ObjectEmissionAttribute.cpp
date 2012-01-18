#include "../ObjectEmissionAttribute.h"

namespace EG{
    namespace Game{
        ObjectAttributeEmissionLight::ObjectAttributeEmissionLight(EG::Graphics::Light *_light){
            type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT;
            light = _light;
        }

        ObjectAttributeEmissionLight::~ObjectAttributeEmissionLight(void){
            delete light;
        }

        void ObjectAttributeEmissionLight::SetLight(EG::Graphics::Light *_light){
            light = _light;
        }

        EG::Graphics::Light *ObjectAttributeEmissionLight::GetLight(void){
            return light;
        }

        ObjectAttributeEmissionParticleSystem::ObjectAttributeEmissionParticleSystem(EG::Graphics::ParticleSystem *_particle_system){
            type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_PARTICLE_SYSTEM;
            particle_system = _particle_system;
        }

        ObjectAttributeEmissionParticleSystem::~ObjectAttributeEmissionParticleSystem(void){
            delete particle_system;
        }

        void ObjectAttributeEmissionParticleSystem::SetParticleSystem(EG::Graphics::ParticleSystem *_particle_system){
            particle_system = _particle_system;
        }

        EG::Graphics::ParticleSystem *ObjectAttributeEmissionParticleSystem::GetParticleSystem(void){
            return particle_system;
        }
    }
}
