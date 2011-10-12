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
			if (!(attributes.Has(attribute->GetType()))){
				attributes.Set(attribute->GetType(), new std::vector<EG::Game::ObjectAttribute *>);
			}
			attributes.Get(attribute->GetType())->push_back(attribute);
		}

		EG::Utility::Dictionary<EG::Game::ObjectAttribute::ObjectAttributeType, std::vector<EG::Game::ObjectAttribute *> *> *Object::GetAttributes(void){
			//std::cout << "Getting ALl of It!" << std::endl;
			return &attributes;
		}

		std::vector<EG::Game::ObjectAttribute *> *Object::GetAttributesByType(EG::Game::ObjectAttribute::ObjectAttributeType type){
			//std::cout << "Getting Type: " << type << std::endl;
			std::vector<EG::Game::ObjectAttribute *> *out = attributes.Get(type);
			//return attributes.Get(type);
			//std::cout << "Got" << std::endl;
			return out;
		}

		bool Object::HasAttributesOfType(EG::Game::ObjectAttribute::ObjectAttributeType type){
			return attributes.Has(type);
		}
	}
}
