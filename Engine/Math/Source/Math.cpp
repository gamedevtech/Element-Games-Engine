#include "../Math.h"

#include <time.h>
#include <iostream>

#include "../../Utility/Console.h"

namespace EG{
    namespace Math{
        Utility::Utility(void){
            srand(time(NULL));
        }

        Utility::~Utility(void){
            //
        }

        float Utility::AbsoluteValue(float in){
            if (in < 0.0f){
                return -in;
            }
            return in;
        }

        int Utility::AbsoluteValue(int in){
            if (in < 0){
                return -in;
            }
            return in;
        }

        void Utility::PrintMat4(glm::mat4 in){
            for (unsigned int i = 0; i < 4; i++){
                for (unsigned int j = 0; j < 4; j++){
                    std::cout << in[j][i] << ' ';
                }
                std::cout << std::endl;
            }
        }

        glm::mat4 Utility::GenerateNormalMatrix(glm::mat4 in){
            glm::mat4 out = glm::inverse(in);
            return glm::transpose(out);
        }

        unsigned int Utility::NextPowerOfTwo(unsigned int in){
            unsigned int rval = 1;
            while(rval < in){
                rval<<=1;
            }
            return rval;
        }

        unsigned int Utility::RandomUnsigned(unsigned int max){
            return rand() % (max + 1);
        }

        int Utility::RandomInteger(int min, int max){
            return rand() % (max - min + 1) + min;
        }

        float Utility::RandomFloat(float min, float max){
            return ((max - min) * ((float)rand() / RAND_MAX)) + min;
        }

        glm::mat4 Utility::GenerateTransform(glm::vec3 translate, glm::vec3 scale, glm::quat rotation){
            glm::mat4 out = glm::toMat4(rotation);
            out[3][0] = translate.x;
            out[3][1] = translate.y;
            out[3][2] = translate.z;
            out = glm::scale(out, scale);
            return out;
        }

        bool Utility::RayAABBTest(glm::vec3 begin, glm::vec3 dir, glm::vec3 min, glm::vec3 max, glm::mat4 model) {
            dir = glm::normalize(dir);

            glm::vec3 dirfrac = glm::vec3(1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z);

            glm::vec4 bmin = model * glm::vec4(min.x, min.y, min.z, 1.0f);
            glm::vec4 bmax = model * glm::vec4(max.x, max.y, max.z, 1.0f);

            float t1 = (bmin.x - begin.x) * dirfrac.x;
            float t2 = (bmax.x - begin.x) * dirfrac.x;
            float t3 = (bmin.y - begin.y) * dirfrac.y;
            float t4 = (bmax.y - begin.y) * dirfrac.y;
            float t5 = (bmin.z - begin.z) * dirfrac.z;
            float t6 = (bmax.z - begin.z) * dirfrac.z;

            float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
            float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

            // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
            if (tmax < 0) {
                return false;
            }

            // if tmin > tmax, ray doesn't intersect AABB
            if (tmin > tmax) {
                return false;
            }

            return true;
        }

        glm::vec3 Utility::ProjectClick(glm::vec2 mp, glm::vec2 dims, glm::vec3 camera_position, glm::mat4 inverse_view, glm::mat4 inverse_projection) {
            glm::vec4 mouse_clip = glm::vec4(mp.x * 2.0f / dims.x - 1.0f, 1.0f - mp.y * 2.0f / dims.y, 0.0f, 1.0f);
            glm::vec4 mouse_world = inverse_view * inverse_projection * mouse_clip;
            glm::vec3 mouse = glm::normalize(glm::vec3(mouse_world.x, mouse_world.y, mouse_world.z));
            glm::vec3 out = glm::normalize(mouse - camera_position);
            console->Print(mouse_clip, "Mouse Click: ");
            console->Print(mouse, "Mouse World: ");
            console->Print(camera_position, "Cam Pos: ");
            console->Print(out, "Dir: ");
            return out;
        }
    }
}
