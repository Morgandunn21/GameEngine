#include "Light.h"

Light::Light()
{
	position = vec3(0, 0, 0);
	ambient = vec3(.1, .1, .1);
	diffuse = vec3(1, 1, 1);
	specular = vec3(1, 1, 1);
}

Light::Light(vec3 pos, vec3 amb, vec3 dif, vec3 spec)
{
	position = pos;
	ambient = amb;
	diffuse = dif;
	specular = spec;
}

void Light::ApplyToShader()
{
	shader->use();
	shader->setVec3("light.ambient", ambient);
	shader->setVec3("light.diffuse", diffuse); // darken diffuse light a bit
	shader->setVec3("light.specular", specular);
	shader->setVec3("light.position", position);
}

void Light::SetShader(Shader* s)
{
	this->shader = s;
}

void Light::Update(GameObject* o, float deltaTime)
{
	this->owner = o;
	this->position = owner->GetPosition();
	ApplyToShader();
}

void Light::Init(GameObject* o)
{
	Update(o, 0);
}

void Light::LoadComponentAttribute(string line, int index)
{

}

void Light::Print()
{
	Component::Print();
}

void Light::Save(ofstream& file)
{

}

////////////////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight()
{
	name = "DirectionalLight";
	numLinesToLoad = 4;
	position = vec3(0, 0, 0);
	ambient = vec3(.1, .1, .1);
	diffuse = vec3(1, 1, 1);
	specular = vec3(1, 1, 1);
	direction = vec3(1, 0, 0);
}

DirectionalLight::DirectionalLight(vec3 dir, vec3 amb, vec3 dif, vec3 spec)
{
	name = "DirectionalLight";
	numLinesToLoad = 4;
	position = vec3(0, 0, 0);
	ambient = amb;
	diffuse = dif;
	specular = spec;
	direction = dir;
}

void DirectionalLight::ApplyToShader()
{
	shader->use();

	if (ID == -1)
	{
		ID = shader->addDirLight();
	}	

	if (ID < shader->MAX_DIR_LIGHTS)
	{
		shader->setInt("numDirLights", ID + 1);
		shader->setVec3("dirLight.ambient", ambient);
		shader->setVec3("dirLight.diffuse", diffuse); // darken diffuse light a bit
		shader->setVec3("dirLight.specular", specular);
		shader->setVec3("dirLight.direction", direction);
	}
}

void DirectionalLight::LoadComponentAttribute(string line, int index)
{
	switch (index)
	{
	case 0:
		this->ambient = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 1:
		this->diffuse = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 2:
		this->specular = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 3:
		this->direction = ComponentLoader::LoadVec3FromFile(line);
		break;
	}
}

void DirectionalLight::Print()
{
	Component::Print();
	cout << "Ambient ->" << ComponentLoader::vec3toString(ambient) << endl;
	cout << "Diffuse ->" << ComponentLoader::vec3toString(diffuse) << endl;
	cout << "Specular ->" << ComponentLoader::vec3toString(specular) << endl;
	cout << "Direction ->" << ComponentLoader::vec3toString(direction) << endl;;
}

void DirectionalLight::Save(ofstream& file)
{
	Component::Save(file);
	file << "Ambient ->" << ComponentLoader::vec3toString(ambient) << endl;
	file << "Diffuse ->" << ComponentLoader::vec3toString(diffuse) << endl;
	file << "Specular ->" << ComponentLoader::vec3toString(specular) << endl;
	file << "Direction ->" << ComponentLoader::vec3toString(direction) << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////

PointLight::PointLight()
{
	name = "PointLight";
	numLinesToLoad = 6;
	position = vec3(0, 0, 0);
	ambient = vec3(.1, .1, .1);
	diffuse = vec3(1, 1, 1);
	specular = vec3(1, 1, 1);

	constant = 1;
	linear = .09;
	quadratic = .032;
}

PointLight::PointLight(vec3 pos, vec3 amb, vec3 dif, vec3 spec, float cons, float lin, float quad)
{
	name = "PointLight";
	numLinesToLoad = 6;
	position = pos;
	ambient = amb;
	diffuse = dif;
	specular = spec;

	constant = cons;
	linear = lin;
	quadratic = quad;
}

void PointLight::ApplyToShader()
{
	shader->use();

	if (ID == -1)
	{
		ID = shader->addPointLight();
	}	

	if (ID < shader->MAX_POINT_LIGHTS)
	{
		shader->setInt("numPointLights", ID + 1);
		shader->setVec3("pointLights[" + to_string(ID) + "].ambient", ambient);
		shader->setVec3("pointLights[" + to_string(ID) + "].diffuse", diffuse);
		shader->setVec3("pointLights[" + to_string(ID) + "].specular", specular);
		shader->setVec3("pointLights[" + to_string(ID) + "].position", position);
		shader->setFloat("pointLights[" + to_string(ID) + "].constant", constant);
		shader->setFloat("pointLights[" + to_string(ID) + "].linear", linear);
		shader->setFloat("pointLights[" + to_string(ID) + "].quadratic", quadratic);
	}
}

void PointLight::LoadComponentAttribute(string line, int index)
{
	switch (index)
	{
	default:
		break;
	case 0:
		this->ambient = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 1:
		this->diffuse = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 2:
		this->specular = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 3:
		this->constant = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 4:
		this->linear = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 5:
		this->quadratic = ComponentLoader::LoadFloatFromFile(line);
		break;
	}
}

void PointLight::Print()
{
	Component::Print();
	cout << "Ambient ->" << ComponentLoader::vec3toString(ambient) << endl;
	cout << "Diffuse ->" << ComponentLoader::vec3toString(diffuse) << endl;
	cout << "Specular ->" << ComponentLoader::vec3toString(specular) << endl;
	cout << "Constant ->" << constant << endl;
	cout << "Linear ->" << linear << endl;
	cout << "Quadratic ->" << quadratic << endl;
}

void PointLight::Save(ofstream& file)
{
	Component::Save(file);
	file << "Ambient ->" << ComponentLoader::vec3toString(ambient) << endl;
	file << "Diffuse ->" << ComponentLoader::vec3toString(diffuse) << endl;
	file << "Specular ->" << ComponentLoader::vec3toString(specular) << endl;
	file << "Constant ->" << constant << endl;
	file << "Linear ->" << linear << endl;
	file << "Quadratic ->" << quadratic << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////

SpotLight::SpotLight()
{
	name = "SpotLight";
	numLinesToLoad = 8;

	position = vec3(0, 0, 0);
	ambient = vec3(.1, .1, .1);
	diffuse = vec3(1, 1, 1);
	specular = vec3(1, 1, 1);
	direction = vec3(0, 0, 0);

	constant = 1;
	linear = .09;
	quadratic = .032;

	cutOff = cos(radians(12.5f));
	outerCutOff = cos(radians(15.0f));
}

SpotLight::SpotLight(vec3 pos, vec3 amb, vec3 dif, vec3 spec, vec3 dir, float cons, float lin, float quad, float cO, float outerCO)
{
	name = "SpotLight";
	numLinesToLoad = 8;

	position = pos;
	ambient = amb;
	diffuse = dif;
	specular = spec;
	direction = dir;

	constant = cons;
	linear = lin;
	quadratic = quad;

	cutOff = cO;
	outerCutOff = outerCO;
}

void SpotLight::Update(GameObject* o, float deltaTime)
{
	this->owner = o;
	this->position = owner->GetPosition();
	this->direction = owner->GetForward();

	ApplyToShader();
}

void SpotLight::ApplyToShader()
{
	shader->use();

	if (ID == -1)
	{
		ID = shader->addSpotLight();
	}	

	if (ID < shader->MAX_SPOT_LIGHTS)
	{
		shader->setInt("numSpotLights", ID + 1);
		shader->setVec3("spotLight.ambient", ambient);
		shader->setVec3("spotLight.diffuse", diffuse); // darken diffuse light a bit
		shader->setVec3("spotLight.specular", specular);
		shader->setVec3("spotLight.direction", direction);
		shader->setVec3("spotLight.position", position);
		shader->setFloat("spotLight.cutOff", cutOff);
		shader->setFloat("spotLight.outerCutOff", outerCutOff);
		shader->setFloat("spotLight.constant", constant);
		shader->setFloat("spotLight.linear", linear);
		shader->setFloat("spotLight.quadratic", quadratic);
	}
}

void SpotLight::LoadComponentAttribute(string line, int index)
{
	switch (index)
	{
	default:
		break;
	case 0:
		this->ambient = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 1:
		this->diffuse = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 2:
		this->specular = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 3:
		this->cutOff = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 4:
		this->outerCutOff = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 5:
		this->constant = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 6:
		this->linear = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 7:
		this->quadratic = ComponentLoader::LoadFloatFromFile(line);
		break;
	}
}

void SpotLight::Print()
{
	Component::Print();
	cout << "Ambient ->" << ComponentLoader::vec3toString(ambient) << endl;
	cout << "Diffuse ->" << ComponentLoader::vec3toString(diffuse) << endl;
	cout << "Specular ->" << ComponentLoader::vec3toString(specular) << endl;
	cout << "Cut Off ->" << cutOff << endl;
	cout << "Outer Cut Off ->" << outerCutOff << endl;
	cout << "Constant ->" << constant << endl;
	cout << "Linear ->" << linear << endl;
	cout << "Quadratic ->" << quadratic << endl;
}

void SpotLight::Save(ofstream& file)
{
	Component::Save(file);
	file << "Ambient ->" << ComponentLoader::vec3toString(ambient) << endl;
	file << "Diffuse ->" << ComponentLoader::vec3toString(diffuse) << endl;
	file << "Specular ->" << ComponentLoader::vec3toString(specular) << endl;
	file << "Cut Off ->" << cutOff << endl;
	file << "Outer Cut Off ->" << outerCutOff << endl;
	file << "Constant ->" << constant << endl;
	file << "Linear ->" << linear << endl;
	file << "Quadratic ->" << quadratic << endl;
}