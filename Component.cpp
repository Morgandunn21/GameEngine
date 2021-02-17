#include "Component.h"

void Component::Print()
{
	cout << "Component ID ->" << ComponentLoader::GetComponentID(name) << endl;
	cout << "Type ->" << name << endl;
}

void Component::Save(ofstream& file)
{
	file << "Component ID ->" << ComponentLoader::GetComponentID(name) << endl;
	file << "Type ->" << name << endl;
	file << "NumAttributes ->" << numLinesToLoad << endl;
}

void Component::Update(GameObject * o, float deltaTime)
{
	this->owner = o;
}

void Component::Init(GameObject* o)
{
	this->owner = o;
}

void Component::Load(ifstream &file)
{
	string line;

	getline(file, line);
	this->name = ComponentLoader::LoadStringFromFile(line);

	getline(file, line);
	this->numLinesToLoad = ComponentLoader::LoadIntFromFile(line);

	for (int i = 0; i < numLinesToLoad; i++)
	{
		getline(file, line);
		LoadComponentAttribute(line, i);
	}
}

void Component::LoadComponentAttribute(string line, int index)
{
	switch (index)
	{
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////
enum ComponentType
{
    light,
    pointLight,
    spotLight,
    dirLight,
    camera,
    cameraController,
    meshRenderer,
    objectMover,
    rigidBody
};

Component* ComponentLoader::LoadComponentByName(string s)
{
    return LoadComponentByID(GetComponentID(s));
}

Component* ComponentLoader::LoadComponentByID(int id)
{
    Component* component = new Component();
    switch (id)
    {
    case(light):
        component = new Light();
        break;
    case(pointLight):
        component = new PointLight();
        break;
    case(spotLight):
        component = new SpotLight();
        break;
    case(dirLight):
        component = new DirectionalLight();
        break;
    case(camera):
        component = new Camera();
        break;
    case(cameraController):
        component = new CameraController();
        break;
    case(meshRenderer):
        component = new MeshRenderer();
        break;
    case(objectMover):
        component = new ObjectMover();
        break;
    case(rigidBody):
        component = new RigidBody();
        break;
    }

    return component;
}

int ComponentLoader::GetComponentID(string s)
{
    int result = -1;

    if (s == "Camera")
    {
        result = camera;
    }
    else if (s == "CameraController")
    {
        result = cameraController;
    }
    else if (s == "Light")
    {
        result = light;
    }
    else if (s == "PointLight")
    {
        result = pointLight;
    }
    else if (s == "SpotLight")
    {
        result = spotLight;
    }
    else if (s == "DirectionalLight")
    {
        result = dirLight;
    }
    else if (s == "MeshRenderer")
    {
        result = meshRenderer;
    }
    else if (s == "ObjectMover")
    {
        result = objectMover;
    }
    else if (s == "RigidBody")
    {
        result = rigidBody;
    }

    return result;
}

int ComponentLoader::LoadIntFromFile(string line)
{
    return (int)LoadFloatFromFile(line);
}

float ComponentLoader::LoadFloatFromFile(string line)
{
    int pos = line.find(">");
    return std::stof(line.substr(pos + 1));
}

string ComponentLoader::LoadStringFromFile(string line)
{
    int pos = line.find(">");
    return line.substr(pos + 1);
}

vec3 ComponentLoader::LoadVec3FromFile(string line)
{
    float values[3];
    int pos = line.find(">");
    string textPath = line.substr(pos + 1);

    for (int i = 0; i < 2; i++)
    {
        int nextPos = textPath.find(" ");
        values[i] = std::stof(textPath.substr(0, nextPos));
        textPath = textPath.substr(nextPos + 1);
        pos = nextPos;
    }

    values[2] = std::stof(textPath);
    return vec3(values[0], values[1], values[2]);
}

string ComponentLoader::vec3toString(vec3 v)
{
    string result;

    result = to_string(v.x) + " " + to_string(v.y) + " " + to_string(v.z);

    return result;
}