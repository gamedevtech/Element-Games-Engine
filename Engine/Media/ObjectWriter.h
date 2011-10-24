#ifndef EG_MEDIA_OBJECT_WRITER_H
#define EG_MEDIA_OBJECT_WRITER_H

#include "../Game/Object.h"
#include "../Game/Scene.h"

namespace EG{
	namespace Media{
		class ObjectWriter{
			public:
				ObjectWriter(EG::Game::Object *_object, EG::Game::Scene *_scene, std::string _object_name);
				~ObjectWriter(void);

				void Write(std::string file_name);
			private:
				EG::Game::Object *object;
				EG::Game::Scene *scene;
				std::string object_name;
		};
	}
}

#endif
