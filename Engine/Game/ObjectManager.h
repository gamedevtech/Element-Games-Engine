#ifndef EG_GAME_OBJECT_MANAGER_H
#define EG_GAME_OBJECT_MANAGER_H

//#include <map>
#include "../Utility/UnsignedIntDictionary.h"
#include "../Utility/StringDictionary.h"
#include <string>

#include "Object.h"

namespace EG{
	namespace Game{
		class ObjectManager{
		public:
			ObjectManager(void);
			~ObjectManager(void);

			bool AddObject(EG::Game::Object *object);
			EG::Game::Object *GetObject(std::string object_name);
			EG::Game::Object *GetObject(unsigned int object_id);
			EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *GetObjects(void);
		private:
			EG::Utility::UnsignedIntDictionary<EG::Game::Object *> objects_by_id;
			EG::Utility::StringDictionary<EG::Game::Object *> objects_by_name;
		};
	}
}

#endif
