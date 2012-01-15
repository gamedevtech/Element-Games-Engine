#include "../Object.h"

namespace EG{
    namespace Game{
        Object::Object(std::string _object_name){
            object_name = _object_name;
        }

        Object::~Object(void){
            std::vector<EG::Game::ObjectAttribute::ObjectAttributeType>::iterator type_iter = attributes.GetKeysBegin();
            while (type_iter != attributes.GetKeysEnd()){
                EG::Game::ObjectAttribute::ObjectAttributeType type_key = (*type_iter);
                std::vector<EG::Game::ObjectAttribute *> *type_attributes = GetAttributesByType(type_key);
                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = type_attributes->begin();
                while (attr_iter != type_attributes->end()){
                    EG::Game::ObjectAttribute *attribute = (*attr_iter);
                    delete attribute;
                    ++attr_iter;
                }
                ++type_iter;
            }
        }

        void Object::SetObjectId(unsigned int _object_id){
            object_id = _object_id;
        }

        unsigned int Object::GetObjectId(void){
            return object_id;
        }

        std::string Object::GetObjectName(void){
            if (object_name != ""){
                return object_name;
            }
            std::stringstream stream;
            stream << object_id;
            return stream.str();
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
