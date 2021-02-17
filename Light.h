#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Component.h"
#include "GlobalConstants.h"

using namespace glm;

class Light : public Component
{
public:
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    unsigned int ID = -1;

    Light();
    Light(vec3, vec3, vec3, vec3);
    virtual void ApplyToShader();

    void Update(GameObject*, float) override;
    void Init(GameObject*) override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;

    void SetShader(Shader*);
protected:
    Shader* shader = GlobalConstants::shaders->at(0);
};

class DirectionalLight : public Light
{
public:
    vec3 direction;

    DirectionalLight();
    DirectionalLight(vec3, vec3, vec3, vec3);
    void ApplyToShader() override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;
};

class PointLight : public Light
{
public:
    float constant;
    float linear;
    float quadratic;

    PointLight();
    PointLight(vec3, vec3, vec3, vec3, float, float, float);
    void ApplyToShader() override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;
};

class SpotLight : public Light
{
public:
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;

    SpotLight();
    SpotLight(vec3, vec3, vec3, vec3, vec3, float, float, float, float, float);

    void Update(GameObject*, float) override;
    void ApplyToShader() override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;
};
#endif
