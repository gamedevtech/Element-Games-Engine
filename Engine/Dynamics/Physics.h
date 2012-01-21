#ifndef EG_DYNAMICS_PHYSICS_H
#define EG_DYNAMICS_PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>

namespace EG{
	namespace Dynamics{
		class CollisionShape{
			public:
				CollisionShape(void);
				~CollisionShape(void);

				enum CollisionShapeType{
					SPHERE = 0,
					BOX = 1,
					CONE = 2,
					CYLINDER = 3,
					PLANE = 4,
					POLYSHAPE = 5,
					COMPOUND = 6,
					HEIGHTS = 7
				};

				CollisionShapeType GetShapeType(void);

				void SetMass(float _mass);
				float GetMass(void);
			protected:
				CollisionShapeType shape_type;
				float mass;
		};

		class CollisionSphere : public CollisionShape{
			public:
				CollisionSphere(void);
				~CollisionSphere(void);
			private:
				//
		};

		class CollisionBox : public CollisionShape{
			public:
				CollisionBox(void);
				~CollisionBox(void);
			private:
				//
		};

		class CollisionCone : public CollisionShape{
			public:
				CollisionCone(void);
				~CollisionCone(void);
			private:
				//
		};

		class CollisionCylinder : public CollisionShape{
			public:
				CollisionCylinder(void);
				~CollisionCylinder(void);
			private:
				//
		};

		class CollisionPlane : public CollisionShape{
			public:
				CollisionPlane(void);
				~CollisionPlane(void);
			private:
				//
		};

		class CollisionPolys : public CollisionShape{
			public:
				CollisionPolys(void);
				~CollisionPolys(void);
			private:
				//
		};

		class CollisionHeights : public CollisionShape{
			public:
				CollisionHeights(void);
				~CollisionHeights(void);
			private:
				//
		};

		class CollisionCompoundShape : public CollisionShape{
			public:
				CollisionCompoundShape(void);
				~CollisionCompoundShape(void);
			private:
				std::vector<CollisionShape *> shapes;
		};

		class RigidBody{
			public:
				RigidBody(void);
				~RigidBody(void);
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
		};
	}
}

#endif
