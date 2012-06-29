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
            glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), scale);
            std::cout << "Scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;
            EG::Math::Utility::PrintMat4(scaling_matrix);

            glm::mat4 rotation_matrix = glm::toMat4(rotation);
            std::cout << "Rotation: " << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << std::endl;
            EG::Math::Utility::PrintMat4(rotation_matrix);

            glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), translate);
            std::cout << "Translate: " << translate.x << ", " << translate.y << ", " << translate.z << std::endl;
            EG::Math::Utility::PrintMat4(translation_matrix);

            std::cout << "Result: " << std::endl;
            glm::mat4 out = scaling_matrix * rotation_matrix * translation_matrix;
            EG::Math::Utility::PrintMat4(out);

            return out;
        }
    }
}
