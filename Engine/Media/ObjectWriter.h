#ifndef EG_MEDIA_OBJECT_WRITER_H
#define EG_MEDIA_OBJECT_WRITER_H

#include "../Game/Object.h"
#include "../Game/Scene.h"

namespace EG{
	namespace Media{
		class ObjectWriter{
			public:
				ObjectWriter(EG::Game::Object *_object, EG::Game::Scene *_scene, std::string _object_name, std::string _images_output_path, std::string _model_output_path);
				~ObjectWriter(void);

				void Write(std::string file_name);
			private:
				void CopyFile(std::string in, std::string out);
				EG::Game::Object *object;
				EG::Game::Scene *scene;
				std::string object_name, images_output_path, model_output_path;
		};
	}
}

#endif
