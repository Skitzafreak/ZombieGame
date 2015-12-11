#pragma once

#include "Agent.h"

class PowerUP : public Agent
{
public:
	PowerUP();
	~PowerUP();

	void init(glm::vec2 pos); 
	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);
};

