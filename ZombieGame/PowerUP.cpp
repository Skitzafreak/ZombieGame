#include "PowerUP.h"



PowerUP::PowerUP()
{
}


PowerUP::~PowerUP()
{
}

void PowerUP::init(glm::vec2 pos)
{
	_position = pos;
	_health = 999999;
}

void PowerUP::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{

}
