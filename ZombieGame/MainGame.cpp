#include "MainGame.h"

#include <Bengine.h>
#include <Timing.h>
#include <Errors.h>
#include <random>
#include <ctime>

#include <SDL.h>
#include <iostream>

#include "Gun.h"
#include "Zombie.h"


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame()  :
    _screenWidth(1024),
    _screenHeight(768),
    _gameState(GameState::PLAY),
    _fps(0),
    _player(nullptr),
    _numHumansKilled(0),
    _numZombiesKilled(0) {
    // Empty
}

MainGame::~MainGame() {
    // Don't forget to delete the levels!
    for (int i = 0; i < _levels.size(); i++) {
        delete _levels[i];
    }
    // Don't forget to delete the humans and zombies!
    for (int i = 0; i < _humans.size(); i++) {
        delete _humans[i];
    }
    for (int i = 0; i < _zombies.size(); i++) {
        delete _zombies[i];
    }
}

void MainGame::run() {

    initSystems();

    initLevel();

    gameLoop();
}

void MainGame::initSystems() {
    // Initialize the game engine
    Bengine::init();

    // Create our window
    _window.create("ZombieGame", _screenWidth, _screenHeight, 0);

    // Grey background color
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    // Set up the shaders
    initShaders();

    // Initialize our spritebatch
    _agentSpriteBatch.init();

    // Set up the camera
    _camera.init(_screenWidth, _screenHeight);

}

void MainGame::initLevel() {
    // Level 1
    _levels.push_back(new Level("Levels/level1.txt"));
    _currentLevel = 0;

    _player = new Player();
    _player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

    _humans.push_back(_player);

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));

    std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2);
    std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);


	//Add ammo caches
	const std::vector<glm::vec2>& ammoPositions = _levels[_currentLevel]->getAmmoStartPositions();
	for (int i = 0; i < ammoPositions.size(); i++)
	{
		_ammo.push_back(new Ammo);
		_ammo.back()->init(ammoPositions[i]);
	}

	//Add powerUP tokens/Upgrades
	const std::vector<glm::vec2>& powerUPPositions = _levels[_currentLevel]->getPowerUPStartPosition();
	for (int i = 0; i < powerUPPositions.size(); i++)
	{
		_powerUPs.push_back(new PowerUP);
		_powerUPs.back()->init(powerUPPositions[i]);
	}

    // Add all the random humans
    for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
        _humans.push_back(new Human);
        glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
        _humans.back()->init(HUMAN_SPEED, pos);
    }

    // Add the zombies
    const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPositions();
    for (int i = 0; i < zombiePositions.size(); i++) {
        _zombies.push_back(new Zombie);
        _zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
    }




    // Set up the players guns
    const float BULLET_SPEED = 20.0f;
    _player->addGun(new Gun("Magnum", 10, 1, 0.01f, 30, BULLET_SPEED, 100, 1));
    _player->addGun(new Gun("Shotgun", 30, 12, 0.2f, 4, BULLET_SPEED, 12*10, 1));
    _player->addGun(new Gun("MP5", 2, 1, 0.1f, 20, BULLET_SPEED, 250, 1));
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
    
    Bengine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(60.0f);

    // Main loop
    while (_gameState == GameState::PLAY) {
        fpsLimiter.begin();

        checkVictory();

        processInput();
       
        updateAgents();

        updateBullets();

        _camera.setPosition(_player->getPosition());

        _camera.update();

        drawGame();

        _fps = fpsLimiter.end();
    }
}

void MainGame::updateAgents() {
    // Update all humans
    for (int i = 0; i < _humans.size(); i++) {
        _humans[i]->update(_levels[_currentLevel]->getLevelData(),
                           _humans,
                           _zombies);
    }

    // Update all zombies
    for (int i = 0; i < _zombies.size(); i++) {
        _zombies[i]->update(_levels[_currentLevel]->getLevelData(),
                           _humans,
                           _zombies);
    }


    // Update Zombie collisions
    for (int i = 0; i < _zombies.size(); i++) {
        // Collide with other zombies
        for (int j = i + 1; j < _zombies.size(); j++) {
            _zombies[i]->collideWithAgent(_zombies[j]);
        }
        // Collide with humans
        for (int j = 1; j < _humans.size(); j++) {
            if (_zombies[i]->collideWithAgent(_humans[j])) {
                // Add the new zombie
                _zombies.push_back(new Zombie);
                _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
                // Delete the human
                delete _humans[j];
                _humans[j] = _humans.back();
                _humans.pop_back();
            }
        }
		/*
		// Collide with ammo caches
		for (int j = 0; j < _ammo.size(); j++)
		{
			_zombies[i]->collideWithAgent(_ammo[j]);
		}

		//Collide with powerUP tokens
		for (int j = 0; j < _powerUPs.size(); j++)
		{
			_zombies[i]->collideWithAgent(_powerUPs[j]);
		}
		*/



        // Collide with player
        if (_zombies[i]->collideWithAgent(_player)) {
            Bengine::fatalError("YOU LOSE");
        }
    }

    // Update Human collisions
    for (int i = 0; i < _humans.size(); i++) {
        // Collide with other humans
        for (int j = i + 1; j < _humans.size(); j++) {
            _humans[i]->collideWithAgent(_humans[j]);
        }
    }

	//Player collides with ammo caches
	for (int i = 0; i < _ammo.size(); i++)
	{
		if (_ammo[i]->collideWithAgent(_player))
		{
			_player->getGun()->addAmmo(50);
			std::cout << "50 bullets added.\n" << std::endl;
			std::cout << "Your " << _player->getGun()->getName() << " has " << _player->getGun()->getAmmo() << " bullets left.\n" << std::endl;
			delete _ammo[i];
			_ammo[i] = _ammo.back();
			_ammo.pop_back();
		}
	}

	//Player collides with an PowerUP token
	for (int i = 0; i < _powerUPs.size(); i++)
	{
		if (_powerUPs[i]->collideWithAgent(_player))
		{
			_player->getGun()->upgrade();
			std::cout << "Your " << _player->getGun()->getName() << " has been upgraded\n" << std::endl;
			delete _powerUPs[i];
			_powerUPs[i] = _powerUPs.back();
			_powerUPs.pop_back();
		}
	}

    // Dont forget to update zombies
}

void MainGame::updateBullets() {

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int> rand(0, 9);

	int number = rand(randomEngine);


    // Update and collide with world
    for (int i = 0; i < _bullets.size(); ) {
        // If update returns true, the bullet collided with a wall
        if (_bullets[i].update(_levels[_currentLevel]->getLevelData())) {
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        } else {
            i++;
        }
    }

    bool wasBulletRemoved;

    // Collide with humans and zombies
    for (int i = 0; i < _bullets.size(); i++) {
        wasBulletRemoved = false;
        // Loop through zombies
        for (int j = 0; j < _zombies.size(); ) {
            // Check collision
            if (_bullets[i].collideWithAgent(_zombies[j])) {
                // Damage zombie, and kill it if its out of health
                if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
                    // If the zombie died, remove him
					if (number > 8)
					{
						_powerUPs.push_back(new PowerUP);
						_powerUPs.back()->init(_zombies[j]->getPosition());
					}
					else if (number < 2)
					{
						_ammo.push_back(new Ammo);
						_ammo.back()->init(_zombies[j]->getPosition());
					}

                    delete _zombies[j];
                    _zombies[j] = _zombies.back();
                    _zombies.pop_back();
                    _numZombiesKilled++;
                } else {
                    j++;
                }

                // Remove the bullet
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
                wasBulletRemoved = true;
                i--; // Make sure we don't skip a bullet
                // Since the bullet died, no need to loop through any more zombies
                break;
            } else {
                j++;
            }
        }
        // Loop through humans
        if (wasBulletRemoved == false) {
            for (int j = 1; j < _humans.size(); ) {
                // Check collision
                if (_bullets[i].collideWithAgent(_humans[j])) {
                    // Damage human, and kill it if its out of health
                    if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
                        // If the human died, remove him
                        delete _humans[j];
                        _humans[j] = _humans.back();
                        _humans.pop_back();
                    } else {
                        j++;
                    }

                    // Remove the bullet
                    _bullets[i] = _bullets.back();
                    _bullets.pop_back();
                    _numHumansKilled++;
                    i--; // Make sure we don't skip a bullet
                    // Since the bullet died, no need to loop through any more zombies
                    break;
                } else {
                    j++;
                }
            }
        }
    }
}

void MainGame::checkVictory() {
    // TODO: Support for multiple levels!
    // _currentLevel++; initLevel(...);

    // If all zombies are dead we win!
    if (_zombies.empty()) {
        // Print victory message
        std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining",
                    _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());

        // Easy way to end the game :P
        Bengine::fatalError("");
    }
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureProgram.use();

    // Draw code goes here
    glActiveTexture(GL_TEXTURE0);

    // Make sure the shader uses texture 0
    GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    glm::mat4 projectionMatrix = _camera.getCameraMatrix();
    GLint pUniform = _textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Draw the level
    _levels[_currentLevel]->draw();

    // Begin drawing agents
    _agentSpriteBatch.begin();


	// Draw ammo caches
	for (int i = 0; i < _ammo.size(); i++)
	{
		_ammo[i]->draw(_agentSpriteBatch, "Textures/ammo.png");
	}

	//Draw PowerUP tokens
	for (int i = 0; i < _powerUPs.size(); i++)
	{
		_powerUPs[i]->draw(_agentSpriteBatch, "Textures/upgrade.png");
	}

    // Draw the humans
    for (int i = 0; i < _humans.size(); i++) {
        _humans[i]->draw(_agentSpriteBatch);
    }

    // Draw the zombies
    for (int i = 0; i < _zombies.size(); i++) {
        _zombies[i]->draw(_agentSpriteBatch);
    }

    // Draw the bullets
    for (int i = 0; i < _bullets.size(); i++) {
        _bullets[i].draw(_agentSpriteBatch);
    }



    // End spritebatch creation
    _agentSpriteBatch.end();

    // Render to the screen
    _agentSpriteBatch.renderBatch();

    // Unbind the program
    _textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}
