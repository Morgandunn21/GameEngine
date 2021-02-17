#include "Scene.h"

void Scene::SaveScene(string name)
{
    ofstream file((directory + name).c_str(), ios::trunc);

    for (int i = 1; i < heirarchy.size(); i++)
    {
        heirarchy[i]->Save(file);
    }

    file.close();
}

void Scene::LoadScene(string name)
{
    phongLightingShader->use();
    phongLightingShader->initLights();
    phongLightingShader->setVec3("ambientLight", vec3(0.05, 0.05, 0.05));

    CreateCameraObject();

    ifstream file((directory + name + ".txt").c_str());
    string line;

    if (file.is_open())
    {
        GameObject* temp = new GameObject();

        while (getline(file, line))
        {
            if (line == "GameObject")
            {
                temp->Load(file);
                heirarchy.push_back(temp);
                temp = new GameObject();
            }
        }

        file.close();
    }

    else cout << "Unable to open file";
}

void Scene::CreateObjects()
{
    phongLightingShader->use();
    phongLightingShader->initLights();
    phongLightingShader->setVec3("ambientLight", vec3(0.05));

    CreateCameraObject();

    GameObject* cube = new GameObject("cube");
    heirarchy.push_back(cube);
    cube->SetScale(vec3(.5, .5, .5));
    cube->SetRotation(vec3(45.0, 45.0, 45.0));
    MeshRenderer * cubeRenderer = new MeshRenderer (new Model("cube.obj"), new Material("Test"));
    cubeRenderer->SetShader(phongLightingShader);
    cube->AddComponent(cubeRenderer);

    GameObject* lightObject = new GameObject("light");
    SpotLight* light = new SpotLight();
    light->SetShader(phongLightingShader);
    lightObject->SetScale(vec3(0.5));
    lightObject->SetPosition(vec3(2, .5, 4));
    MeshRenderer* lightCubeRenderer = new MeshRenderer(new Model("cube.obj"), new Material("Test"));
    lightCubeRenderer->SetShader(noLightingShader);
    lightObject->AddComponent(light);
    lightObject->AddComponent(lightCubeRenderer);
    heirarchy.push_back(lightObject);

    for(GameObject* g : heirarchy)
    {
        g->Print();
    }

    SaveScene("Scene2.txt");
}

void Scene::Update(float deltaTime)
{
    UpdateShaderCamera();

    physicsWorld->Update(deltaTime);

    for (GameObject* go : heirarchy)
    {
        go->Update(deltaTime);
    }
}

void Scene::Init()
{
    GlobalConstants::shaders->push_back(new Shader("phongLighting_vert.shader", "phongLighting_frag.shader", 0));
    GlobalConstants::shaders->push_back(new Shader("noLighting_vert.shader", "noLighting_frag.shader", 1));
    GlobalConstants::physicsWorld->Init();

    phongLightingShader = GlobalConstants::shaders->at(0);
    noLightingShader = GlobalConstants::shaders->at(1);
    physicsWorld = GlobalConstants::physicsWorld;
}

void Scene::UpdateShaderCamera()
{
    view = mainCamera->GetViewMatrix();
    projection = mainCamera->GetProjectionMatrix();
    camPos = mainCamera->GetPosition();

    phongLightingShader->use();
    phongLightingShader->setMat4("view", view);
    phongLightingShader->setMat4("projection", projection);
    phongLightingShader->setVec3("viewPos", camPos);

    noLightingShader->use();
    noLightingShader->setMat4("view", view);
    noLightingShader->setMat4("projection", projection);
    noLightingShader->setVec3("viewPos", camPos);
}

void Scene::ProcessMouseMovement(float xMove, float yMove)
{
    mainCameraController->ProcessMouseMovement(xMove, yMove);
}

void Scene::ProcessMouseScroll(float scroll)
{
    mainCameraController->ProcessMouseScroll(scroll);
}

void Scene::CreateCameraObject()
{
    GameObject* cameraObject = new GameObject();
    cameraObject->SetScale(vec3(40, 40, 40));
    cameraObject->SetRotation(vec3(0, 180, 0));
    mainCameraController = new CameraController();
    mainCamera = new Camera(vec3(0, 1, 0));
    DirectionalLight* light = new DirectionalLight();
    MeshRenderer* Skybox = new MeshRenderer(new Model("sphere.obj"), new Material("Sky"));

    mainCameraController->SetWindow(window);
    light->SetShader(phongLightingShader);
    Skybox->SetShader(noLightingShader);
    Skybox->stationary = true;

    cameraObject->AddComponent(mainCameraController);
    cameraObject->AddComponent(mainCamera);
    cameraObject->AddComponent(light);
    cameraObject->AddComponent(Skybox);

    heirarchy.push_back(cameraObject);
}