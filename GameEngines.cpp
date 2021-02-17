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
#include "Scene.h"
#include "GlobalConstants.h"
std::vector<Shader*>* GlobalConstants::shaders = new vector<Shader*>();
PhysicsWorld* GlobalConstants::physicsWorld = new PhysicsWorld();
#include "util.h"
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
void Timing();

//Constants---------------------------------------------------------------------
const unsigned int SCREEN_WIDTH = GlobalConstants::SCREEN_WIDTH;
const unsigned int SCREEN_HEIGHT = GlobalConstants::SCREEN_HEIGHT;
//Variables---------------------------------------------------------------------
// camera
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//Scene
vector<string> scenePaths = {"Scene1"};
Scene currentScene;

//Main Function-----------------------------------------------------------------
int main()
{
    //Initialize Libraries
    GLFWwindow* window = Init();
    SetCallbacks(window);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    currentScene.Init();
    currentScene.SetWindow(window);
    currentScene.LoadScene("Scene1");

    //Rendering Loop
    RenderingLoop(window);
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

    currentScene.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    currentScene.ProcessMouseScroll(yoffset);
}

//Initializes GLFW, GLAD, and an openGL viewport
GLFWwindow* Init()
{
    //GLFW Initiation//////////////////////////////////////////////
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Creating the GLFW Window////////////////////////////////////
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Engines", NULL, NULL);
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

        glClear(GL_DEPTH_BUFFER_BIT);
        float bkg[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, bkg);
        //Update the scene
        currentScene.Update(deltaTime);

        //Handle Buffers and Events
        glfwSwapBuffers(window);
        glfwPollEvents();
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
////////////////////////////////////////

