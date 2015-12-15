#pragma once

#include <string>
#include <vector>

#include <SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
    // Load the level
    Level(const std::string& fileName);
    ~Level();

    void draw();

    // Getters
    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    int getNumHumans() const { return _numHumans; }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
    const std::vector<glm::vec2>& getZombieStartPositions() const { return _zombieStartPositions; }
<<<<<<< HEAD
	const std::vector<glm::vec2>& getSoldierPath() const { return _soldierPathWaypoints; }

=======
	const std::vector<glm::vec2>& getAmmoStartPositions() const { return _ammoStartPositions; }
	const std::vector<glm::vec2>& getPowerUPStartPosition() const { return _powerUPstartPositions; }
>>>>>>> refs/remotes/origin/Dajh
private:
    std::vector<std::string> _levelData;
    int _numHumans;
    Bengine::SpriteBatch _spriteBatch;

    glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _ammoStartPositions;
    std::vector<glm::vec2> _zombieStartPositions;
<<<<<<< HEAD
	std::vector<glm::vec2> _soldierPathWaypoints;
=======
	std::vector<glm::vec2> _powerUPstartPositions;
>>>>>>> refs/remotes/origin/Dajh
};

