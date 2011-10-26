#ifndef EG_MEDIA_OBJECT_READER_H
#define EG_MEDIA_OBJECT_READER_H

#include "../Game/Object.h"
#include "../Game/Scene.h"
#include "../Utility/StringMethods.h"

namespace EG{
	namespace Media{
		class ObjectReader{
			public:
				ObjectReader(void);
				~ObjectReader(void);

				bool Read(std::string file_path, EG::Game::Scene *scene);
				EG::Game::Object *GetObject(void);
			private:
				EG::Game::Object *object;
		};
	}
}

#endif

