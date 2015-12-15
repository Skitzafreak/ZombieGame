#include "Soldier.h"



Soldier::Soldier(){
}

Soldier::~Soldier(){
}

void Soldier::init(float speed, std::vector<glm::vec2*>& wavePoints, bool leader){
	_speed = speed;
	_position = *wavePoints[0];
	_health = 100;
	_wavePoints = wavePoints;
	_leader = leader;
	

}
void Soldier::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	std::vector<Soldier*>& soldiers){



	//create a couple of typedefs so the code will sit comfortably on the page   
	//typedef Graph_SearchAStar<NavGraph, Heuristic_Euclid> AStarSearch;

	//create an instance of the A* search using the Euclidean heuristic
	//AStarSearch AStar(*m_pGraph, m_iSourceCell, m_iTargetCell);





	if (_leader == true){
		//::Graph_SearchAStar<
		//astar (_wavePoints[i], _position)
		//glm::vec2 direction = glm::normalize(astar.nextNode - _position);
		//_position += direction * _speed;

		//if _position == astar.nextNode
			//astar.goToNextNextNode

		//if _position == _wavePoints[i]
			//if _wavePoints[i] is the last
				//i = 0
			//else
				//i++
	}
	//if leader == false
		//if leader._position + offset is walkable
			//astar (leader._position + offset, this._position)

			//get Astar lanth 
			//if lanth > 3 
				//walkable = false

			//if lanth <= 3
				//walkable = true
		//else
			//walkable = false

		//if walkable
			//glm::vec2 direction = glm::normalize(astar.nextNode - _position);
			//_position += direction * _speed;
		//else
			//astar (leader.last_position, this._position)
			//glm::vec2 direction = glm::normalize(astar.nextNode - _position);
			//_position += direction * _speed;

		//if _position == astar.nextNode
			//astar.goToNextNextNode










	/*switch (state)
	{
	case Soldier::Stat::MOVE:

		break;

	default:
		break;
	}*/

}
