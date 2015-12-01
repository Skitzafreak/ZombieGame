#include "Soldier.h"

const float TILE_OFFSET = 2.0f;

Soldier::Soldier()
{
}
Soldier::~Soldier()
{
}

void Soldier::init(float speed, glm::vec2 pos, std::vector<Bullet>* bullets, std::vector<glm::vec2> waypoints)
{
	_speed = speed;
	_position = pos;
	_bullets = bullets;
	_waypoints = waypoints;
	_currentWaypoint = 0;
	_detectRadius = 1.0f;
	_state = SoldierState::ROAM;
	_color.r = 255;
	_color.g = 215;
	_color.b = 0;
	_color.a = 255;
	_health = 100;
}

void Soldier::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies)
{
	if (_state == SoldierState::ROAM)
	{
		_direction = glm::normalize(_waypoints[_currentWaypoint] - _position);
		_position += _direction * _speed;

		if (_position.x + TILE_OFFSET >= _waypoints[_currentWaypoint].x && _position.x - TILE_OFFSET <= _waypoints[_currentWaypoint].x &&
			_position.y + TILE_OFFSET >= _waypoints[_currentWaypoint].y && _position.y - TILE_OFFSET <= _waypoints[_currentWaypoint].y)
			_currentWaypoint++;
		if (_currentWaypoint == _waypoints.size())
			_currentWaypoint = 0;
	}
}
