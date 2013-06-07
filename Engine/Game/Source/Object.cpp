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
            for (ObjectAttributeArrayPair attributes_of_type : attributes) {
                for (ObjectAttribute *attribute : attributes_of_type.second) {
                    switch (attribute->GetType()) {
                        case EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_RENDERING_MESH:
                            delete static_cast<EG::Game::ObjectAttributeRenderingMesh *>(attribute);
                            break;
                        case EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_BASIC_TRANSFORMATION:
                            delete static_cast<EG::Game::ObjectAttributeBasicTransformation *>(attribute);
                            break;
                        case EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT:
                            delete static_cast<EG::Game::ObjectAttributeEmissionLight *>(attribute);
                            break;
                        case EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY:
                            delete static_cast<EG::Game::ObjectAttributeControlRigidBody *>(attribute);
                            break;
                    }
                }
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
            attributes[attribute->GetType()].push_back(attribute);
        }

        ObjectAttributes *Object::GetAttributes(void){
            return &attributes;
        }

        bool Object::HasAttributesOfType(EG::Game::ObjectAttribute::ObjectAttributeType type) {
            return attributes.count(type) > 0;
        }

        ObjectAttributeArray *Object::GetAttributesByType(EG::Game::ObjectAttribute::ObjectAttributeType type) {
            return &attributes[type];
        }

        void Object::AddScript(ObjectScript *script) {
            scripts.push_back(script);
        }

        std::vector<ObjectScript *> *Object::GetScripts(void) {
            return &scripts;
        }
    }
}
