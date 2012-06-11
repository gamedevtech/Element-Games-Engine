#include "../ObjectControlAttribute.h"

namespace EG{
    namespace Game{
        ObjectAttributeControlRigidBody::ObjectAttributeControlRigidBody(EG::Dynamics::RigidBody *_body){
            type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_RIGID_BODY;
            body = _body;
            connected_to_bullet = false;
        }

        ObjectAttributeControlRigidBody::~ObjectAttributeControlRigidBody(void){
            delete body;
        }

        void ObjectAttributeControlRigidBody::SetBody(EG::Dynamics::RigidBody *_body){
            body = _body;
        }

        EG::Dynamics::RigidBody *ObjectAttributeControlRigidBody::GetBody(void){
            return body;
        }

        void ObjectAttributeControlRigidBody::SetConnected(bool _connected_to_bullet){
            connected_to_bullet = _connected_to_bullet;
        }

        bool ObjectAttributeControlRigidBody::GetConnected(void){
            return connected_to_bullet;
        }

        ObjectAttributeAnimationState::ObjectAttributeAnimationState(EG::Dynamics::AnimationState *_state){
            type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_CONTROL_ANIMATION;
            animation_state = _state;
        }
        ObjectAttributeAnimationState::~ObjectAttributeAnimationState(void){
            delete animation_state;
        }
        void ObjectAttributeAnimationState::SetAnimationState(EG::Dynamics::AnimationState *_state){
            animation_state = _state;
        }
        EG::Dynamics::AnimationState *ObjectAttributeAnimationState::GetAnimationState(void){
            return animation_state;
        }
    }
}
