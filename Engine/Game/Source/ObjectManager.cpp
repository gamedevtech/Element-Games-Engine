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
            if (object->GetObjectName() != "" && objects_by_name.count(object->GetObjectName())){
                return false;
            }

            if (object->GetObjectName() != ""){
                objects_by_name[object->GetObjectName()] = object;
            }

            unsigned int object_id = 0;
            bool found = false;
            while (!found){
                object_id += 1;
                if (!(objects_by_id.count(object_id))){
                    found = true;
                }
            }

            object->SetObjectId(object_id);
            objects_by_id[object_id] = object;

            return true;
        }

        unsigned int ObjectManager::GetObjectCount(void){
            return objects_by_id.size();
        }

        EG::Game::Object *ObjectManager::GetObjectByName(std::string object_name){
            if (objects_by_name.count(object_name)) {
                return objects_by_name[object_name];
            }
            return nullptr;
        }

        EG::Game::Object *ObjectManager::GetObjectById(unsigned int object_id){
            if (objects_by_id.count(object_id)) {
                return objects_by_id[object_id];
            }
            return nullptr;
        }

        std::unordered_map<unsigned int, EG::Game::Object *> *ObjectManager::GetObjects(void){
            return &objects_by_id;
        }
    }
}
