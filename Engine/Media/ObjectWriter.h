#ifndef EG_MEDIA_OBJECT_WRITER_H
#define EG_MEDIA_OBJECT_WRITER_H

#include "../Game/Object.h"

namespace EG{
	namespace Media{
		class ObjectWriter{
			public:
				ObjectWriter(EG::Game::Object *_sobject);
				~ObjectWriter(void);

				void Write(std::string file_name);
			private:
				EG::Game::Object *object;
		};
	}
}

#endif
