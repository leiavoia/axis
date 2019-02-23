//#include "stlastar.h"
#include "mapsearch.h"
#include "map.h"



#include <iostream>
#include <stdio.h>


using namespace std;


// a pointer to the map.
Map* MapSearchNode::map;


// map pointer setter - used for all map search nodes.
void MapSearchNode::SetMapPointer(Map &incoming_map){
	map = &incoming_map;
	}




bool MapSearchNode::IsSameState( MapSearchNode &rhs ) {
	// same state in a maze search is simply when (x,y) are the same
	if( (x == rhs.x) && (y == rhs.y) ) { return true; }
	else { return false; }
	}



void MapSearchNode::PrintNodeInfo() {
	char str[100];
	sprintf( str, "Node position : (%d,%d)\n", x,y );
	cout << str;
	}



// Here's the heuristic function that estimates the distance from a Node to the Goal.
float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal ) {
	float xd = float( ( (float)x - (float)nodeGoal.x ) );
	float yd = float( ( (float)y - (float)nodeGoal.y) );
	return ((xd*xd) + (yd*yd));
	}



bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal ) {
	if( (x == nodeGoal.x) && (y == nodeGoal.y) ) { return true; }
	return false;
	}




// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node ) {

	//extern Map map;

	// dummy values if no parent - no negative coords on map :-)
	int parent_x = -1;
	int parent_y = -1; 

	if( parent_node ) {
		parent_x = parent_node->x;
		parent_y = parent_node->y;
		}

	MapSearchNode NewNode;

	// push each possible move except allowing the search to go backwards
	if( map->IsValid(x-1, y) && (map->GetObst(x-1, y) != 1)  &&  !((parent_x == x-1) && (parent_y == y))  ) {
		NewNode = MapSearchNode( x-1, y );
		astarsearch->AddSuccessor( NewNode );
		}

	if( map->IsValid(x, y-1) && (map->GetObst(x, y-1) != 1)  &&  !((parent_x == x) && (parent_y == y-1))  ) {
		NewNode = MapSearchNode( x, y-1 );
		astarsearch->AddSuccessor( NewNode );
		}

	if( map->IsValid(x+1, y) && (map->GetObst(x+1, y) != 1)  &&  !((parent_x == x+1) && (parent_y == y))  ) {
		NewNode = MapSearchNode( x+1, y );
		astarsearch->AddSuccessor( NewNode );
		}

	if( map->IsValid(x, y+1) && (map->GetObst(x, y+1) != 1)  &&  !((parent_x == x) && (parent_y == y+1))  ) {
		NewNode = MapSearchNode( x, y+1 );
		astarsearch->AddSuccessor( NewNode );
		}

	return true;
}



// given this node, what does it cost to move to successor. 
float MapSearchNode::GetCost( MapSearchNode &successor ) {
	// later on change the values for diff terrain types. for now, cost is just 1
	// use the MapSearchNode:: x/y coords for reference.
	return 1.0;
	}






