#include "GameObject.h"

//Constructors----------------------------------------
GameObject::GameObject(string n, vec3 pos, vec3 rot, vec3 scl, unsigned int vao, unsigned int numVert)
{
	this->name = n;
	this->objectPosition = pos;
	this->objectRotation = rot;
	this->objectScale = scl;
	this->VAO = vao;
	this->numVertices = numVert;
	CalculateModel();
}

void GameObject::Print()
{
	cout << "------GameObject------" << endl;
	cout << "Name ->" << name << endl;
	cout << "Pos ->" << objectPosition.x << ", " << objectPosition.y << ", " << objectPosition.z << endl;
	cout << "Rot ->" << objectRotation.x << ", " << objectRotation.y << ", " << objectRotation.z << endl;
	cout << "Scl ->" << objectScale.x << ", " << objectScale.y << ", " << objectScale.z << endl;
	cout << "Num Components ->" << numComponents << endl;
	for (Component* c : components)
	{
		cout << "**Component**" << endl;
		c->Print();
	}
	cout << "----------------------" << endl;
}

void GameObject::Save(ofstream& file)
{
	file << "GameObject" << endl;
	file << "Name ->" << name << endl;
	file << "Pos ->" << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << endl;
	file << "Rot ->" << objectRotation.x << " " << objectRotation.y << " " << objectRotation.z << endl;
	file << "Scl ->" << objectScale.x << " " << objectScale.y << " " << objectScale.z << endl;
	file << "Num Components ->" << numComponents << endl;
	for (Component* c : components)
	{
		file << "**Component**" << endl;
		c->Save(file);
	}
	file << "----------------------" << endl;
}

void GameObject::Load(ifstream& file)
{
	string line;

	for (int i = 0; i < numComponents + 5; i++)
	{
		getline(file, line);
		if (i < 5)
		{
			switch (i)
			{
			case 0:
				this->name = ComponentLoader::LoadStringFromFile(line);
				break;
			case 1:
				this->SetPosition(ComponentLoader::LoadVec3FromFile(line));
				break;
			case 2:
				this->SetRotation(ComponentLoader::LoadVec3FromFile(line));
				break;
			case 3:
				this->SetScale(ComponentLoader::LoadVec3FromFile(line));
				break;
			case 4:
				this->numComponents = ComponentLoader::LoadIntFromFile(line);
				break;
			}
		}
		if (line == "**Component**")
		{
			getline(file, line);
			int ID = ComponentLoader::LoadIntFromFile(line);

			Component* c = ComponentLoader::LoadComponentByID(ID);

			c->Load(file);

			AddComponent(c);
		}
	}
}

//Setters---------------------------------------------
void GameObject::SetName(string n)
{
	this->name = n;
}

void GameObject::SetPosition(vec3 pos)
{
	this->objectPosition = pos;
	CalculateModel();
}

void GameObject::SetRotation(vec3 rot)
{
	this->objectRotation = rot;
	CalculateModel();
}

void GameObject::SetScale(vec3 scl)
{
	this->objectScale = scl;
	CalculateModel();
}

void GameObject::SetVAO(unsigned int vao)
{
	this->VAO = vao;
}

void GameObject::SetNumVertices(unsigned int numVerts)
{
	this->numVertices = numVerts;
}

void GameObject::SetModel(mat4 mod)
{
	this->model = mod;
}

//Getters---------------------------------------------
string GameObject::GetName()
{
	return this->name;
}

vec3 GameObject::GetPosition()
{
	return this->objectPosition;
}

vec3 GameObject::GetRotation()
{
	return this->objectRotation;
}

vec3 GameObject::GetForward()
{
	float yRot = GetRotation().y;
	float xRot = GetRotation().x;
	// Calculate the new forward vector
	vec3 front;
	front.x = cos(radians(yRot)) * cos(radians(xRot));
	front.y = sin(radians(xRot));
	front.z = sin(radians(yRot)) * cos(radians(xRot));

	return normalize(front);
}

vec3 GameObject::GetScale()
{
	return this->objectScale;
}

unsigned int GameObject::GetVAO()
{
	return this->VAO;
}

unsigned int GameObject::GetNumVertices()
{
	return this->numVertices;
}

mat4 GameObject::GetModel()
{
	CalculateModel();
	return this->model;
}

//Transformations-----------------------------------
void GameObject::Translate(vec3 translateVector)
{
	this->objectPosition += translateVector;
	CalculateModel();
}

void GameObject::Rotate(vec3 rotationVector)
{
	this->objectRotation += rotationVector;
	CalculateModel();
}

void GameObject::Scale(vec3 scaleVector)
{
	this->objectScale += scaleVector;
	CalculateModel();
}

void GameObject::CalculateModel()
{
	mat4 temp(1.0);

	temp = translate(temp, objectPosition);

	if (objectRotation.x != 0)
	{
		temp = rotate(temp, radians(objectRotation.x), vec3(1, 0, 0));
	}

	if (objectRotation.y != 0)
	{
		temp = rotate(temp, radians(objectRotation.y), vec3(0, 1, 0));
	}

	if (objectRotation.z != 0)
	{
		temp = rotate(temp, radians(objectRotation.z), vec3(0, 0, 1));
	}

	temp = scale(temp, objectScale);

	this->model = temp;
}

//Components----------------------------------------
void GameObject::AddComponent(Component* c)
{
	c->Init(this);
	this->components.push_back(c);
}

void GameObject::Update(float deltaTime)
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update(this, deltaTime);
	}
}