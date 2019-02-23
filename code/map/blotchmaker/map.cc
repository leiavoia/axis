/*
=================
MAP.cc
=================
version 0.1
[alpha]
=================

This is the main data structure for the game map. All Map does is hold the data for the map.
It does no create the terrain or anything else, just holds data.

To create the map content, use BlotchMaker class in conjunction with this one. First, create a map
with Map class, then create the BlotchMaker class and run it's main function.
*/

#include <iostream>
#include <stdio.h>
#include "map.h"

using namespace std;



//////////////////////////////////////////////////////////////////////////////////////////////////////////


Map::Map(short int cols, short int rows) {
// constructor

// set local vars for later use.
map_cols = cols;
map_rows = rows;

// build the dynamic map
the_map = new maptile *[cols]; // pointer array
for (int counter = 0; counter < cols; counter++) {
	the_map[counter] = new maptile [rows];
	}

// fill in the map --> automatically starts with EMPTY
for (int counter = 0; counter < map_rows; counter++ ) {
	for (int counterB = 0; counterB < map_cols; counterB++ ) {
		the_map[counterB][counter].obst = EMPTY;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


Map::~Map(void) { // destructor - kill map structure
for (int counter = 0; counter < map_cols; counter++) {
	delete[] the_map[counter];
	}
delete[] the_map;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


void Map::InitializeMap(short int initializer) {
// fill in the map
for (int counter = 0; counter < map_rows; counter++ ) {
	for (int counterB = 0; counterB < map_cols; counterB++ ) {
		the_map[counterB][counter].obst = obstacle(initializer);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


short int Map::GetRows(void) {
return map_rows;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


short int Map::GetCols(void) {
return map_cols;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


void Map::SetObst(short int x, short int y, short int type) {
the_map[x][y].obst = obstacle(type);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


short int Map::GetObst(short int x, short int y) {
return int(the_map[x][y].obst);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


bool Map::IsValid (short int x, short int y) {
// Returns 1 if a valid map coord is passed, zero if not.
// x/y coords
if (  (x < 0) || (x > (map_cols - 1))  )  {return 0;}
else if (  (y < 0) || (y > (map_rows - 1))  )  {return 0;}
return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////



void Map::PrintToScreen(void) {
//print the results
cout << '\n';
for (int counter = 0; counter < map_cols + 2; counter++) {cout << '*';}
cout << '\n';
for (int counter = map_rows-1; counter >= 0; counter-- ) {
	cout << '*';
	for (int counterB = 0; counterB < map_cols; counterB++ ) {
		if ( the_map[counterB][counter].obst == 0 ) {cout << ' ';}
		else if ( the_map[counterB][counter].obst == 1 ) {cout << 'X';}
		else if ( the_map[counterB][counter].obst == 2 ) {cout << '~';}
		else if ( the_map[counterB][counter].obst == 3 ) {cout << '+';}
		else if ( the_map[counterB][counter].obst == 4 ) {cout << 'u';}
		else if ( the_map[counterB][counter].obst == 5 ) {cout << '.';}
		else if ( the_map[counterB][counter].obst == 6 ) {cout << 'R';}
		else if ( the_map[counterB][counter].obst == 7 ) {cout << 'O';}
		else if ( the_map[counterB][counter].obst == 8 ) {cout << 'Y';}
		else if ( the_map[counterB][counter].obst == 9 ) {cout << 'G';}
		else if ( the_map[counterB][counter].obst == 10) {cout << 'B';}
		else if ( the_map[counterB][counter].obst == 11) {cout << 'V';}
		else if ( the_map[counterB][counter].obst == 12) {cout << 'W';}
		else if ( the_map[counterB][counter].obst == 13) {cout << 'B';}
		else {cout << '!';};
		}
	cout << "*\n";
	}
for (int counter = 0; counter < map_cols + 2; counter++) {cout << '*';}
cout << '\n';

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


void Map::dump(FILE* out) {
     //print the results
     fprintf(out, "\n");
     for (int counter = 0; counter < map_cols + 2; counter++) {
          fprintf(out, "*");
     }
     fprintf(out, "\n");
     for (int counter = 0; counter < map_rows; counter++ ) {
          fprintf(out, "*");
          for (int counterB = 0; counterB < map_cols; counterB++ ) {
		if ( the_map[counterB][counter].obst == 0 ) {fprintf(out, " ");}
		else if ( the_map[counterB][counter].obst == 1 ) {fprintf(out, "X");}
		else if ( the_map[counterB][counter].obst == 2 ) {fprintf(out, "~");}
		else if ( the_map[counterB][counter].obst == 3 ) {fprintf(out, "+");}
		else if ( the_map[counterB][counter].obst == 4 ) {fprintf(out, "u");}
		else if ( the_map[counterB][counter].obst == 5 ) {fprintf(out, "R");}
		else if ( the_map[counterB][counter].obst == 6 ) {fprintf(out, "O");}
		else if ( the_map[counterB][counter].obst == 7 ) {fprintf(out, "Y");}
		else if ( the_map[counterB][counter].obst == 8 ) {fprintf(out, "G");}
		else if ( the_map[counterB][counter].obst == 9 ) {fprintf(out, "B");}
		else if ( the_map[counterB][counter].obst == 10) {fprintf(out, "V");}
		else if ( the_map[counterB][counter].obst == 11) {fprintf(out, "W");}
		else if ( the_map[counterB][counter].obst == 12) {fprintf(out, "B");}
          }
          fprintf(out, "*\n");
     }
     for (int counter = 0; counter < map_cols + 2; counter++) {fprintf(out, "*");}
}




