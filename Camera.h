#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "Component.h"
#include "GlobalConstants.h"

using namespace glm;

// Default camera values
const float YROT = -90.0f;
const float XROT = 0.0f;
const float MOVE_SPEED = 2.5f;
const float ROT_SPEED = 0.1f;
const float ZOOM = 45.0f;

class Component;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public Component
{
public:
    // Camera Attributes
    vec3 forward;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    float yRot;
    float xRot;
    // Camera options
    float moveSpeed;
    float rotationSpeed;
    float zoom;

    // Constructor with vectors
    Camera(vec3 up = vec3(0.0f, 1.0f, 0.0f));
    // Constructor with scalar values
    Camera(float upX, float upY, float upZ);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    mat4 GetViewMatrix();
    mat4 GetProjectionMatrix();
    vec3 GetPosition();

    void Update(GameObject*, float) override;
    void Init(GameObject*) override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;

private:
    mat4 projection;
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif
