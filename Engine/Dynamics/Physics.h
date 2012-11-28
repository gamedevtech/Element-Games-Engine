#ifndef EG_DYNAMICS_PHYSICS_H
#define EG_DYNAMICS_PHYSICS_H

#define BIT(x) (1<<(x))

#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include "../Math/Math.h"

namespace EG{
    namespace Dynamics{
        class CollisionShape{
            public:
                CollisionShape(void);
                ~CollisionShape(void);

                enum CollisionShapeType{
                    COLLISION_SHAPE_SPHERE = 0,
                    COLLISION_SHAPE_BOX = 1,
                    COLLISION_SHAPE_CONE = 2,
                    COLLISION_SHAPE_CYLINDER = 3,
                    COLLISION_SHAPE_PLANE = 4,
                    COLLISION_SHAPE_POLYSHAPE = 5,
                    COLLISION_SHAPE_COMPOUND = 6,
                    COLLISION_SHAPE_HEIGHTS = 7,
                    COLLISION_SHAPE_NONE = 8
                };

                CollisionShapeType GetShapeType(void);

                void SetMass(float _mass);
                float GetMass(void);
                btCollisionShape *GetBulletShape(void);
            protected:
                CollisionShapeType shape_type;
                float mass;
                btCollisionShape *bt_shape;
        };

        class CollisionSphere : public CollisionShape{
            public:
                CollisionSphere(float _mass, float _radius = 1.0f);
            private:
                float radius;
        };

        class CollisionBox : public CollisionShape{
            public:
                CollisionBox(float _mass, glm::vec3 _half_extents = glm::vec3(0.5f, 0.5f, 0.5f));
            private:
                glm::vec3 half_extents;
        };

        class CollisionCone : public CollisionShape{
            public:
                CollisionCone(float _mass, float _radius, float _height);
            private:
                float radius;
                float height;
        };

        class CollisionCylinder : public CollisionShape{
            public:
                CollisionCylinder(float _mass, glm::vec3 _half_extents = glm::vec3(0.5f, 0.5f, 0.5f));
            private:
                glm::vec3 half_extents;
        };

        class CollisionPlane : public CollisionShape{
            public:
                CollisionPlane(float _mass, glm::vec3 _normal = glm::vec3(0.0f, 1.0f, 0.0f), float _constant = 0.0f);
            private:
                float mass;
                float constant;
                glm::vec3 normal;
        };

        /*class CollisionPolys : public CollisionShape{
            public:
                CollisionPolys(void);
            private:
                //
        };

        class CollisionHeights : public CollisionShape{
            public:
                CollisionHeights(void);
            private:
                //
        };

        class CollisionCompoundShape : public CollisionShape{
            public:
                CollisionCompoundShape(void);
            private:
                std::vector<CollisionShape *> shapes;
        };*/

        class RigidBody{
            public:
                RigidBody(CollisionShape *_shape, glm::mat4 transformation, glm::vec3 _local_scaling = glm::vec3(1.0f, 1.0f, 1.0f));
                ~RigidBody(void);

                btRigidBody *GetBulletBody(void);
                glm::mat4 GetMotionState(void);

                void SetCollisionFiltering(int _collision_group, int _collides_with);
                int GetCollisionGroup(void);
                int GetCollidesWith(void);

                void ApplyForce(glm::vec3 force_vector, glm::vec3 relative_position = glm::vec3(0.0f, 0.0f, 0.0f));
                void ApplyImpulse(glm::vec3 impulse_vector, glm::vec3 relative_position = glm::vec3(0.0f, 0.0f, 0.0f));
                void ApplyTorque(glm::vec3 torque);
            private:
                btRigidBody *bt_rigid_body;
                glm::vec3 local_scaling;
                CollisionShape *shape;
                int collision_group;
                int collides_with;
        };

        class Physics{
            public:
                Physics(void);
                ~Physics(void);

                void Update(float frame_time);
                void AddRigidBody(RigidBody *body);
                void RemoveRigidBody(RigidBody *body);
                void SetGravity(glm::vec3 gravity_vector);
            private:
                btBroadphaseInterface *broadphase;
                btDefaultCollisionConfiguration *configuration;
                btCollisionDispatcher *dispatcher;
                btSequentialImpulseConstraintSolver *solver;
                btDiscreteDynamicsWorld *world;
        };
    }
}

#endif
