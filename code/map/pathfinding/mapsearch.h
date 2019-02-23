#include "stlastar.h"
#include "map.h"



using namespace std;




// Definitions

class MapSearchNode
{
public:
	/*unsigned*/ int x;	 // the (x,y) positions of the node
	/*unsigned*/ int y;

	// constructors:
	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py ) { x=px; y=py; }

	// manipulation funcs:
	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );

	// map pointer funcs:
	static void SetMapPointer(Map &incoming_map);


	void PrintNodeInfo();


private:

	static Map* map; // a pointer to the map we are working on. static makes it available to all nodes in use.

};

