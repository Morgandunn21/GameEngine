#include "CameraController.h"
#include "Scene.h"

CameraController::CameraController()
{
    name = "CameraController";
    numLinesToLoad = 0;
    worldUp = vec3(0, 1, 0);
    updateCameraVectors();
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void CameraController::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    if (owner == nullptr)
    {
        return;
    }

    vec3 pos = owner->GetPosition();

    float velocity = moveSpeed * deltaTime;

    if (direction == FORWARD)
    {
        pos += forward * velocity;
    }

    if (direction == BACKWARD)
    {
        pos -= forward * velocity;
    }

    if (direction == LEFT)
    {
        pos -= right * velocity;
    }

    if (direction == RIGHT)
    {
        pos += right * velocity;
    }

    this->owner->SetPosition(pos);
}

//Called each update to ee what keys are pressed
void CameraController::ProcessInput(float deltaTime)
{
    if (window == NULL)
    {
        cout << "Win NuLL" << endl;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        ProcessKeyboard(RIGHT, deltaTime);
    }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void CameraController::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    if (owner == nullptr)
    {
        return;
    }

    float yRot = this->owner->GetRotation().y;
    float xRot = this->owner->GetRotation().x;

    xoffset *= rotationSpeed;
    yoffset *= rotationSpeed;

    yRot += xoffset;
    xRot += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (xRot > 89.0f)
        {
            xRot = 89.0f;
        }

        if (xRot < -89.0f)
        {
            xRot = -89.0f;
        }
    }

    this->owner->SetRotation(vec3(xRot, yRot, this->owner->GetRotation().z));
    // Update forward, right and up Vectors using the updated Euler angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void CameraController::ProcessMouseScroll(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
    {
        zoom -= yoffset;
    }

    if (zoom <= 1.0f)
    {
        zoom = 1.0f;
    }

    if (zoom >= 45.0f)
    {
        zoom = 45.0f;
    }
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void CameraController::updateCameraVectors()
{
    if (owner == nullptr)
    {
        return;
    }

    this->forward = owner->GetForward();
    // Also re-calculate the right and up vector
    this->right = normalize(cross(forward, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = normalize(cross(right, forward));
}

void CameraController::Update(GameObject * o, float deltaTime)
{
    this->owner = o;
    ProcessInput(deltaTime);
}

void CameraController::LoadComponentAttribute(string line, int index)
{
    //Scene::mainCameraController = this;
}

void CameraController::Print()
{
    Component::Print();
}

void CameraController::Save(ofstream& file)
{
    Component::Save(file);
}