#include "Human.h"
#include <ctime>
#include <random>
#include <gtx/rotate_vector.hpp>
#include "Zombie.h"

Human::Human() : _frames(0) {

}


Human::~Human() {

}

void Human::init(float speed, glm::vec2 pos) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    _health = 20;

    _color.r = 200;
    _color.g = 0;
    _color.b = 200;
    _color.a = 255;

    _speed = speed;
    _position = pos;
    // Get random direction
    _direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    // Make sure direction isn't zero
    if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

    _direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);

	////////////////////////////////////////////
	//      make human flee code go here      //
	//            David Reynolds              //
	////////////////////////////////////////////

	//if zombie gets withing range of detection
	//human will flee zombie

	//-------------------------------------------------------
	Zombie* closestZombie = getNearestZombie(zombies);

	// If we found a zombie, move away him
	if (closestZombie != nullptr) {
		// Get the direction vector away the player
		glm::vec2 direction = glm::normalize(closestZombie->getPosition() - _position);
		_position -= direction * _speed;
	}
	else
	{
		_position += _direction * _speed;

		// Randomly change direction every 100 frames
		if (_frames == 100) {
			_direction = glm::rotate(_direction, randRotate(randomEngine));
			_frames = 0;
		}
		else {
			_frames++;
		}

		if (collideWithLevel(levelData)) {
			_direction = glm::rotate(_direction, randRotate(randomEngine));
		}
	}
	//end of else statement
	collideWithLevel(levelData);
	
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//detecting nearest Zombie
Zombie* Human::getNearestZombie(std::vector<Zombie*>& zombies) {
	Zombie* closestZombie = nullptr;
	float humanSight = 300.0f;

	for (int i = 0; i < zombies.size(); i++) {
		// Get the direction vector
		glm::vec2 distVec = zombies[i]->getPosition() - _position;
		// Get distance
		float distance = glm::length(distVec);

		// If this zombie is closer than the closest zombie, this is the new closest
		if (distance < humanSight) {
			humanSight = distance;
			closestZombie = zombies[i];
		}
	}

	return closestZombie;
}
//------------------------------------------------------------------------