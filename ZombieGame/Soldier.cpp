#include "Soldier.h"

const float TILE_OFFSET = 2.0f;

Soldier::Soldier()
{
}
Soldier::~Soldier()
{
	printf("Soldier Killed Himself");
}

void Soldier::init(float speed, glm::vec2 pos, std::vector<Bullet>* bullets, std::vector<glm::vec2> waypoints)
{
	_speed = speed;
	_position = pos;
	_bullets = bullets;
	_waypoints = waypoints;
	_currentWaypoint = 0;
	_detectRadius = 400.0f;
	_shootRadius = 160.0f;
	_state = SoldierState::ROAM;
	_color.r = 255;
	_color.g = 215;
	_color.b = 0;
	_color.a = 255;
	_health = 100;
	_type = 3;
}

void Soldier::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies)
{
	_target = getNearestAgent(humans, zombies);
	if (_state == SoldierState::ROAM)
	{
		_direction = glm::normalize(_waypoints[_currentWaypoint] - _position);
		_position += _direction * _speed;

		if (_position.x + TILE_OFFSET >= _waypoints[_currentWaypoint].x && _position.x - TILE_OFFSET <= _waypoints[_currentWaypoint].x &&
			_position.y + TILE_OFFSET >= _waypoints[_currentWaypoint].y && _position.y - TILE_OFFSET <= _waypoints[_currentWaypoint].y)
			_currentWaypoint++;

		if (_currentWaypoint == _waypoints.size())
			_currentWaypoint = 0;
		
		if (glm::length(_target->getPosition() - _position) <= _detectRadius)
			_state = SoldierState::CHASE;
	}
	else if (_state == SoldierState::CHASE)
	{
		glm::vec2 direction = glm::normalize(_target->getPosition() - _position);
		_position += direction * _speed;

		if (glm::length(_target->getPosition() - _position) <= _shootRadius)
			_state = SoldierState::ATTACK;
	}
	else if (_state == SoldierState::ATTACK)
	{
		glm::vec2 direction = glm::normalize(_target->getPosition() - _position);
		glm::vec2 firePosition = normalize(_position, _target->getPosition(), AGENT_RADIUS*1.5f);

		_gun->update(true, firePosition, direction, *_bullets);

		if (glm::length(_target->getPosition() - _position) > _shootRadius)
			_state = SoldierState::ROAM;
	}

	collideWithLevel(levelData);
}

void Soldier::addGun(Gun * gun)
{
	_gun = gun;
}

Agent* Soldier::getNearestAgent(std::vector<Human*>& humans, std::vector<Zombie*>& zombies)
{
	Agent* closestAgent = nullptr;
	float smallestDistance = 9999999.0f;

	for (int i = 0; i < humans.size(); i++)
	{
		if (humans[i]->getType() != 3)
		{
			glm::vec2 vDist = humans[i]->getPosition() - _position;
			float fDist = glm::length(vDist);

			if (fDist < smallestDistance)
			{
				smallestDistance = fDist;
				closestAgent = humans[i];
			}
		}
	}
	for (int i = 0; i < zombies.size(); i++)
	{
		glm::vec2 vDist = zombies[i]->getPosition() - _position;
		float fDist = glm::length(vDist);

		if (fDist < smallestDistance)
		{
			smallestDistance = fDist;
			closestAgent = zombies[i];
		}
	}

	return closestAgent;
}
glm::vec2 Soldier::normalize(glm::vec2 a, glm::vec2 b, float length)
{
	//get the distance between a and b along the x and y axes
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	//right now, sqrt(dx ^ 2 + dy ^ 2) = distance(a, b).
	//we want to modify them so that sqrt(dx ^ 2 + dy ^ 2) = the given length.
	dx = dx * length / glm::distance(a, b);
	dy = dy * length / glm::distance(a, b);
	glm::vec2 c;
	c.x = a.x + dx;
	c.y = a.y + dy;
	return c;
}
