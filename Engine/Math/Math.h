#ifndef EG_MATH_H
#define EG_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// NOTE: Remember... Rotate then Translate then Scale!

namespace EG{
	namespace Math{
		class Utility{
			public:
				Utility(void);
				~Utility(void);

				static float AbsoluteValue(float in);
				static void PrintMat4(glm::mat4 in);
				static glm::mat4 GenerateNormalMatrix(glm::mat4 in);
				static unsigned int NextPowerOfTwo(unsigned int in);
			private:
				//
		};
	}
}

#endif
