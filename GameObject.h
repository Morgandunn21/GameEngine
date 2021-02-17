//This class is used to create and manipulate shader programs

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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

#include "stb_image.h"
#include "Component.h"
class Component;
class ComponentLoader;

using namespace std;
using namespace glm;

//Class---------------------------------------------------------------------------
class GameObject
{
    //Public values///////////////////////////////////////////////
public:
    GameObject(string n = "New Object", vec3 pos = vec3(0.0, 0.0, 0.0), vec3 rot = vec3(0.0, 0.0, 0.0), vec3 scl = vec3(1.0, 1.0, 1.0), unsigned int vao = 0, unsigned int numVert = 0);

    void SetName(string);
    void SetPosition(vec3);
    void SetRotation(vec3);
    void SetScale(vec3);
    void SetModel(mat4);
    void SetVAO(unsigned int);
    void SetNumVertices(unsigned int);

    string GetName();
    vec3 GetPosition();
    vec3 GetRotation();
    vec3 GetForward();
    vec3 GetScale();
    mat4 GetModel();
    unsigned int GetVAO();
    unsigned int GetNumVertices();

    void Translate(vec3);
    void Rotate(vec3);
    void Scale(vec3);

    void Print();
    void Save(ofstream &);
    void Load(ifstream &);

    void AddComponent(Component *);

    void Update(float);

    //Private values/////////////////////////////
private:
    int numComponents;
    string name;
    vector<Component *> components;
    vec3 objectPosition;
    vec3 objectRotation;
    vec3 objectScale;
    mat4 model;
    unsigned int VAO;
    unsigned int numVertices;

    void CalculateModel();
};
#endif
#pragma once