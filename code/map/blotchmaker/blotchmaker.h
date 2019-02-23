
/*
====================
BlotchMaker.h
====================
version 2.5.9
====================

BlotchMaker works with "Map" classes. While Map builds the map structure for the game,
BltochMaker fills it all out with terrain.

To use Blotchmaker, first create a map object, then a Blotchmaker object. Use the main
blotchmaking function. It will automagically interact with the map object and create a terrain on it.

BlotchMaker has a default setup which you should change through a UI eventually.
There is a variety of functions for manipulating
the variables. BlotchMaker works off of a set of variables bunched into slots.
There are at present 5 slots to work with. Each slot is configured independently. There is a random chance
for a blotch to be placed for each slot of variables ("blotch types") which is also definable. See the class public
functions for more details.



SETUP SAMPLE:
----------------------
// creat a map object
class Map the_map(cols_in_map, rows_in_map);

// make blotchmaker object
class Blotchmaker blotchmaker(the_map);

// make blotches on map
blotchmaker.StartBlotching();

// you can also use this Map debug feature on a terminal window to get a character printout of the map:
the_map.PrintToScreen();




TO-DO & EXPANSION LIST
----------------------
get rid of redundant IsValid map checking.
SetAxisPalette() needs individual push and pop instead of a lump set of perams.
option: pathcut bad maps or re-blotch & timeout?
Path cutting for bad maps
Waypoints for path cutting
FULL mode infinite loop possible? or just get rid of full mode - check to see if still works even.
Expand slots from 5 to 10 (not so easy)
Revamp brush selection to include thickness/size perams
Remove templates package and just make them direct class members.
Code cleanup: Rename "blotch_type" to "slot". this is confusing.
Bounds checking and errors
get rid of extra variables like num_blotch_types. set to hard code now that design is done or switch to #defines.




CHANGELOG:
----------------------
2.5.9	- fixed *another* reporting bug saying bad maps were good. caused by no default resolution in verification.
	- fixed a bug in the main attempts loop that said the last try was bad if it was actually a success
	- put SearchSteps timeout back to it's old formula with some modification. It now times out after 1/8 of the map area is searched. as a result, maps that are technically good may be thrown out as a sacrifice to verification speed. This change has slowed down the algorithm quite a bit. On simple maps, you can get away with a low timeout, but on larger more complicated maps you cannot and the algorithm must search more to find the goal around many obstacles.
	- reduced attempts to 3 from 5.
	- slight optimization to verification loop for repetetive calculations
	- fixed a homebase placement bug. some HBs were not squarely in the corners.
2.5.8	- fixed reporting bug that said a bad map was good. caused by lowering attempts from 10 to 5.
2.5.7	- optimized the SearchSteps map verification timeout. It now takes less searched tiles to call it a bad map.
2.5.6	- added a few new private assistant variables:
		hb_locs to record base locations
		num_players for an easy reference, recorded automatically after SetAxisPalette() has been invoked
	- changed CreateHBs(). it now places the bases in patterns according the the number of players in the game.
	- VerifyMap() now uses less searches, depending on the number of players in the game. *Major speed improvement*
	- slight change to the header file in SetAxisPalette(). Put var=0 defaults in the function proto instead of in the actual function :-)
	- changed map attempts timout to 5 tries instead of 10
2.5.5	- BM makes 10 attempts to make a good map with the current settings.
	- the main munction StartBlotching() will return a zero if nothing could be produced and a 1 if it could.
2.5.4	- preliminary home base placement set up.
	- map verification via stlastarsearch.h and mapsearch.h
	- blotchmaker interface change: BM constructor now takes the actual map as a perameter.
		example: BlotchMaker bm(map);
		instead of: BlotchMaker bm(&map);
		as it used to.
	- new features for home base placement include:
		-> hbsize is the square size in tiles. this will probably be replaced by radius in future versions
		-> hbplacement (not implemented) - this will be for *how* HBs are placed - corners edges, etc.
		-> CreatHBs();
		-> VerifyMap();
		-> SetAxisPalette();
	- removed auto-allocation of some vars. this is a deprecated feature replaced by standard defaults at initialization time.
	- removed Hash brush types from the brushes enum
2.5.3	- implemented Brush, Step, and Deterioration features.
2.5.2	- ditched old template-input system and put it into user-usable Set() functions which the prog can also use.
2.5.1	- switched template blotch_types to array instead of hardcoded types
< 2.5.0	- lost to history and time

===============================================================
*/



#include "map.h"




// single tile position structure
struct pos {
	short int x;
	short int y;
	};

//blotch modes for blotch tile placement
enum blotch_modes {full=0, chaotic=1, x_or=2, clear=3} ;
	// full - places all tiles regardless
	// chaotic - counts all attempts to place tiles as tiles actually placed.
	// x_or - places blocks if tile is empty, otherwise empties it.
	// clear - empties a tile if it contains an obstacle.

// brush types - NOTE: not all types are implemented and the brush system may change in future versions.
enum brush {RE=0, SC, MC, LC, SS, MS, LS, SX, MX, LX, SP, MP, LP, SB, MB, LB, LR, MR, SR};
	// RE = [RE]gular single tile (default)
	// first letter:
	//	[S]mall, [M]edium, [L]arge
	// second letter:
	// 	[C]ircle, [S]quare, [X]-mark, [P]lus-mark, [B]ox (hollow square), [R]ing

// a little data module to fit inside the blotch template below
// aka a "slot"
struct blotch_type_perams {
	short int tiles_per_blotch; // zero for AUTO
	float mode_chances[4]; // Mode Persuasion (in parts): Full, Chaotic, X_OR, Clear
	float turn_chances[4]; // Turn Persuasion (in parts): Forward, Left, Right, Back
	short int blank_shot_chance; // Blank Shot (percentage chance of shooting a blank per tile)
	short int edge_det; // percent chance of edges of path to "shoot a blank"
	short int edge_grav; // value of the edge attraction. usually 1-5. zero for none;
	short int blotch_type; // just to call it something later when placing tiles, determining which kind of obstacle to place
	enum brush brush; // brush type for tile placement
	short int step;  // number of steps in between tile placements
	};

// "blotch variables template"
// aka "slot holder" + blotching global vars
struct map_user_vars_package {
	int max_blotches; // max number of blotches per blotch_type
	float type_chances[5]; // blotch type (aka "slot") chances
	// one set of vars for each mode
	struct blotch_type_perams type[5]; // NOTE: you only get 5 slots to work with! Maybe more in future
	};





/***** BLOTCHMAKER INTERFACE *****/

class Blotchmaker {
public:
	// BLOTCHING FUNCTIONS
		// Main function: start making blotches!
		// returns 0 if settings could not produce a verified map.
		bool StartBlotching();

		// sets the initializer obstacle blotchmaker will reset the map to when doing a new map.
		void SetMapInit(short int x);

		// Template vars functions - use these from the UI or internally, i don't care.
		//	defaults are set if you set nothing yourself
		// Note for perams: "type" is a short int corresponding to the "slot" (0-4)

		// Sets the tiles to be placed per blotch for this slot
		void SetTilesPerBlotch(short int type, short int x);
		// Sets the chances for a blotch placement mode in lottery balls (see above blotch_mode enum for details)
		void SetModeChances(short int type, short int full, short int chaotic, short int x_or, short int clear);
		// Sets the chances for the blotch leader to turn different direction in lottery balls
		void SetTurnChances(short int type, short int forward, short int left, short int right, short int back);
		// The chance as a percentage ("10" = ten percent) to "shoot a blank" and skip a blotch entirely.
		void SetBlankShotChance(short int type, short int x);
		// Sets the gravity of the wall edges which will attract the first tile placement in a blotch (usually -10 to 10)
		//	negative will push away from the walls while positive will attract.
		void SetEdgeGrav(short int type, short int x);
		// Deterioration. Chance as a percentage ("10" = ten percent) to shoot a blank for and individual tile inside a brushed placement of a blotch
		void SetEdgeDet(short int type, short int x);
		// brush type for a tile placement. See above brush enum for details
		void SetBrush(short int type, enum brush brush);
		// Number of map tiles to skip in between tiles placed by a blotch.
		void SetStep(short int type, short int x);
		// The kind of obstacle to place while blotching for this blotch_type slot. See the obstacle enum of the Map class for additional info.
		void SetBlotchType(short int type, short int x);
		// The max number of blotches for the entire blotching process
		void SetMaxBlotches(short int x);
		// The chance to select a variables slot (this function has additional perams for future expansion.
		//	Pads with zeros for nonexistant slots. currently, only perams 0-4 are needed as there are only 5 available slots to work with internally)
		void SetTypeChances(short int a, short int b=0, short int c=0, short int d=0, short int e=0, short int f=0, short int g=0, short int h=0, short int i=0, short int j=0);

	// OTHER FUNCTIONS
		// set the square size of home bases in tiles
		void SetHBSize(short int x);
		// set the placement of home bases. 0=Corners, 1=Sides, [future: 2=random?]
		void SetHBPlacement(short int x);
		// loads the axis colors to place on the map - ints corespond to the equivelent obstacle enum numbers. (0 = BLOCK)
		//	you cannot load colors seperately yet. you must do it all in one big lump.
		//	this also determines how many players are on the map.
		void SetAxisPalette(short int=0, short int=0, short int=0, short int=0, short int=0, short int=0, short int=0, short int=0);
	// *STRUCTORS
		Blotchmaker(Map &incoming_map);
		~Blotchmaker() {/*do nothing*/}

private:
	// variables
	Map *map; // a pointer to the map we are working on
	short int map_cols;
	short int map_rows;
	struct pos prev_tile;
	struct pos this_tile;
	struct pos next_tile;
	int tile_count; // tiles per blotch, running total
	short int blotch_count; // blotches running total
	short int turn_types[4]; // possibly add 45deg turns in the future
	short int num_turn_types; // simple number to pass to functions
	short int num_blotch_modes; // simple number to pass to functions
	short int num_blotch_types; // simple number to pass to functions
	short int num_slots; // number of total blotch_type slots available for game use
	enum blotch_modes blotch_mode; // current blotch mode in operation
	struct blotch_type_perams *blotch_type; // ptr - current blotch type in operation
	struct map_user_vars_package vars_template; // the template containing the user variables. determine which one to use later

	short int hbsize; // radius size of home bases. setting of 3 will produce a seed with 3 on each side, aka 7x7
	short int hbplacement; // placement type of home bases, corners=0, sides, [random]
	obstacle hbtypes[8]; // a list of hb types to put on the map.
	pos hb_locs[4]; // x,y locations of home bases
	short int num_players; //based on the number of home bases loaded

	obstacle map_init; // default initializer.

	// Functions: Starting from the top:
	// Blotching Functions
	void MakeBlotch ();
		bool DoNextTile ();
			int AdjustForEdgeAttraction (char axis, short int coord);
			int ExtractVector (struct pos prev_tile, struct pos next_tile);
			int GetTurnAngle ();
			bool GetNextTileCoords (short int vector, struct pos &this_tile, struct pos &next_tile);
			bool ApplyTile (short int x, short int y);
			bool ApplyBrush (struct pos tile);
	// Other Functions
	void CreateHBs();
	bool VerifyMap(); // checks to make sure each HB can get to each other HB
	};







