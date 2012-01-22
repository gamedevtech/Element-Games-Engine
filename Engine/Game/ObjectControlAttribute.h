#ifndef EG_GAME_OBJECT_CONTROL_ATTRIBUTE_H
#define EG_GAME_OBJECT_CONTROL_ATTRIBUTE_H

#include "ObjectAttribute.h"
#include "../Dynamics/Physics.h"

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
    }
}

#endif
