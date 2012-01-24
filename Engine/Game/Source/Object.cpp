#include "../Object.h"
#include "../ObjectBasicAttribute.h"
#include "../ObjectRenderingAttribute.h"
#include "../ObjectControlAttribute.h"
#include "../ObjectEmissionAttribute.h"

namespace EG{
    namespace Game{
        Object::Object(std::string _object_name){
            object_name = _object_name;
        }

        Object::~Object(void){
            //std::cout << "Deleting Object" << std::endl;
            std::vector<EG::Game::ObjectAttribute::ObjectAttributeType>::iterator type_iter = attributes.GetKeysBegin();
            while (type_iter != attributes.GetKeysEnd()){
                EG::Game::ObjectAttribute::ObjectAttributeType type_key = (*type_iter);
                std::vector<EG::Game::ObjectAttribute *> *type_attributes = GetAttributesByType(type_key);
                std::vector<EG::Game::ObjectAttribute *>::iterator attr_iter = type_attributes->begin();
                while (attr_iter != type_attributes->end()){
                    if (type_key == EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH) {
                        delete static_cast<EG::Game::ObjectAttributeRenderingMesh *>(*attr_iter);
                    }else if (type_key == EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION){
                        delete static_cast<EG::Game::ObjectAttributeBasicTransformation *>(*attr_iter);
                    }else if (type_key == EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT){
                        delete static_cast<EG::Game::ObjectAttributeEmissionLight *>(*attr_iter);
                    }else if (type_key == EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY){
                        delete static_cast<EG::Game::ObjectAttributeControlRigidBody *>(*attr_iter);
                    }
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
