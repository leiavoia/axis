////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STL A* Search implementation
// (C)2001 Justin Heyes-Jones
//
// Finding a path on a simple grid maze
// This shows how to do shortest path finding using A*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stlastar.h"
#include "mapsearch.h"
#include "blotchmaker.h"
#include "map.h"

#include <iostream>
#include <stdio.h>
#include <time.h>

//#define DEBUG_LISTS 0
//#define DEBUG_LIST_LENGTHS_ONLY 0

using namespace std;




Map* map;
Blotchmaker* blotchmaker;



/////////// MAIN ///////////
int main( int argc, char *argv[] ) { // not that we actually have any command line args, but just in case...


//Seed the random numbers:
srand(time(NULL));



clock_t start = clock();



// DO 100 MAPS
for (int x = 0; x <100; x++){
	cout << "\nMap #" << x << "\n";

	//Map* map;
	map = new Map(300,300);

	//Blotchmaker* blotchmaker;
	blotchmaker = new Blotchmaker(*map);


	// SAMPLE CONFIG

	// Group vars:
	blotchmaker->SetMaxBlotches(0);
	blotchmaker->SetTypeChances(1,0); 

	// SLOT 0
	blotchmaker->SetBlotchType(0, 1);
	blotchmaker->SetTilesPerBlotch(0, 0);
	blotchmaker->SetModeChances(0, 0, 1, 0, 0);
	blotchmaker->SetTurnChances(0, 3, 1, 1, 0);
	blotchmaker->SetBlankShotChance(0, 0);
	blotchmaker->SetEdgeGrav(0, 0);
	blotchmaker->SetEdgeDet(0, 0);
	blotchmaker->SetBrush(0, RE);
	blotchmaker->SetStep(0, 1);

	// make blotches on map
	blotchmaker->StartBlotching();

	// print the prelim map before searching
	//map->PrintToScreen();

	delete map;
	delete blotchmaker;
	}





// UNCOMMENT THIS WHOLE BLOCK IF YOU WANT TO SEE A SEARCH IN ACTION:

/*

//Map* map;
map = new Map(50,50);

//Blotchmaker* blotchmaker;
blotchmaker = new Blotchmaker(*map);


// SAMPLE CONFIG

// Group vars:
blotchmaker->SetMaxBlotches(25);
blotchmaker->SetTypeChances(1,0); // lottery chances of each of the first two slots. you do not need to pad with zeros anymore :-)

// SLOT 0
blotchmaker->SetBlotchType(0, 1);
blotchmaker->SetTilesPerBlotch(0, 40);
blotchmaker->SetModeChances(0, 0, 1, 0, 0);
blotchmaker->SetTurnChances(0, 3, 1, 1, 0);
blotchmaker->SetBlankShotChance(0, 0);
blotchmaker->SetEdgeGrav(0, 0);
blotchmaker->SetEdgeDet(0, 0);
blotchmaker->SetBrush(0, RE);
blotchmaker->SetStep(0, 1);

// make blotches on map
blotchmaker->StartBlotching();





// Create an instance of the search class...
AStarSearch<MapSearchNode> astarsearch;

// set the pointer to the map
MapSearchNode::SetMapPointer(*map);

// Create a start state
MapSearchNode nodeStart(2,2);

// Define the goal state
MapSearchNode nodeEnd(47,47);

// Set Start and goal states
astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );

unsigned int SearchState;
unsigned int SearchSteps = 0;

do {
	SearchState = astarsearch.SearchStep();

	SearchSteps++;

#if DEBUG_LISTS

	cout << "Steps:" << SearchSteps << "\n";

	int len = 0;

	cout << "Open:\n";
	MapSearchNode *p = astarsearch.GetOpenListStart();
	while( p ) {
		len++;

#if !DEBUG_LIST_LENGTHS_ONLY
		//((MapSearchNode *)p)->PrintNodeInfo();
#endif

		p = astarsearch.GetOpenListNext();
		}

	cout << "Open list has " << len << " nodes\n";

	len = 0;

	cout << "Closed:\n";
	p = astarsearch.GetClosedListStart();
	while( p ) {
		len++;

#if !DEBUG_LIST_LENGTHS_ONLY
		//p->PrintNodeInfo();
#endif

		p = astarsearch.GetClosedListNext();
		}

	cout << "Closed list has " << len << " nodes\n";
#endif

	}

while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );



// if solution found, display:
if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED ) {
	cout << "Search found goal state\n";

	MapSearchNode *node = astarsearch.GetSolutionStart();

#if DISPLAY_SOLUTION
	cout << "Displaying solution\n";
#endif

	int steps = 0;

	//node->PrintNodeInfo();
	//also paint the map too
	map->SetObst(node->x, node->y, 2);
	for( ;; ) {
		node = astarsearch.GetSolutionNext();
		if( !node ) { break; }
		//node->PrintNodeInfo();
		//also paint the map too
		map->SetObst(node->x, node->y, 2);
		steps ++;
		};

	cout << "Solution steps " << steps << endl;

	// Once you're done with the solution you can free the nodes up
	astarsearch.FreeSolutionNodes();
	}
else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED ) {
	cout << "Search terminated. Did not find goal state\n";
	}



// Display the number of loops the search went through
cout << "SearchSteps : " << SearchSteps << "\n";








map->PrintToScreen();
*/


clock_t end = clock();

cout << "Generation time: " << end - start << "\n";

cout << "test: " << (end - start) / (CLOCKS_PER_SEC *1.0) << "\n";

cout << "CPS: " << CLOCKS_PER_SEC << "\n";
return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
