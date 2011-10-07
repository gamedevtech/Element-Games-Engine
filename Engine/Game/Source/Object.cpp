#include "../Object.h"

namespace EG{
	namespace Game{
		Object::Object(std::string _object_name){
			object_name = _object_name;
		}

		Object::~Object(void){
			//
		}

		void Object::SetObjectId(unsigned int _object_id){
			object_id = _object_id;
		}

		unsigned int Object::GetObjectId(void){
			return object_id;
		}

		std::string Object::GetObjectName(void){
			return object_name;
		}

		void Object::AddAttribute(EG::Game::ObjectAttribute *attribute){
			attributes[attribute->GetType()].push_back(attribute);
		}

		std::map<EG::Game::ObjectAttribute::ObjectAttributeType, std::vector<EG::Game::ObjectAttribute *> > *Object::GetAttributes(void){
			return &attributes;
		}

		std::vector<EG::Game::ObjectAttribute *> *Object::GetAttributesByType(EG::Game::ObjectAttribute::ObjectAttributeType type){
			return &attributes[type];
		}
	}
}
