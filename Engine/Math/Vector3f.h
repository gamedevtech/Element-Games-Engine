#ifndef EG_MATH_VECTOR3F_H
#define EG_MATH_VECTOR3F_H

#include <glm/glm.hpp>

namespace EG{
	namespace Math{
		class Vector3f{
			public:
				Vector3f(void);
				Vector3f(float x, float y, float z);
				Vector3f(float *data);
			private:
				glm::vec3 glm_data;
		};
	}
}

#endif
