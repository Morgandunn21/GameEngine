#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <btBulletDynamicsCommon.h>
#include <stdio.h>

#include "RigidBody.h"

class PhysicsWorld
{
public:
	void Init();
	void AddRigidBody(RigidBody*);
	void Update(float);
	void Clean();
private:
	vector<RigidBody*> rigidBodies;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};
#endif 