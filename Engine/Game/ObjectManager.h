#ifndef EG_GAME_OBJECT_MANAGER_H
#define EG_GAME_OBJECT_MANAGER_H

#include <unordered_map>
#include <string>

#include "Object.h"

namespace EG{
    namespace Game{
        class ObjectManager{
        public:
            ObjectManager(void);
            ~ObjectManager(void);

            bool AddObject(EG::Game::Object *object);
            EG::Game::Object *GetObjectByName(std::string object_name);
            EG::Game::Object *GetObjectById(unsigned int object_id);
            unsigned int GetObjectCount(void);
            std::unordered_map<unsigned int, EG::Game::Object *> *GetObjects(void);
        private:
            std::unordered_map<unsigned int, EG::Game::Object *> objects_by_id;
            std::unordered_map<std::string, EG::Game::Object *> objects_by_name;
        };
    }
}

#endif
