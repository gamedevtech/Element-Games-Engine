#ifndef EG_GRAPHICS_PARTICLE_H
#define EG_GRAPHICS_PARTICLE_H

#include "../Game/Object.h"

namespace EG{
	namespace Graphics{
		class Particle : public Object{
			public:
				Particle(void);
				~Particle(void);
			private:
				//
		};

        class ParticleSystem{
            public:
                ParticleSystem(void);
                ~ParticleSystem(void);
            private:
                //
        };
	}
}

#endif
