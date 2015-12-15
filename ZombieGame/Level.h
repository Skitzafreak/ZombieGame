#pragma once

#include <string>
#include <vector>

#include <SpriteBatch.h>

#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "GraphAlgorithms.h"
#include "AStarHeuristicPolicies.h"
//#include "HandyGraphFunctions.h"

const int TILE_WIDTH = 64;

class Level
{
public:
    // Load the level
    Level(const std::string& fileName);
    ~Level();

    void draw();

	void buildGraph();

    // Getters
    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    int getNumHumans() const { return _numHumans; }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
    const std::vector<glm::vec2>& getZombieStartPositions() const { return _zombieStartPositions; }


	//the terrain type of each cell
	std::vector<int>              m_TerrainType;
	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;
	
	NavGraph*                     m_pGraph;
	std::vector<int> wavePoints;

private:
    std::vector<std::string> _levelData;
    int _numHumans;
    Bengine::SpriteBatch _spriteBatch;

    glm::vec2 _startPlayerPos;
    std::vector<glm::vec2> _zombieStartPositions;
};

