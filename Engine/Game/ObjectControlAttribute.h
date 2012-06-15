#ifndef EG_GAME_OBJECT_CONTROL_ATTRIBUTE_H
#define EG_GAME_OBJECT_CONTROL_ATTRIBUTE_H

#include "ObjectAttribute.h"
#include "../Dynamics/Physics.h"
#include "../Dynamics/Animation.h"

namespace EG{
    namespace Game{
        class ObjectAttributeControlRigidBody : public ObjectAttribute{
            public:
                ObjectAttributeControlRigidBody(EG::Dynamics::RigidBody *_body);
                ~ObjectAttributeControlRigidBody(void);

                void SetBody(EG::Dynamics::RigidBody *_body);
                EG::Dynamics::RigidBody *GetBody(void);

                void SetConnected(bool _connected_to_bullet = false);
                bool GetConnected(void);
            private:
                EG::Dynamics::RigidBody *body;
                bool connected_to_bullet;
        };

        class ObjectAttributeControlAnimationState : public ObjectAttribute{
            public:
                ObjectAttributeControlAnimationState(EG::Dynamics::AnimationState *_state);
                ~ObjectAttributeControlAnimationState(void);

                void SetAnimationState(EG::Dynamics::AnimationState *_state);
                EG::Dynamics::AnimationState *GetAnimationState(void);
            private:
                EG::Dynamics::AnimationState *animation_state;
        };
    }
}

#endif
