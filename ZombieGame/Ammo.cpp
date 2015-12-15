#include "Ammo.h"

Ammo::Ammo()
{

}

Ammo::~Ammo()
{

}

void Ammo::init(glm::vec2 pos)
{
	_speed = 0;
	_position = pos;
	_color.r = 100;
	_color.g = 160;
	_color.b = 200;
	_color.a = 255;
	_health = 999999;
}

void Ammo::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{

}
