//This class is used to create and manipulate shader programs
#ifndef SHADER_H
#define SHADER_H

//Inclusions----------------------------------------------------------------------
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "util.h"

using namespace std;
using namespace glm;

//Class---------------------------------------------------------------------------
class Shader
{
    //Public values///////////////////////////////////////////////
public:
    unsigned int MAX_POINT_LIGHTS = 4;
    unsigned int MAX_DIR_LIGHTS = 1;
    unsigned int MAX_SPOT_LIGHTS = 1;
    //ID of this shader
    unsigned int ID;
    unsigned int index;

    string name;

    Shader(const char* vertexPath, const char* fragmentPath, int i);

    Shader() { index = -1;  ID = -1; }

    // activate the shader
    // ------------------------------------------------------------------------
    void use() { glUseProgram(ID); }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const string& name, mat4 value)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, value_ptr(value));
    }

    void setVec4(const string& name, vec4 value)
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value));
    }

    void setVec3(const string& name, vec3 value)
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value));
    }

    void setVec3(const string& name, float v1, float v2, float v3)
    {
        vec3 value(v1, v2, v3);
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value));
    }

    int addPointLight()
    {
        this->numPointLights++;
        cout << "Add Light: " << to_string(numPointLights) << endl;
        return numPointLights - 1;
    }

    int addDirLight()
    {
        return numDirLights++;
    }

    int addSpotLight()
    {
        return numSpotLights++;
    }

    void initLights()
    {
        numPointLights = 0;
        numDirLights = 0;
        numSpotLights = 0;
        setInt("numPointLights", 0);
        setInt("numDirLights", 0);
        setInt("numSpotLights", 0);
    }

    void numLights()
    {
        cout << to_string(numPointLights) << " Point Lights" << endl;
        cout << to_string(numDirLights) << " Directional Lights" << endl;
        cout << to_string(numSpotLights) << " Spot Lights" << endl;
    }

    void setMVP(mat4 model, mat4 view, mat4 projection)
    {
        this->setMat4("model", model);
        this->setMat4("projection", projection);
        this->setMat4("view", view);
    }
    //Memory functions
    void SaveShader();
    void LoadShader(string);

    //Private values/////////////////////////////
private:
    string vert, frag;
    unsigned int numPointLights;
    unsigned int numDirLights;
    unsigned int numSpotLights;
    void checkCompileErrors(unsigned int, string);
};
#endif