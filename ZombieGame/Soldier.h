#pragma once

#include "Agent.h"
//#include "AStar.h"
#include <vector>
#include <Bengine.h>
//#include "Zombie.h"
//#include "Human.h"

#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "GraphAlgorithms.h"
#include "AStarHeuristicPolicies.h"
//#include "HandyGraphFunctions.h"


class Soldier : public Agent
{
public:
	Soldier();
	~Soldier();

	enum Stat { MOVE, FORMATION, FIRE };
	unsigned int state;

	void init(float speed, std::vector<glm::vec2*>& wavePoints, bool leader);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		std::vector<Soldier*>& soldiers);
	
	std::vector<glm::vec2*> _wavePoints;
	bool _leader;
	glm::vec2 offset;

	typedef SparseGraph<NavGraphNode<void*>, GraphEdge> NavGraph;
	NavGraph*                     m_pGraph;
	 //the indices of the source and target cells
	  int                           m_iSourceCell,
	                                m_iTargetCell;

	//glm::vec2 OffsetPursuit(const Soldier* leader, const glm::vec2 offset);
	/*SVector2D SteeringBehaviors::OffsetPursuit(const Vehicle* leader,
const SVector2D offset)
{
//calculate the offset’s position in world space
SVector2D WorldOffsetPos = PointToWorldSpace(offset,
leader->Heading(),
leader->Side(),
leader->Pos());
SVector2D ToOffset = WorldOffsetPos - m_pVehicle->Pos();
//the look-ahead time is proportional to the distance between the leader
//and the pursuer; and is inversely proportional to the sum of both
//agents’ velocities
double LookAheadTime = ToOffset.Length() /
(m_pVehicle->MaxSpeed() + leader->Speed());
//now arrive at the predicted future position of the offset
return Arrive(WorldOffsetPos + leader->Velocity() * LookAheadTime, fast);
}*/
private:

	Human* getNearestHuman(std::vector<Human*>& humans);
	Zombie* getNearestZombie(std::vector<Zombie*>& humans);
};