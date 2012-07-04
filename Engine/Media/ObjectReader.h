#ifndef EG_MEDIA_OBJECT_READER_H
#define EG_MEDIA_OBJECT_READER_H

#include "../Game/Object.h"
#include "../Game/Scene.h"
#include "../Utility/StringMethods.h"

#include <fstream>

namespace EG{
    namespace Media{
        class ObjectReader{
            public:
                ObjectReader(void);
                ~ObjectReader(void);

                bool Read(std::string file_path, EG::Game::Scene *scene);
                EG::Game::Object *GetLoadedObject(void);
            private:
                std::ifstream in;
                EG::Game::Object *object;
                char *characters;

                std::string ReadString(unsigned int size);
                bool ReadBool(void);
                unsigned int ReadUInt(void);
                float ReadFloat(void);
                float *ReadFloatV(unsigned int size);
                glm::vec3 ReadVec3(void);
                glm::vec4 ReadVec4(void);
                glm::quat ReadQuat(void);
                glm::mat4 ReadMat4(void);
        };
    }
}

#endif
