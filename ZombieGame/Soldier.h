#pragma once
#include "Human.h"
#include "Zombie.h"
#include "Gun.h"
#include "Bullet.h"

enum class SoldierState { ROAM, CHASE, ATTACK };

class Soldier : public Human
{
public:
	Soldier();
	~Soldier();

	void init(float speed, glm::vec2 pos, std::vector<Bullet>* bullets, std::vector<glm::vec2> waypoints);
	void update(const	std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) override;
	void addGun(Gun* gun);

private:
	Agent* getNearestAgent(std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
	glm::vec2 normalize(glm::vec2 a, glm::vec2 b, float length);

	Gun* _gun;
	std::vector<Bullet>* _bullets;
	std::vector<glm::vec2> _waypoints;
	int _currentWaypoint;
	float _detectRadius;
	float _shootRadius;
	SoldierState _state;
	
	Agent* _target;
	bool _targetAquired;
};
