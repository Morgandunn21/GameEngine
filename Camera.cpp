#include "Camera.h"

// Constructor with vectors
Camera::Camera(vec3 up) : forward(vec3(0.0f, 0.0f, -1.0f)), moveSpeed(MOVE_SPEED), rotationSpeed(ROT_SPEED), zoom(ZOOM)
{
    name = "Camera";
    numLinesToLoad = 1;
    worldUp = up;
    projection = perspective(radians(zoom), (float)GlobalConstants::SCREEN_WIDTH / (float)GlobalConstants::SCREEN_HEIGHT, 0.1f, 100.0f);
}
// Constructor with scalar values
Camera::Camera(float upX, float upY, float upZ) : forward(vec3(0.0f, 0.0f, -1.0f)), moveSpeed(MOVE_SPEED), rotationSpeed(ROT_SPEED), zoom(ZOOM)
{
    name = "Camera";
    numLinesToLoad = 1;
    worldUp = vec3(upX, upY, upZ);
    projection = perspective(radians(zoom), (float)GlobalConstants::SCREEN_WIDTH / (float)GlobalConstants::SCREEN_HEIGHT, 0.1f, 100.0f);
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
mat4 Camera::GetViewMatrix()
{
    vec3 pos = owner->GetPosition();
    return lookAt(pos, pos + forward, up);
}

mat4 Camera::GetProjectionMatrix()
{
    return projection;
}

vec3 Camera::GetPosition()
{
    vec3 result;

    if (owner)
    {
        result = owner->GetPosition();
    }

    return result;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    float yRot = owner->GetRotation().y;
    float xRot = owner->GetRotation().x;
    // Calculate the new forward vector
    vec3 front;
    front.x = cos(radians(yRot)) * cos(radians(xRot));
    front.y = sin(radians(xRot));
    front.z = sin(radians(yRot)) * cos(radians(xRot));
    this->forward = normalize(front);
    // Also re-calculate the right and up vector
    this->right = normalize(cross(forward, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = normalize(cross(right, forward));
}

void Camera::Update(GameObject * o, float deltaTime)
{
    this->owner = o;

    if (owner == nullptr)
    {
        cout << "test cam" << endl;
        return;
    }

    updateCameraVectors();
}

void Camera::Init(GameObject* o)
{
    this->owner = o;

    if (owner == nullptr)
    {
        return;
    }

    updateCameraVectors();
}

void Camera::Print()
{
    Component::Print();
    cout << "Up Vector ->" << up.x << " " << up.y << " " << up.z << endl;
}

void Camera::Save(ofstream& file)
{
    Component::Save(file);
    file << "Up Vector ->" << up.x << " " << up.y << " " << up.z << endl;
}

void Camera::LoadComponentAttribute(string line, int index)
{
    switch (index)
    {
    case 0:
        up = ComponentLoader::LoadVec3FromFile(line);
        break;
    }
}
