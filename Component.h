//This class is used to create and manipulate shader programs

#ifndef COMPONENT_H
#define COMPONENT_H

//Inclusions----------------------------------------------------------------------
#include <string>
#include "GameObject.h"

using namespace std;
using namespace glm;

class GameObject;

//Class---------------------------------------------------------------------------
class Component
{
    //Public values///////////////////////////////////////////////
public:
    string name;

    virtual void Print();
    virtual void Save(ofstream&);
    virtual void Update(GameObject*, float deltaTime);
    virtual void Init(GameObject*);
    virtual void LoadComponentAttribute(string, int);
    void Load(ifstream&);

protected:
    int numLinesToLoad;
    GameObject* owner;
private:

};

#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "CameraController.h"
#include "ObjectMover.h"
#include "RigidBody.h"

class ComponentLoader
{
    //Public values///////////////////////////////////////////////
public:
    static Component* LoadComponentByName(string);
    static Component* LoadComponentByID(int);

    static int GetComponentID(string);

    static int LoadIntFromFile(string);
    static float LoadFloatFromFile(string);
    static string LoadStringFromFile(string);
    static vec3 LoadVec3FromFile(string);
    static string vec3toString(vec3);
protected:
private:

};
#endif
#pragma once
