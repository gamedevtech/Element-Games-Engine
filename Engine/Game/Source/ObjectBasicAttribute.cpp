#include "../ObjectBasicAttribute.h"

namespace EG{
	namespace Game{
		ObjectAttributeBasicTransformation::ObjectAttributeBasicTransformation(glm::mat4 _transformation){
			type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION;
			transformation = _transformation;
		}

		ObjectAttributeBasicTransformation::~ObjectAttributeBasicTransformation(void){
			//
		}

		void ObjectAttributeBasicTransformation::SetTransformation(glm::mat4 _transformation){
			transformation = _transformation;
		}

		glm::mat4 ObjectAttributeBasicTransformation::GetTransformation(void){
			return transformation;
		}



		ObjectAttributeBasicString::ObjectAttributeBasicString(std::string _key, std::string _value){
			type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_STRING;
			key = _key;
			value = _value;
		}
		ObjectAttributeBasicString::~ObjectAttributeBasicString(void){
			//
		}

		void ObjectAttributeBasicString::SetKey(std::string _key){
			key = _key;
		}
		void ObjectAttributeBasicString::SetValue(std::string _value){
			value = _value;
		}

		std::string ObjectAttributeBasicString::GetKey(void){
			return key;
		}
		std::string ObjectAttributeBasicString::GetValue(void){
			return value;
		}

		ObjectAttributeBasicInteger::ObjectAttributeBasicInteger(std::string _key, int _value){
			type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_INTEGER;
			key = _key;
			value = _value;
		}
		ObjectAttributeBasicInteger::~ObjectAttributeBasicInteger(void){
			//
		}

		void ObjectAttributeBasicInteger::SetKey(std::string _key){
			key = _key;
		}
		void ObjectAttributeBasicInteger::SetValue(int _value){
			value = _value;
		}

		std::string ObjectAttributeBasicInteger::GetKey(void){
			return key;
		}
		int ObjectAttributeBasicInteger::GetValue(void){
			return value;
		}

		ObjectAttributeBasicFloat::ObjectAttributeBasicFloat(std::string _key, float _value){
			type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_FLOAT;
			key = _key;
			value = _value;
		}
		ObjectAttributeBasicFloat::~ObjectAttributeBasicFloat(void){
			//
		}

		void ObjectAttributeBasicFloat::SetKey(std::string _key){
			key = _key;
		}
		void ObjectAttributeBasicFloat::SetValue(float _value){
			value = _value;
		}

		std::string ObjectAttributeBasicFloat::GetKey(void){
			return key;
		}
		float ObjectAttributeBasicFloat::GetValue(void){
			return value;
		}
	}
}
