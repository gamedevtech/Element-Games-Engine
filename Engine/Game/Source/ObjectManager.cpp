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
            if (object->GetObjectName() != "" && objects_by_name.Has(object->GetObjectName())){
                return false;
            }

            if (object->GetObjectName() != ""){
                objects_by_name.Set(object->GetObjectName(), object);
            }

            unsigned int object_id = 0;
            bool found = false;
            while (!found){
                object_id += 1;
                if (!(objects_by_id.Has(object_id))){
                    found = true;
                }
            }

            object->SetObjectId(object_id);
            objects_by_id.Set(object->GetObjectId(), object);

            return true;
        }

        EG::Game::Object *ObjectManager::GetObjectByName(std::string object_name){
            EG::Game::Object *out = objects_by_name.Get(object_name);
            if (out != NULL){
                return out;
            }
            return NULL;
        }

        EG::Game::Object *ObjectManager::GetObjectById(unsigned int object_id){
            EG::Game::Object *out = objects_by_id.Get(object_id);
            if (out != NULL){
                return out;
            }
            return NULL;
        }

        EG::Utility::UnsignedIntDictionary<EG::Game::Object *> *ObjectManager::GetObjects(void){
            return &objects_by_id;
        }
    }
}
