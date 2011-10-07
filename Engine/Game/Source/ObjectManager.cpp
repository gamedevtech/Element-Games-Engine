#include "../ObjectManager.h"

namespace EG{
	namespace Game{
		ObjectManager::ObjectManager(void){
			//
		}

		ObjectManager::~ObjectManager(void){
			//
		}

		bool ObjectManager::AddObject(EG::Game::Object *object){
			if (objects_by_name.count(object->GetObjectName()) > 0){
				return false;
			}

			objects_by_name[object->GetObjectName()] = object;

			unsigned int object_id = 0;
			bool found = false;
			while (!found){
				object_id += 1;
				if (objects_by_id.count(object_id) <= 0){
					found = true;
				}
			}

			object->SetObjectId(object_id);
			objects_by_id[object->GetObjectId()] = object;

			return true;
		}

		EG::Game::Object *ObjectManager::GetObject(std::string object_name){
			if (objects_by_name.count(object_name) > 0){
				return objects_by_name[object_name];
			}
			return NULL;
		}

		EG::Game::Object *ObjectManager::GetObject(unsigned int object_id){
			if (objects_by_id.count(object_id) > 0){
				return objects_by_id[object_id];
			}
			return NULL;
		}

		std::map<std::string, EG::Game::Object *> *ObjectManager::GetObjects(void){
			return &objects_by_name;
		}
	}
}
