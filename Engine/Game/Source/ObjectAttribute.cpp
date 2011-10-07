#include "../ObjectAttribute.h"

namespace EG{
	namespace Game{
		ObjectAttribute::ObjectAttribute(void){
			type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_DEFAULT;
		}

		ObjectAttribute::~ObjectAttribute(void){
			//
		}

		EG::Game::ObjectAttribute::ObjectAttributeType ObjectAttribute::GetType(void){
			return type;
		}
	}
}
