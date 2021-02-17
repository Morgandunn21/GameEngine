#ifndef OBJECTMOVER_H
#define OBJECTMOVER_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Component.h"

using namespace glm;
using namespace std;

enum MoveType
{
	loop,
	oscillate,
	skip
};

class ObjectMover : public Component
{
public:
	ObjectMover(float ms = 2.5, vec3 rs = vec3(0.0), vec3 ss = vec3(0, 0, 0), int mt = skip);

	bool Move(vec3, float);
	void MoveThroughWaypoints(float);
	void Rotate(float);
	void Scale(float);
	void AddWaypoint(vec3);

	void Update(GameObject*, float) override;
	void LoadComponentAttribute(string, int) override;
	void Print() override;
	void Save(ofstream&) override;

private:
	int numWaypoints= 0;
	int index = 0;
	bool backwards = false;
	MoveType moveType;
	float moveSpeed;
	vec3 rotSpeed;
	vec3 scaleSpeed;
	vector<vec3> waypoints;
};

#endif