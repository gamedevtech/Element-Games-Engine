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
            glm::vec4 bmin = model * glm::vec4(min.x, min.y, min.z, 1.0f);
            glm::vec4 bmax = model * glm::vec4(max.x, max.y, max.z, 1.0f);

            glm::vec3 t1, t2;
            double t_near = -DBL_MAX; // maximums defined in float.h
            double t_far = DBL_MAX;

            for (int i = 0; i < 3; i++){ //we test slabs in every direction
                if (dir[i] == 0){ // ray parallel to planes in this direction
                    if ((begin[i] < bmin[i]) || (begin[i] > bmax[i])) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    t1[i] = (bmin[i] - begin[i]) / dir[i];
                    t2[i] = (bmax[i] - begin[i]) / dir[i];

                    if(t1[i] > t2[i]){ // we want t1 to hold values for intersection with near plane
                        glm::vec3 tmp = t2;
                        t2 = t1;
                        t1 = tmp;
                    }
                    if (t1[i] > t_near){
                        t_near = t1[i];
                    }
                    if (t2[i] < t_far){
                        t_far = t2[i];
                    }
                    if( (t_near > t_far) || (t_far < 0) ){
                        return false;
                    }
                }
            }
            //tnear = t_near; tfar = t_far; // put return values in place
            return true; // if we made it here, there was an intersection - YAY
        }

        glm::vec3 Utility::ProjectClick(glm::vec2 viewport_dimensions, glm::vec2 mouse_position, float far, glm::vec3 camera_position, glm::mat4 view_matrix, glm::mat4 projection_matrix) {
            glm::vec4 viewport = glm::vec4(0.0f, 0.0f, viewport_dimensions.x, viewport_dimensions.y);
            glm::vec3 wincoord = glm::vec3(mouse_position.x, viewport_dimensions.y - mouse_position.y, -far);
            glm::vec3 dir = glm::normalize(glm::unProject(wincoord, view_matrix, projection_matrix, viewport) - camera_position);
            return dir;
        }
    }
}
