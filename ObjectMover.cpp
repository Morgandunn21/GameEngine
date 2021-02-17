#include "ObjectMover.h"

ObjectMover::ObjectMover(float ms, vec3 rs, vec3 ss, int mt)
{
	this->name = "ObjectMover";
	this->moveSpeed = ms;
	this->rotSpeed = rs;
	this->scaleSpeed = ss;
	this->moveType = (MoveType)mt;
}

bool ObjectMover::Move(vec3 waypoint, float deltaTime)
{
	bool arrived = false;

	vec3 pos = owner->GetPosition();
	vec3 dir = waypoint - pos;

	if (glm::length(dir) <= moveSpeed * deltaTime)
	{
		owner->SetPosition(waypoint);
		arrived = true;
	}
	else
	{
		dir = normalize(dir);

		dir *= moveSpeed * deltaTime;

		pos += dir;

		this->owner->SetPosition(pos);
	}

	return arrived;
}

void ObjectMover::MoveThroughWaypoints(float deltaTime)
{
	if (Move(waypoints[index], deltaTime))
	{
		switch (moveType)
		{
			int step;
		case loop:
			if (index == waypoints.size() - 1)
			{
				this->index = 0;
			}
			else
			{
				this->index++;
			}
			break;
		case oscillate:
			if ((index == 0 && backwards) || (index == waypoints.size() - 1 && !backwards))
			{
				backwards = !backwards;
			}

			step = 1;

			if (backwards)
			{
				step *= -1;
			}

			index += step;
			break;
		case skip:
			if (index == waypoints.size() - 1)
			{
				this->index = 1;
				this->owner->SetPosition(waypoints[0]);
			}
			else
			{
				this->index++;
			}
			break;
		}
	}
}

void ObjectMover::Rotate(float deltaTime)
{
	vec3 rot = owner->GetRotation();

	rot += rotSpeed * deltaTime;

	owner->SetRotation(rot);
}

void ObjectMover::Update(GameObject * o, float deltaTime)
{
	this->owner = o;
	MoveThroughWaypoints(deltaTime);
	Rotate(deltaTime);
}

void ObjectMover::Print()
{
	numLinesToLoad = 4 + numWaypoints;
	Component::Print();
	cout << "Move Speed ->" << to_string(moveSpeed);
	cout << "Rot Speed ->"  << ComponentLoader::vec3toString(rotSpeed);
	cout << "Scale Speed ->" << ComponentLoader::vec3toString(scaleSpeed);
	cout << "Move Type ->" << moveType;

	for (int i = 0; i < numLinesToLoad - 4; i++)
	{
		cout << "Waypoint " << i << " ->" << ComponentLoader::vec3toString(waypoints[i]);
	}
}

void ObjectMover::Save(ofstream& file)
{
	numLinesToLoad = 4 + numWaypoints;
	Component::Save(file);
	file << "Move Speed ->" << to_string(moveSpeed);
	file << "Rot Speed ->" << ComponentLoader::vec3toString(rotSpeed);
	file << "Scale Speed ->" << ComponentLoader::vec3toString(scaleSpeed);
	file << "Move Type ->" << moveType;

	for (int i = 0; i < numLinesToLoad - 4; i++)
	{
		file << "Waypoint " << i << " ->" << ComponentLoader::vec3toString(waypoints[i]);
	}
}

void ObjectMover::LoadComponentAttribute(string line, int index)
{
	switch (index)
	{
	case 0:
		this->moveSpeed = ComponentLoader::LoadFloatFromFile(line);
		break;
	case 1:
		this->rotSpeed = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 2:
		this->scaleSpeed = ComponentLoader::LoadVec3FromFile(line);
		break;
	case 3:
		this->moveType = (MoveType)ComponentLoader::LoadIntFromFile(line);
		break;
	default :
		this->waypoints.push_back(ComponentLoader::LoadVec3FromFile(line));
		break;
	}
}