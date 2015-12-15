#pragma once

#include <glm.hpp>
#include <SpriteBatch.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent
{
public:
    Agent();
    virtual ~Agent();

    virtual void update(const std::vector<std::string>& levelData,
                        std::vector<Human*>& humans,
                        std::vector<Zombie*>& zombies);

    bool collideWithLevel(const std::vector<std::string>& levelData);

    bool collideWithAgent(Agent* agent);

    void draw(Bengine::SpriteBatch& _spriteBatch);
   
	void draw(Bengine::SpriteBatch& _spriteBatch, std::string texturePath);

    // Return true if we died
    bool applyDamage(float damage);

    glm::vec2 getPosition() const { return _position; }

	int getType() const { return _type;  }

protected:

    void checkTilePosition(const std::vector<std::string>& levelData,
                           std::vector<glm::vec2>& collideTilePositions,
                           float x, 
                           float y);

    void collideWithTile(glm::vec2 tilePos);
    
    glm::vec2 _position;
    Bengine::Color _color;
	float _health;
    float _speed;
	int _type;
};

