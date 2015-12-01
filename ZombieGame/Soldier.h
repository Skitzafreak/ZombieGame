#pragma once
#include "Human.h"
#include "Bullet.h"

enum class SoldierState { ROAM, ATTACK };

class Soldier : public Human
{
public:
	Soldier();
	~Soldier();

	void init(float speed, glm::vec2 pos, std::vector<Bullet>* bullets, std::vector<glm::vec2> waypoints);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies) override;

private:
	std::vector<Bullet>* _bullets;
	std::vector<glm::vec2> _waypoints;
	int _currentWaypoint;
	float _detectRadius;
	SoldierState _state;
};
