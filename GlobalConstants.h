#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#include "Shader.h"
class PhysicsWorld;

class GlobalConstants
{
public:
	static std::vector<Shader*>* shaders;
	static PhysicsWorld* physicsWorld;

	const static int SCREEN_WIDTH = 1600;
	const static int SCREEN_HEIGHT = 900;
};
#endif
#pragma once