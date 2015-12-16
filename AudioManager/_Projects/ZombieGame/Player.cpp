#include "Player.h"
#include <SDL.h>

#include "Gun.h"

Player::Player() :
    _currentGunIndex(-1) {
    // Empty
}

Player::~Player() {
    // Empty
}

void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets) {
    _speed = speed;
    _position = pos;
    _inputManager = inputManager;
    _bullets = bullets;
    _camera = camera;
    _color.r = 0;
    _color.g = 0;
    _color.b = 185;
    _color.a = 255;
    _health = 150;

	_switch_sound_playing = false;
	_switch_sound_framecount_reset = 20;
	_switch_frame_counter = 0;

	_walk_sound_playing = false;
	_walk_framecount_reset = 40;
	_walk_frame_counter = 0;
;}

void Player::addGun(Gun* gun) {
    // Add the gun to his inventory
    _guns.push_back(gun);

    // If no gun equipped, equip gun.
    if (_currentGunIndex == -1) {
        _currentGunIndex = 0;
    }
}

void Player::update(const std::vector<std::string>& levelData,
                    std::vector<Human*>& humans,
                    std::vector<Zombie*>& zombies) {
	if (_switch_sound_playing)
	{
		_switch_frame_counter++;

		if (_switch_frame_counter >= _switch_sound_framecount_reset)
		{
			_switch_frame_counter = 0;
			_switch_sound_playing = false;
		}
	}

	if (_walk_sound_playing)
	{
		_walk_frame_counter++;

		if (_walk_frame_counter >= _walk_framecount_reset)
		{
			_walk_frame_counter = 0;
			_walk_sound_playing = false;
		}
	}

    if (_inputManager->isKeyPressed(SDLK_w)) {
		if (!_walk_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/walk.wav", 0.6f);
			_walk_sound_playing = true;
		}
        _position.y += _speed;
    } else if (_inputManager->isKeyPressed(SDLK_s)) {
		if (!_walk_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/walk.wav", 0.6f);
			_walk_sound_playing = true;
		}
        _position.y -= _speed;
    }
    if (_inputManager->isKeyPressed(SDLK_a)) {
		if (!_walk_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/walk.wav", 0.6f);
			_walk_sound_playing = true;
		}
        _position.x -= _speed;
    } else if (_inputManager->isKeyPressed(SDLK_d)) {
		if (!_walk_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/walk.wav", 0.6f);
			_walk_sound_playing = true;
		}
        _position.x += _speed;
    }

    if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0) {
        _currentGunIndex = 0;
		if (!_switch_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/switch_weapon.wav", 0.1f);
			_switch_sound_playing = true;
		}
    } else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1) {
        _currentGunIndex = 1;
		if (!_switch_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/switch_weapon.wav", 0.1f);
			_switch_sound_playing = true;
		}
    } else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2) {
        _currentGunIndex = 2;
		if (!_switch_sound_playing)
		{
			Bengine::AudioManager::getInstance()->playOnce("Sounds/switch_weapon.wav", 0.1f);
			_switch_sound_playing = true;
		}
    }

    if (_currentGunIndex != -1) {

        glm::vec2 mouseCoords = _inputManager->getMouseCoords();
        mouseCoords = _camera->convertScreenToWorld(mouseCoords);

        glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

        glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

        _guns[_currentGunIndex]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT),
                                        centerPosition,
                                        direction,
                                        *_bullets);
                                        

    }

    collideWithLevel(levelData);
}
