#pragma once

#include "Human.h"
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>
#include <Bengine/AudioManager.h>
#include "Bullet.h"

class Gun;

class Player : public Human
{
public:
    Player();
    ~Player();

    void init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets);

    void addGun(Gun* gun);

    void update(const std::vector<std::string>& levelData,
                std::vector<Human*>& humans,
                std::vector<Zombie*>& zombies) override;
private:
    Bengine::InputManager* _inputManager;

    std::vector<Gun*> _guns;
    int _currentGunIndex;

	bool _switch_sound_playing;
	int _switch_sound_framecount_reset;
	int _switch_frame_counter;

	bool _walk_sound_playing;
	int _walk_framecount_reset;
	int _walk_frame_counter;

    Bengine::Camera2D* _camera;
    std::vector<Bullet>* _bullets;
};

