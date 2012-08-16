#ifndef EG_MATH_H
#define EG_MATH_H

#include <glm/glm.hpp>

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_integer.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
// #include <glm/gtx/matrix_interpolation.hpp>

// NOTE: Remember... Rotate then Translate then Scale!

namespace EG{
    namespace Math{
        class Utility{
            public:
                Utility(void);
                ~Utility(void);

                static float AbsoluteValue(float in);
                static int AbsoluteValue(int in);
                static void PrintMat4(glm::mat4 in);
                static glm::mat4 GenerateNormalMatrix(glm::mat4 in);
                static unsigned int NextPowerOfTwo(unsigned int in);
                static unsigned int RandomUnsigned(unsigned int max);
                static int RandomInteger(int min, int max);
                static float RandomFloat(float min, float max);
                static glm::mat4 GenerateTransform(glm::vec3 translate = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::quat rotation = glm::quat(glm::vec3(0.0f)));
                static bool RayAABBTest(glm::vec3 begin, glm::vec3 dir, glm::vec3 min, glm::vec3 max, glm::mat4 model = glm::mat4(1.0f));
                static glm::vec3 ProjectClick(glm::vec2 viewport_dimensions, glm::vec2 mouse_position, float far, glm::vec3 camera_position, glm::mat4 view_matrix, glm::mat4 projection_matrix);
                static glm::vec3 GetScalingFromMatrix(glm::mat4 in);
            private:
                //
        };
    }
}

#endif
