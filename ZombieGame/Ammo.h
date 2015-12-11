#pragma once

#include "Agent.h"

class Ammo : public Agent
{
public:
	Ammo();
	~Ammo();
	
	void init(glm::vec2 pos);
	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

};
