#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Model.h"
#include "Component.h"
#include "Material.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

class MeshRenderer : public Component{
public:
    /*  Mesh Data  */
    Model* model;

    bool stationary;
    bool first = true;

    mat4 modelMatrix;

    /*  Functions  */
    // constructor
    MeshRenderer();
    MeshRenderer(Model*, Material* material = new Material());

    // render the mesh
    void Draw();

    void Update(GameObject*, float) override;
    void LoadComponentAttribute(string, int) override;
    void Print() override;
    void Save(ofstream&) override;

    void SetShader(Shader* s) { this->shader = s; }

    void SetMat(Material* mat) { this->material = mat; }

private:
    Shader* shader;
    Material* material;
    vector<Mesh> meshes;

    void DrawMesh(int index);
};
#endif