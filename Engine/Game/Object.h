#ifndef EG_GAME_OBJECT_H
#define EG_GAME_OBJECT_H

//#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

#include "ObjectAttribute.h"
#include "ObjectScript.h"

namespace EG{
    namespace Game{
        typedef std::vector<EG::Game::ObjectAttribute *> ObjectAttributeArray;
        typedef std::pair<EG::Game::ObjectAttribute::ObjectAttributeType, ObjectAttributeArray> ObjectAttributeArrayPair;
        typedef std::unordered_map<EG::Game::ObjectAttribute::ObjectAttributeType, ObjectAttributeArray, std::hash<unsigned int>> ObjectAttributes;
        class Object{
            public:
                Object(std::string _object_name = "");
                ~Object(void);

                void SetObjectId(unsigned int _object_id);
                unsigned int GetObjectId(void);
                std::string GetObjectName(void);

                void AddAttribute(EG::Game::ObjectAttribute *attribute);

                ObjectAttributes *GetAttributes(void);
                bool HasAttributesOfType(EG::Game::ObjectAttribute::ObjectAttributeType type);
                ObjectAttributeArray *GetAttributesByType(EG::Game::ObjectAttribute::ObjectAttributeType type);

                void AddScript(ObjectScript *script);
                std::vector<ObjectScript *> *GetScripts(void);
            protected:
                std::string object_name;
                unsigned int object_id;

                ObjectAttributes attributes;
                std::vector<ObjectScript *> scripts;
        };
    }
}

#endif
