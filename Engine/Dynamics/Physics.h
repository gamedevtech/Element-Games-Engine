#ifndef EG_DYNAMICS_PHYSICS_H
#define EG_DYNAMICS_PHYSICS_H

#include <btBulletDynamicsCommon.h>
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
                RigidBody(CollisionShape *_shape);
                ~RigidBody(void);

                btRigidBody *GetBulletBody(void);
                glm::mat4 GetMotionState(void);
            private:
                btRigidBody *bt_rigid_body;
                CollisionShape *shape;
        };

        class Physics{
            public:
                Physics(void);
                ~Physics(void);

                void Update(float frame_time);
                void AddRigidBody(RigidBody *body);
            private:
                btBroadphaseInterface *broadphase;
                btDefaultCollisionConfiguration *configuration;
                btCollisionDispatcher *dispatcher;
                btSequentialImpulseConstraintSolver *solver;
                btDiscreteDynamicsWorld *world;

                std::vector<RigidBody *> bodies;
        };
    }
}

#endif
