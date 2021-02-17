//This class is used to create and manipulate shader programs

#ifndef SCENE_H
#define SCENE_H

//Inclusions----------------------------------------------------------------------
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <LinearMath/btTransform.h>
#include <LinearMath/btHashMap.h>

#include "GameObject.h"
class GameObject;

#include "util.h"

//Components
#include "Camera.h"
#include "CameraController.h"
#include "Light.h"
#include "MeshRenderer.h"

//Data
#include "Model.h"
#include "Shader.h"
#include "Material.h"

#include "PhysicsWorld.h"

using namespace std;
using namespace glm;

//Class---------------------------------------------------------------------------
class Scene
{
    //Public values///////////////////////////////////////////////
public:
    //objects
    vector<GameObject*> heirarchy;
    vec3 ambientLight;

    void SaveScene(string);
    void LoadScene(string);
    void CreateObjects();
    void Update(float);
    void SetWindow(GLFWwindow* w) { window = w; }
    void Init();
    void ProcessMouseMovement(float, float);
    void ProcessMouseScroll(float);
    //Private values/////////////////////////////
private:
    mat4 view;
    mat4 projection;
    vec3 camPos;
    Shader* phongLightingShader;
    Shader* noLightingShader;
    CameraController* mainCameraController;
    Camera* mainCamera;
    GLFWwindow* window;
    PhysicsWorld* physicsWorld;

    string directory = "FileSystem/Scenes/";

    void CreateCameraObject();
    void UpdateShaderCamera();
};
#endif
