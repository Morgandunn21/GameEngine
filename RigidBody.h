#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <btBulletDynamicsCommon.h>
#include <stdio.h>

#include "Component.h"

class Component;

class RigidBody : public Component
{
public:
	enum Shapes
	{
		box,
		sphere
	};

	RigidBody(int s = 0, float m = 0, float b = 0, vec3 sc = vec3(1, 1, 1));

	btRigidBody* GetRigidBody();
	btCollisionShape* GetShape();

	btCollisionShape* InitShape();

	void Init(GameObject*) override;
	void LoadComponentAttribute(string, int) override;
	void Print() override;
	void Save(ofstream&) override;

	void UpdatePosition(vec3);
private:
	Shapes shapeID;
	vec3 scale;
	btRigidBody* body;
	btCollisionShape* shape;
	float bounciness;
	float mass;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};
#endif 