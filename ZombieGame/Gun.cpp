#include "Gun.h"
#include <random>
#include <ctime>
#include <gtx/rotate_vector.hpp>


Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, int bullets, int upgrade) :
_name(name),
_fireRate(fireRate),
_bulletsPerShot(bulletsPerShot),
_spread(spread),
_bulletDamage(bulletDamage),
_bulletSpeed(bulletSpeed),
_ammo(bullets),
_upgrades(upgrade),
_frameCounter(0){
	// Empty
}


Gun::~Gun() {
	// Empty
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
	_frameCounter++;
	// After a certain number of frames has passed we fire our gun
	if (_frameCounter >= _fireRate && isMouseDown) {
		fire(direction, position, bullets);
		_frameCounter = 0;
	}
}

std::string Gun::getName()
{
	return _name;
}

void Gun::upgrade()
{
	if (_upgrades < 3)
		_upgrades++;
}

int Gun::getAmmo()
{
	return _ammo;
}

void Gun::addAmmo(int ammo)
{
	_ammo += ammo;
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets) {

	static std::mt19937 randomEngine(time(nullptr));
	// For offsetting the accuracy
	std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	if (_ammo >= (_bulletsPerShot))
	{
		for (int i = 0; i < _bulletsPerShot; i++) {
			// Add a new bullet
			bullets.emplace_back(position,
				glm::rotate(direction, randRotate(randomEngine)),
				_bulletDamage + (_upgrades * (12 / _bulletsPerShot)),
				_bulletSpeed / _upgrades);
			_ammo--;
		}
	}
}
