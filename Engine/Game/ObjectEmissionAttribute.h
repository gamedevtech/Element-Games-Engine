#ifndef EG_GAME_OBJECT_EMISSION_ATTRIBUTE_H
#define EG_GAME_OBJECT_EMISSION_ATTRIBUTE_H

#include "../Graphics/Light.h"
#include "../Graphics/Particle.h"
#include "ObjectAttribute.h"

namespace EG{
    namespace Game{
        class ObjectAttributeEmissionLight : public ObjectAttribute{
            public:
                ObjectAttributeEmissionLight(EG::Graphics::Light *_light);
                ~ObjectAttributeEmissionLight(void);

                void SetLight(EG::Graphics::Light *_light);
                EG::Graphics::Light *GetLight(void);
            private:
                EG::Graphics::Light *light;
        };

        class ObjectAttributeEmissionParticleSystem : public ObjectAttribute{
            public:
                ObjectAttributeEmissionParticleSystem(EG::Graphics::ParticleSystem *_particle_system);
                ~ObjectAttributeEmissionParticleSystem(void);

                void SetParticleSystem(EG::Graphics::ParticleSystem *_particle_system);
                EG::Graphics::ParticleSystem *GetParticleSystem(void);
            private:
                EG::Graphics::ParticleSystem *particle_system;
        };
    }
}

#endif
