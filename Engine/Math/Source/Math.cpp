#include "../Math.h"
#include <time.h>
#include <iostream>


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
            glm::mat4 translation_matrix = glm::translate(translate);
            glm::mat4 scaling_matrix = glm::scale(scale);
            glm::mat4 rotation_matrix = glm::toMat4(rotation);
            return rotation_matrix * translation_matrix * scaling_matrix;
        }

        glm::mat4 Utility::Interpolate(glm::mat4 begin, glm::mat4 end, float factor) {
            glm::vec3 pos_begin = glm::vec3(begin[3][0], begin[3][1], begin[3][2]);
            glm::vec3 pos_end = glm::vec3(end[3][0], end[3][1], end[3][2]);
            glm::vec3 pos_result = glm::mix(pos_begin, pos_end, factor);

            glm::vec3 scale_begin = glm::vec3(
                sqrt(begin[0][0] * begin[0][0] + begin[1][0] * begin[1][0] + begin[2][0] * begin[2][0]),
                sqrt(begin[0][1] * begin[0][1] + begin[1][1] * begin[1][1] + begin[2][1] * begin[2][1]),
                sqrt(begin[0][2] * begin[0][2] + begin[1][2] * begin[1][2] + begin[2][2] * begin[2][2])
            );
            glm::vec3 scale_end = glm::vec3(
                sqrt(end[0][0] * end[0][0] + end[1][0] * end[1][0] + end[2][0] * end[2][0]),
                sqrt(end[0][1] * end[0][1] + end[1][1] * end[1][1] + end[2][1] * end[2][1]),
                sqrt(end[0][2] * end[0][2] + end[1][2] * end[1][2] + end[2][2] * end[2][2])
            );
            glm::vec3 scale_result = glm::mix(scale_begin, scale_end, factor);

            glm::quat rot_begin = glm::toQuat(begin);
            glm::quat rot_end = glm::toQuat(end);
            glm::quat rot_result = glm::mix(rot_begin, rot_end, factor);

            glm::mat4 result = GenerateTransform(pos_result, scale_result, rot_result);
            //PrintMat4(result);
            return result;
        }
    }
}
