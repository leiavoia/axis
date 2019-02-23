#include <iostream.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"
#include "blotchmaker.h"
#include "astar_oll.h"


// USER VARIABLES
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
static const bool verbose = 0;
static const short int rows_in_map = 40;
static const short int cols_in_map = 60;
enum obstacle map_initializer = empty;
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""




/////// MAIN START ///////////////////////////////////////////////////////////////////////////////////////////////////////
int main () {


//Seed the random numbers:
srand(time(NULL));









// creat a map object and start making blotches on it
//class Map the_map(cols_in_map, rows_in_map);

// make blotchmaker object
//class Blotchmaker blotchmaker(&the_map);




/*
// SAMPLE CONFIG # 1 w/ 2 slots
// "Bubble Chamber"

// reinitialize map to block instead of empty - let's erase!
the_map.InitializeMap(block);

// Group vars:
blotchmaker.SetMaxBlotches(20);
blotchmaker.SetTypeChances(4, 1, 0, 0, 0, 0, 0, 0, 0, 0); // lottery chances of each of the first two slots.

// SLOT 0 - configured for "curliqueous clear"
blotchmaker.SetBlotchType(0, 0); // obstacle zero = CLEAR mode
blotchmaker.SetTilesPerBlotch(0, 100);
blotchmaker.SetModeChances(0, 0, 1, 0, 0); //chaotic
blotchmaker.SetTurnChances(0, 20, 1, 1, 0); // mostly forward
blotchmaker.SetBlankShotChance(0, 0);
blotchmaker.SetEdgeGrav(0, 0);
blotchmaker.SetEdgeDet(0, 0);
blotchmaker.SetBrush(0, RE); // regular tile brush
blotchmaker.SetStep(0, 1);

// SLOT 1 - configured for "bubbles"
blotchmaker.SetBlotchType(1, 0); // obstacle zero = CLEAR mode
blotchmaker.SetTilesPerBlotch(1, 1);
blotchmaker.SetModeChances(1, 0, 1, 0, 0);
blotchmaker.SetTurnChances(1, 1, 0, 0, 0);
blotchmaker.SetBlankShotChance(1, 0);
blotchmaker.SetEdgeGrav(1, 0);
blotchmaker.SetEdgeDet(1, 0);
blotchmaker.SetBrush(1, MC); // med. circle brush
blotchmaker.SetStep(1, 1);
*/





/*
// SAMPLE CONFIG # 2 w/ 2 slots
// "AntiBubble Chamber - Moth Eaten Version"

// Group vars:
blotchmaker.SetMaxBlotches(20);
blotchmaker.SetTypeChances(2, 1, 0, 0, 0, 0, 0, 0, 0, 0); // lottery chances of each of the first two slots.

// SLOT 0 - configured for "curliqueous clear"
blotchmaker.SetBlotchType(0, 0); // obstacle zero = CLEAR mode
blotchmaker.SetTilesPerBlotch(0, 35);
blotchmaker.SetModeChances(0, 0, 1, 0, 0); //chaotic
blotchmaker.SetTurnChances(0, 20, 1, 1, 0); // mostly forward
blotchmaker.SetBlankShotChance(0, 0);
blotchmaker.SetEdgeGrav(0, 0);
blotchmaker.SetEdgeDet(0, 0);
blotchmaker.SetBrush(0, SP); // regular tile brush
blotchmaker.SetStep(0, 1);

// SLOT 1 - configured for "bubbles"
blotchmaker.SetBlotchType(1, 1); // obstacle 1 = BLOCK mode
blotchmaker.SetTilesPerBlotch(1, 1);
blotchmaker.SetModeChances(1, 0, 1, 0, 0);
blotchmaker.SetTurnChances(1, 1, 0, 0, 0);
blotchmaker.SetBlankShotChance(1, 0);
blotchmaker.SetEdgeGrav(1, 0);
blotchmaker.SetEdgeDet(1, 20);
blotchmaker.SetBrush(1, LC); // large circle brush
blotchmaker.SetStep(1, 1);
*/






// make blotches on map
//blotchmaker.StartBlotching();


//the_map.PrintToScreen();


// mess with OLLs

class oll list;

list.Add(5,7,5,1,8);
list.Add(6,2,5,2,7);
list.Add(9,1,6,6,9);
list.Add(3,1,9,4,3);
list.Add(7,1,4,7,8);

//list.Print();














return(0);
}
// END OF MAIN //////////////////////////////////////////////////////////////////////////////////////////////////////////////






