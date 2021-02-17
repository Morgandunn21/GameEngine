#include "Material.h"
#include "Shader.h"

Material::Material()
{
	this->shininess = 64;
	this->diffuseMap = new Texture();
	this->specularMap = new Texture();
	this->tint = vec3(1.0, 1.0, 1.0);
}

Material::Material(Texture * diffuseTexture, Texture* specularTexture, vec3 t,float shine)
{
	this->diffuseMap = diffuseTexture;
	this->specularMap = specularTexture;
	this->shininess = shine;
	this->tint = t;
}

Material::Material(string name)
{
	LoadMaterial(name+".txt");
}

void Material::ApplyToShader(Shader* s)
{
	s->use();

	s->setInt("material.diffuse", 0);
	s->setInt("material.specular", 1);
	s->setVec3("material.tint", tint);
	s->setFloat("material.shininess", shininess);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap->ID);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap->ID);
}

void Material::SaveMaterial(string name)
{
	ofstream file(directory + name, ios::trunc);

	file << "Diffuse ->" + diffuseMap->path << endl;
	file << "Specular ->" + specularMap->path << endl;
	file << "Tint ->" + std::to_string(tint.r) + " " + std::to_string(tint.g) + " " + std::to_string(tint.b) << endl;
	file << "Shininess ->" + std::to_string(shininess);
}

void Material::LoadMaterial(string name)
{
	ifstream file(directory + name);
	string line;

	if (file.is_open())
	{
		string textPath;
		int i, pos;
		int colors [3];
		for(i = 0; i < 4 && getline(file, line); i++)
		{
			switch (i)
			{
			case(0):
				pos = line.find(">");
				textPath = line.substr(pos + 1);
				if (textPath != "Null")
				{
					this->diffuseMap = new Texture(textPath, false);
				}
				else
				{
					this->diffuseMap = new Texture();
				}
				break;
			case(1):
				pos = line.find(">");
				textPath = line.substr(pos + 1);
				if (textPath != "Null")
				{
					this->specularMap = new Texture(textPath, false);
				}
				else
				{
					this->specularMap = new Texture();
				}
				break;
			case(2):
				pos = line.find(">");
				textPath = line.substr(pos + 1);
				for (int i = 0; i < 2; i++)
				{
					int nextPos = textPath.find(" ");
					colors[i] = std::stof(textPath.substr(0, nextPos));
					textPath = textPath.substr(nextPos + 1);
					pos = nextPos;
				}

				colors[2] = std::stof(textPath);
				this->tint = vec3(colors[0], colors[1], colors[2]);
				break;
			case(3):
				pos = line.find(">");
				this->shininess = std::stof(line.substr(pos + 1));
				break;
			}
		}

		if (i <= 3)
		{
			cout << "---ERROR LOADING MATERIAL---" << endl;
		}

		file.close();
	}

	else
	{
		cout << "Unable to load material, applying default" << endl;
		this->shininess = 64;
		this->diffuseMap = new Texture();
		this->specularMap = new Texture();
		this->tint = vec3(1.0, 1.0, 1.0);
	}
}

