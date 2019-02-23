/*
=================
MAP.h
=================
version 0.1.2
=================

This is the main data structure for the game map.

To create the map content, use BlotchMaker class in conjunction with this one. First, create a map
with Map class, then create the BlotchMaker class and run it's main function.



CHANGLELOG:
--------------------
0.1.3 - reversed PrintToScreen so that 0,0 is in the bottom left corner.
0.1.2 - added road quality and fixed map intialaizer bug
0.1.1 - added additional obst. type enums and capitalized them as well. change code accordingly.



TO DO
-------------------
- bounds and error checking

*/



#include <stdio.h>

#ifndef MAP_H
#define MAP_H

// obstacle type - for use with maptiles
enum obstacle {
	// obstacles:
	EMPTY=0,
	BLOCK,	// 1
	WATER,	// 2
	PIT,	// 3
	BARRIER,// 4
	// special paint marker for tracking pathfinding or whatever:
	PAINT,	// 5
	// axis home base tiles:
	RED,	// 6
	ORANGE,	// 7
	YELLOW, // 8
	GREEN,	// 9
	BLUE,	// 10
	VIOLET, // 11
	WHITE,	// 12
	BLACK	// 13
	};



/***** MAP INTERFACE *****/

class Map {
	public:
		Map(short int cols, short int rows);
		~Map(void);
		void InitializeMap(short int initializer); // you can reinitialize it here, although it's initalized to EMPTY by default constructor
		short int GetRows(void); // gets the number of rows in the map
		short int GetCols(void); // gets the number of columns in the map
		void SetObst(short int x, short int y, short int type); // sets the obstacle type for a specific map cell at x,y coords
		short int GetObst(short int x, short int y);  // gets the obstacle type for a specific map cell at x,y coords
		bool IsValid (short int x, short int y);
		void PrintToScreen(void); // dump to screen
		void dump(FILE* out); //dump to file
	private:
		// map tile structure - the main gig
		struct maptile {
			enum obstacle obst; //obstacle type
			short int elev; // land elevation
			short int road_q; // road quality. 0-10, 10 = rough new terrain, 0 = well traveled road / fast.
			// occupied by
			// specials - linked list
			} **the_map; // the map pointer-pointer array
		short int map_cols;
		short int map_rows;
	};

#endif

