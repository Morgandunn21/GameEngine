#include "MeshRenderer.h"
#include "Scene.h"

MeshRenderer::MeshRenderer()
{
    this->name = "MeshRenderer";
    this->numLinesToLoad = 3;
}

MeshRenderer::MeshRenderer(Model* m, Material* material)
{
    this->name = "MeshRenderer";
    this->numLinesToLoad = 3;
    this->model = m;
    this->meshes = model->meshes;
    this->material = material;
}

void MeshRenderer::Draw()
{
    shader->use();
    shader->setMat4("model", modelMatrix);

    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    shader->setVec3("material.tint", material->tint);
    shader->setFloat("material.shininess", material->shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->diffuseMap->ID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material->specularMap->ID);
        
    for (int i = 0; i < meshes.size(); i++)
    {
        DrawMesh(i);
    }
}

void MeshRenderer::DrawMesh(int index)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    //used to accommodate any number of textures
    for (unsigned int i = 0; i < meshes[index].textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = meshes[index].textures[i].type;
        if (name == "texture_diffuse")
            number = to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, meshes[index].textures[i].ID);
    }

    // draw mesh
    glBindVertexArray(meshes[index].VAO);
    glDrawElements(GL_TRIANGLES, meshes[index].indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void MeshRenderer::Update(GameObject* o, float deltaTime)
{
    this->owner = o;

    if (!stationary || first)
    {
        modelMatrix = owner->GetModel();

        this->first = false;
    }

    Draw();
}

void MeshRenderer::LoadComponentAttribute(string line, int index)
{
    switch (index)
    {
    case 0:
        this->model = new Model(ComponentLoader::LoadStringFromFile(line));
        this->meshes = model->meshes;
        break;
    case 1:
        this->material = new Material(ComponentLoader::LoadStringFromFile(line));
        break;
    case 2:
        this->shader = GlobalConstants::shaders->at(ComponentLoader::LoadIntFromFile(line));
        break;
    }
}

void MeshRenderer::Print()
{
    Component::Print();
    cout << "Model ->" << model->path << endl;
    cout << "Material ->" << material->name << endl;
    cout << "Shader ->" << shader->index << endl;
}

void MeshRenderer::Save(ofstream& file)
{
    Component::Save(file);
    file << "Model ->" << model->path << endl;
    file << "Material ->" << material->name << endl;
    file << "Shader ->" << shader->index << endl;
}