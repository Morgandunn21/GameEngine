#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "Texture.h"

using namespace std;
using namespace glm;

class Material
{
public:
    Texture* diffuseMap;
    Texture* specularMap;
    vec3 tint;
    float shininess;
    string name;

    Material();
    Material(Texture*, Texture*, vec3, float);
    Material(string);
    void ApplyToShader(Shader*);
    void SaveMaterial(string name);
    void LoadMaterial(string name);
private:
    string directory = "FileSystem/Materials/";
};
#endif