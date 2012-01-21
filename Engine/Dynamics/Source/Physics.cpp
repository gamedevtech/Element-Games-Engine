#include "../Physics.h"

namespace EG{
	namespace Dynamics{
		Physics::Physics(void){
			broadphase = new btDbvtBroadphase();
			configuration = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(configuration);
			solver = new btSequentialImpulseConstraintSolver();
			world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, configuration);
			world->setGravity(btVector3(0, -10, 0));
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
	}
}
