#ifndef EG_MEDIA_OBJECT_WRITER_H
#define EG_MEDIA_OBJECT_WRITER_H

#include "../Game/Object.h"
#include "../Game/Scene.h"

#include <fstream>

namespace EG{
    namespace Media{
        class ObjectWriter{
            public:
                ObjectWriter(EG::Game::Object *_object, EG::Game::Scene *_scene, std::string _object_name, std::string _images_output_path, std::string _model_output_path);
                ~ObjectWriter(void);

                void Write(std::string file_name);
            private:
                std::ofstream out;
                void CopyFile(std::string in, std::string out);
                EG::Game::Object *object;
                EG::Game::Scene *scene;
                std::string object_name, images_output_path, model_output_path;

                void WriteString(std::string value);
                void WriteBool(bool value);
                void WriteUInt(unsigned int value);
                void WriteFloat(float value);
                void WriteFloatV(float *value, unsigned int count);
                void WriteVec4(glm::vec4 value);
                void WriteMat4(glm::mat4 value);
        };
    }
}

#endif
