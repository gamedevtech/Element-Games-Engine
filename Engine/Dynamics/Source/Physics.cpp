#include "../Physics.h"
#include <iostream>

namespace EG{
    namespace Dynamics{
        CollisionShape::CollisionShape(void){
            shape_type = COLLISION_SHAPE_NONE;
            mass = 0.0f;
            bt_shape = NULL;
        }

        CollisionShape::~CollisionShape(void){
            if (bt_shape){
                delete bt_shape;
            }
        }

        void CollisionShape::SetMass(float _mass){
            mass = _mass;
        }

        float CollisionShape::GetMass(void){
            return mass;
        }

        btCollisionShape *CollisionShape::GetBulletShape(void){
            return bt_shape;
        }

        CollisionSphere::CollisionSphere(float _mass, float _radius){
            mass = _mass;
            radius = _radius;
            bt_shape = new btSphereShape(radius);
        }

        CollisionBox::CollisionBox(float _mass, glm::vec3 _half_extents){
            mass = _mass;
            half_extents = _half_extents;
            bt_shape = new btBoxShape(btVector3(half_extents.x, half_extents.y, half_extents.z));
        }

        CollisionCone::CollisionCone(float _mass, float _radius, float _height){
            mass = _mass;
            radius = _radius;
            height = _height;
            bt_shape = new btConeShape(radius, height);
        }

        CollisionCylinder::CollisionCylinder(float _mass, glm::vec3 _half_extents){
            mass = _mass;
            half_extents = _half_extents;
            bt_shape = new btCylinderShape(btVector3(half_extents.x, half_extents.y, half_extents.z));
        }

        CollisionPlane::CollisionPlane(float _mass, glm::vec3 _normal, float _constant){
            mass = _mass;
            normal = _normal;
            constant = _constant;
            bt_shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), constant);
        }

        RigidBody::RigidBody(CollisionShape *_shape, glm::mat4 transformation, glm::vec3 _local_scaling){
            shape = _shape;

            //glm::vec3 scale = glm::vec3(transformation[0][0], transformation[1][1], transformation[2][2]);
            local_scaling = _local_scaling;
            float *matrix = glm::value_ptr(transformation);
            btTransform bt_trans;
            bt_trans.setFromOpenGLMatrix(matrix);
            btDefaultMotionState *ground_motion_state = new btDefaultMotionState(bt_trans);

            btRigidBody::btRigidBodyConstructionInfo ground_rigid_body_config_info(shape->GetMass(), ground_motion_state, shape->GetBulletShape(), btVector3(0.0f, 0.0f, 0.0f));
            bt_rigid_body = new btRigidBody(ground_rigid_body_config_info);
        }

        RigidBody::~RigidBody(void){
            delete bt_rigid_body;
            delete shape;
        }

        btRigidBody *RigidBody::GetBulletBody(void){
            return bt_rigid_body;
        }

        glm::mat4 RigidBody::GetMotionState(void){
            btMotionState *motion_state = bt_rigid_body->getMotionState();
            btTransform world_transform;
            motion_state->getWorldTransform(world_transform);
            btScalar matrix_data[16];
            world_transform.getOpenGLMatrix(matrix_data);
            glm::mat4 out = glm::make_mat4(matrix_data);
            out = out * glm::gtx::transform::scale(local_scaling);
            return out;
        }

        Physics::Physics(void){
            broadphase = new btDbvtBroadphase();
            configuration = new btDefaultCollisionConfiguration();
            dispatcher = new btCollisionDispatcher(configuration);
            solver = new btSequentialImpulseConstraintSolver();
            world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, configuration);
            world->setGravity(btVector3(0.0f, -10.0f, 0.0f));

            btStaticPlaneShape *ground_plane = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
            btDefaultMotionState *ground_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
            btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, ground_motion_state, ground_plane, btVector3(0.0f, 0.0f, 0.0f));
            btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
            world->addRigidBody(groundRigidBody);
        }

        Physics::~Physics(void){
            delete world;
            delete solver;
            delete dispatcher;
            delete configuration;
            delete broadphase;
        }

        void Physics::Update(float frame_time){
            world->stepSimulation(frame_time, 10);
        }

        void Physics::AddRigidBody(RigidBody *body){
            bodies.push_back(body);
            world->addRigidBody(body->GetBulletBody());
        }
    }
}
