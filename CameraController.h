#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "Component.h"
#include <GLFW\glfw3.h>

using namespace glm;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class CameraController : public Component
{
public:
    // Camera Attributes
    vec3 forward;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    // Camera options
    float moveSpeed = 2.5f;
    float rotationSpeed = 0.1f;
    float zoom;

    CameraController();
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessInput(float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    void Update(GameObject *, float) override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;

    void SetWindow(GLFWwindow* w)
    {
        window = w;
    }

private:
    //GLFW window
    GLFWwindow* window;
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif