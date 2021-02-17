//Inculsions--------------------------------------------------------------------
#include <iostream>
#include <array>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"
//Namespace---------------------------------------------------------------------
using namespace std;
using namespace glm;
//Prototypes--------------------------------------------------------------------
//Callbacks
void Framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//Engine
GLFWwindow* Init();
void SetCallbacks(GLFWwindow*);
void RenderingLoop(GLFWwindow*);
void ProcessInput(GLFWwindow*);
void CreateShaderPrograms();
void SetupVertices();
void CreateTextures();
void Timing();
//Constants---------------------------------------------------------------------
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
//Variables---------------------------------------------------------------------
//Shaders
Shader phongLightingShader;
Shader noLightingShader;
array<unsigned int, 2> vao;
array<unsigned int, 2> vbo;
vector<Texture> textures;
vector<Material> materials;
// camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//lighting
vec3 lightPos(1.2f, 1.0f, 2.0f);
//Main Function-----------------------------------------------------------------
int main()
{
    //Initialize Libraries
    GLFWwindow* window = Init();

    SetCallbacks(window);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Create the shader program
    CreateShaderPrograms();

    SetupVertices();

    CreateTextures();

    //Rendering Loop
    RenderingLoop(window);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(vao.size(), vao.data());
    glDeleteBuffers(vbo.size(), vbo.data());

    //Terminate the window
    glfwTerminate();
    return 0;
}

//Aditional Functions------------------------------------------------------------

//Resets the OpenGL Viewport whenever the GLFW window is resized
void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

//Initializes GLFW, GLAD, and an openGL viewport
GLFWwindow* Init()
{
    //GLFW Initiation//////////////////////////////////////////////
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Creating the GLFW Window////////////////////////////////////
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    //Initializing GLAD//////////////////////////////////////////
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return NULL;
    }

    //Set the OpenGL Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    return window;
}

void SetCallbacks(GLFWwindow* window)
{
    //Register resizing Callback
    glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
    //Register mouse moving Callback
    glfwSetCursorPosCallback(window, mouse_callback);
    //Register scrollwheel Callback
    glfwSetScrollCallback(window, scroll_callback);
}

//Handles the rendering loop
void RenderingLoop(GLFWwindow* window)
{
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        //Per frame timing
        Timing();

        //Handle Input
        ProcessInput(window);
        
        glClear(GL_DEPTH_BUFFER_BIT);

        float bkg[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, bkg);

        // be sure to activate shader when setting uniforms/drawing objects
        phongLightingShader.use();
        phongLightingShader.setVec3("viewPos", camera.pos);

        // view/projection transformations
        mat4 projection = perspective(radians(camera.zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();
        phongLightingShader.setMat4("projection", projection);
        phongLightingShader.setMat4("view", view);

        // world transformation
        mat4 model = mat4(1.0f);
        phongLightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        noLightingShader.use();
        noLightingShader.setMat4("projection", projection);
        noLightingShader.setMat4("view", view);
        model = mat4(1.0f);
        model = translate(model, lightPos);
        model = scale(model, vec3(0.2f)); // a smaller cube
        noLightingShader.setMat4("model", model);

        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Handle Buffers and Events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

//Processes User Input
void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// build and compile our shader program
void CreateShaderPrograms()
{
    phongLightingShader = Shader("phongLighting_vert.shader", "phongLighting_frag.shader");
    noLightingShader = Shader("noLighting_vert.shader", "noLighting_frag.shader");
}

//Calculates where the vertices are
void SetupVertices()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    //Generate the vertex arrays
	glGenVertexArrays(vao.size(), vao.data());

    //Generate the vertex buffer
    glGenBuffers(vbo.size(), vbo.data());
    //Bind it
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    //Store the vertice data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind first cube object
	glBindVertexArray(vao[0]);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Bind second cube object
    glBindVertexArray(vao[1]);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void SetupLighting()
{
    phongLightingShader.initLights();

    PointLight pointLight(lightPos, vec3(0.05f, 0.05f, 0.05f), vec3(.8, .8, .8), vec3(1, 1, 1), 1, .09, .032);
    pointLight.ApplyToShader(phongLightingShader);
}

void CreateTextures()
{
    array<string, 0> texturePaths = {};
    textures.resize(texturePaths.size());

    for (unsigned int i = 0; i < texturePaths.size(); i++)
    {
        textures[i] = Texture(texturePaths[i].c_str(), i, true);
    }
}

void Timing()
{
    // per-frame time logic
        // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}