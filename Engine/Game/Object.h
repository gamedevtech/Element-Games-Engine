#ifndef EG_GAME_OBJECT_H
#define EG_GAME_OBJECT_H

//#include <map>
#include <vector>
#include <string>

#include "../Utility/Dictionary.h"
#include "ObjectAttribute.h"

namespace EG{
	namespace Game{
		class Object{
			public:
				Object(std::string _object_name);
				~Object(void);

				void SetObjectId(unsigned int _object_id);
				unsigned int GetObjectId(void);
				std::string GetObjectName(void);

				void AddAttribute(EG::Game::ObjectAttribute *attribute);

				EG::Utility::Dictionary<EG::Game::ObjectAttribute::ObjectAttributeType, std::vector<EG::Game::ObjectAttribute *> *> *GetAttributes(void);
				std::vector<EG::Game::ObjectAttribute *> *GetAttributesByType(EG::Game::ObjectAttribute::ObjectAttributeType type);
				bool HasAttributesOfType(EG::Game::ObjectAttribute::ObjectAttributeType type);
			protected:
				std::string object_name;
				unsigned int object_id;

				EG::Utility::Dictionary<EG::Game::ObjectAttribute::ObjectAttributeType, std::vector<EG::Game::ObjectAttribute *> *> attributes;
		};
	}
}

#endif
