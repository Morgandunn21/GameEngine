//This class holds some basic utility functions
#ifndef UTIL_H
#define UTIL_H
//Inclusions-----------------------------------------------------
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


using namespace std;

class util
{
public:
	static int randInt(int min, int max)
	{
		random_device r;
		default_random_engine e{ r() };
		uniform_int_distribution<> dis(min, max);

		return dis(e);
	}

	static float randFloat(float min, float max)
	{
		random_device r;
		default_random_engine e{ r() };
		uniform_real_distribution<> dis(min, max);

		return dis(e);
	}

	static bool randBool()
	{
		random_device r;
		default_random_engine e{ r() };
		uniform_real_distribution<> dis(0, 1);

		return dis(e) > .5;
	}

	static GLenum glCheckError_()
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << std::endl;
		}
		return errorCode;
	}
private:
};
#endif
