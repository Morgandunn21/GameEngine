#include "RigidBody.h"
#include "PhysicsWorld.h"

RigidBody::RigidBody(int s, float m, float b, vec3 sc)
{
	name = "RigidBody";
	numLinesToLoad = 4;
	shapeID = (Shapes)s;
	mass = m;
	scale = sc;
	bounciness = b;
}

void RigidBody::Init(GameObject* o)
{
	this->owner = o;

	btCollisionShape* shape = InitShape();

	collisionShapes.push_back(shape);

	btTransform groundTransform;
	groundTransform.setIdentity();

	vec3 ownerPosition = owner->GetPosition();
	groundTransform.setOrigin(btVector3(ownerPosition.x, ownerPosition.y, ownerPosition.z));

	btScalar btMass(mass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (btMass != 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
	{
		shape->calculateLocalInertia(btMass, localInertia);
	}

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(btMass, myMotionState, shape, localInertia);
	body = new btRigidBody(rbInfo);

	body->setRestitution(bounciness);

	GlobalConstants::physicsWorld->AddRigidBody(this);
}

void RigidBody::Print()
{
	cout << "Shape ID ->" << shapeID;
	cout << "Mass ->" << mass;
	cout << "Bounciness ->" << bounciness;
	cout << "Scale ->" << ComponentLoader::vec3toString(scale);
}

void RigidBody::Save(ofstream& file)
{
	file << "Shape ID ->" << shapeID;
	file << "Mass ->" << mass;
	file << "Bounciness ->" << bounciness;
	file << "Scale ->" << ComponentLoader::vec3toString(scale);
}

void RigidBody::LoadComponentAttribute(string line, int index)
{
	switch (index)
	{
	case 0:
		this->shapeID = (Shapes)ComponentLoader::LoadIntFromFile(line);
		break;
	case 1:
		this->mass = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 2:
		this->bounciness = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 3:
		this->scale = ComponentLoader::LoadVec3FromFile(line);
		break;
	}
}

btCollisionShape* RigidBody::InitShape()
{
	btCollisionShape* result = new btBoxShape(btVector3(btScalar(1), btScalar(1), btScalar(1)));

	switch (shapeID)
	{
	case box:
		result = new btBoxShape(btVector3(btScalar(scale.x), btScalar(scale.y), btScalar(scale.z)));
		break;
	case sphere:
		result = new btSphereShape(btScalar(scale.x));
		break;
	}

	return result;
}

void RigidBody::UpdatePosition(vec3 pos)
{
	this->owner->SetPosition(pos);
}

btCollisionShape* RigidBody::GetShape()
{
	return shape;
}

btRigidBody* RigidBody::GetRigidBody()
{
	return body;
}