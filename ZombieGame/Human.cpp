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

	_detectionRange = 4.0f;
	_sighted = 1;
	_inSight = false;

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

	Zombie* closestZombie = getNearestZombie(zombies);

	if (closestZombie != nullptr) {
		glm::vec2 direction = glm::normalize(closestZombie->getPosition() - _position);
		_position -= direction * _speed;
	}
	else {
		//// Randomly change direction every 20 frames
		if (_frames == 50) {
			_direction = glm::rotate(_direction, randRotate(randomEngine));
			_frames = 0;
		}
		else {
			_frames++;
		}
		_position += _direction * _speed;
	}

	//Horde of Zombies
	/*_position += _direction * _speed;

	for (int j = 0; j < zombies.size(); j++)
	{
		if (zombies[j]->getPosition().x - this->getPosition().x < _detectionRange && zombies[j]->getPosition().y - this->getPosition().y < _detectionRange)
		{
			if (!_inSight) {
				_speed -= 0.2;
				_sighted++;
				_inSight = true;
			}
			_escapeRoute = zombies[j]->getPosition() - this->getPosition();
			
			_direction.x = (_escapeRoute.x / _sighted);
			_direction.y = (_escapeRoute.y / _sighted);
		}
		else {
			if (_inSight) {
				_speed += 0.2;
				_sighted--;
				_inSight = false;
			}
		}
		_position += _direction * _speed;
	}*/

    if (collideWithLevel(levelData)) {
        _direction = glm::rotate(_direction, randRotate(randomEngine));
    }
}

Zombie* Human::getNearestZombie(std::vector<Zombie*>& zombies) {
	Zombie* closestZombie = nullptr;
	float smallestDisance = 500.0f;
	glm::vec2 distVec;
	float distance;

	for (int i = 0; i < zombies.size(); i++) {
		distVec = zombies[i]->getPosition() - _position;
		distance = glm::length(distVec);

		if (distance < smallestDisance) {
			smallestDisance = distance;
			closestZombie = zombies[i];
		}
	}

	return closestZombie;
}