/*
====================
BlotchMaker.cc
====================
*/




//---------------------------------------------------------------------------------------
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "extra_math.h"
#include "blotchmaker.h"
#include "mapsearch.h"
#include "stlastar.h"

// these are the divisors in the auto-allocation equations if
// 	blotches and tiles per blotch are on auto mode
#define AUTO_TILES_MODIFIER 20
#define AUTO_BLOTCHES_MODIFIER 150
//---------------------------------------------------------------------------------------




using namespace std;



// you want fries with that? works only in this file
static const bool verbose = 0;




inline int RoundValue(const float &param) {
	if((param - floor(param)) < 0.5f)
		return int(floor(param));
	else
		return int(ceil(param));
	}


inline float AbsValue(float num){
	if (num >= 0) return(num);
	else return(  num - num - num  );
	}
inline int AbsValue(int num){
	if (num >= 0) return(num);
	else return(  num - num - num  );
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
Blotchmaker::Blotchmaker(Map &incoming_map) {
// constructor

// get a pointer to the map we are working on for local use.
map = &incoming_map;


// initialize variables:
tile_count = 0;
blotch_count = 0;
map_cols = map->GetCols();
map_rows = map->GetRows();
turn_types[0] = 0;
turn_types[1] = 270;
turn_types[2] = 90;
turn_types[3] = 180;
num_turn_types = 4;
num_blotch_modes = 4;
num_blotch_types = 5;
num_slots = 5;


// ---------------
// Set default map generation variables for each slot:
// ---------------


// Group vars:
SetMaxBlotches(2);
SetTypeChances(0, 1, 0, 0, 0, 0, 0, 0, 0, 0);

//foreach slot
for (short int x = 0; x < num_blotch_types; x++) {
	SetModeChances(x, 0, 1, 0, 0);
	SetBlotchType(x, x);
	SetTilesPerBlotch(x, 1);
	SetTurnChances(x, 2, 1, 1, 0);
	SetBlankShotChance(x, 0);
	SetEdgeGrav(x, 0);
	SetEdgeDet(x, 40);
	SetBrush(x, LC);
	SetStep(x, 1);
	}

// other vars
hbsize = 2; // makes a 5x5 map
hbplacement = 0;
num_players = 0;
// need at least two to play:
SetAxisPalette(6,7);

map_init = EMPTY;


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Blotchmaker::StartBlotching() {
// RETURNS:
//	1 if a good map is produced
//	0 if no good map could produced after 10 tries with the current settings.

// "chances" lists -> send to converter functions (for each blotch_type in the vars template)
ConvertToGoalMarkers( vars_template.type_chances, num_blotch_types );
for (short int x = 0; x < num_blotch_types; x++) {
	ConvertToGoalMarkers( vars_template.type[x].mode_chances, num_blotch_modes );
	ConvertToGoalMarkers( vars_template.type[x].turn_chances, num_turn_types );
	}


short int attempts = 0; // number of times we've tried to make a good map with these settings.
bool good = 0; // whether or not the map was a "good", verifiable map.

do {

	// initialize the map
	map->InitializeMap(map_init);

	// start making blotches
	while(blotch_count < vars_template.max_blotches) {

		// roll for blotch TYPE (slot)
		float blotch_type_roll = rand() % 100;
		for (short int counter = 0; counter < num_blotch_types; counter++) {
			// set the blotch_type pointer to the right type stats in the template
			if (  blotch_type_roll <= vars_template.type_chances[counter]  ) {
				blotch_type = &vars_template.type[counter]; //set a pointer for easy future use
				break;
				}
			}

		// roll for blotch MODE
		float blotch_mode_roll = rand() % 100;
		for (short int counter = 0; counter < num_blotch_modes; counter++) {
			if (  blotch_mode_roll <= blotch_type->mode_chances[counter]  ) {
				blotch_mode = blotch_modes(counter); //typecasted into blotch_mode type
				break;
				}
			}

		MakeBlotch();
		blotch_count++;
		}

	// after making all blotches, set HBs
	CreateHBs();

	// reset blotchcounter in case attempting again
	blotch_count = 0;

	attempts++;
	good = VerifyMap();
	}

while( (good == 0) && (attempts < 3) ); // timeout after so many tries

// if we tried the best we could and still couldn't make a decent map:
// NOTE: in the future if path-cutting is implemented, this is where you would plug it in

return good;

/*
if(good == 0) {
	cout << "Bad Map - Try Again - Attempts: " << attempts << "\n" ;
	return 0;
	}
else {
	cout << "Yeah! Good Map!- Attempts: " << attempts << "\n" ;
	return 1; // produced a good map
	}
*/

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Blotchmaker::MakeBlotch () {
// Starts making a blotch.

// pick a place to start, if needed
if (tile_count == 0) {
	struct pos first_tile;
	first_tile.x = int(  rand() % map_cols  );
	first_tile.y = int(  rand() % map_rows  );
	first_tile.x = AdjustForEdgeAttraction('x', int(rand()%map_cols));
	first_tile.y = AdjustForEdgeAttraction('y', int(rand()%map_rows));
	ApplyBrush(first_tile);
	if (verbose) {cout << "STARTING AT: " << first_tile.x << ',' << first_tile.y << '\n';}
	// update stats
	prev_tile.x = first_tile.x;
	prev_tile.y = first_tile.y;
	tile_count++;
	}


if (verbose) {cout << "Selected: " <<  blotch_mode << " - [0=Full, 1=Chaotic, 2=X-OR]\n";}




// start placing tiles
while(tile_count < blotch_type->tiles_per_blotch) {
	DoNextTile();
	}

tile_count = 0;


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Blotchmaker::DoNextTile () {
// Places a tile in a series inside of a blotch
// Returns: 0 if good, 1 if terminated.

// do a random second tile, if needed, to establish a vector
if (tile_count == 1) {
	bool gotcha = 0;
	while (gotcha == 0) {
		short int roll = int(rand()%4);
		switch (roll) {
			case 0:
				this_tile.x = prev_tile.x + blotch_type->step;
				this_tile.y = prev_tile.y;
				break;
			case 1:
				this_tile.x = prev_tile.x - blotch_type->step;
				this_tile.y = prev_tile.y;
				break;
			case 2:
				this_tile.x = prev_tile.x;
				this_tile.y = prev_tile.y + blotch_type->step;
				break;
			case 3:
				this_tile.x = prev_tile.x;
				this_tile.y = prev_tile.y - blotch_type->step;
				break;
			}
		// if it checks out, go with it.
		if (  map->IsValid(this_tile.x, this_tile.y)  ) {
			ApplyBrush(this_tile);
			// update stats
			tile_count++;
			gotcha = 1;
			}
		}
		return 0;
	} // end random second tile

// get a vector
short int vector = ExtractVector(prev_tile, next_tile);
if (verbose) {cout << "From: " << prev_tile.x << ',' << prev_tile.y << "To: " << this_tile.x << ',' << this_tile.y << '\n';}
if (verbose) {cout << "Vector: " << vector << '\n';}

// do a roll to get a turn angle
short int turn_angle = GetTurnAngle();

// find the absolute turn angle to calculate the next tiles position
short int final_vector = (vector + turn_angle) % 360;
if (verbose) {cout << "Final Vector: " << final_vector << '\n';}

// calculate the next tiles position
bool failed = GetNextTileCoords(final_vector, this_tile, next_tile);

// termination signal if bad
if (failed) { return(1); }

if (verbose) {cout << "From: " << this_tile.x << ',' << this_tile.y << "To: " << next_tile.x << ',' << next_tile.y << "\n\n";}

// apply the blotch mode and tile to map
bool do_not_update = ApplyBrush(next_tile);

// update stats
if ( do_not_update == false ) {tile_count++;}
prev_tile.x = this_tile.x;
prev_tile.y = this_tile.y;
this_tile.x = next_tile.x;
this_tile.y = next_tile.y;

return 0;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Blotchmaker::ApplyBrush (struct pos tile) {
// Applies the brush pattern to the map
// Takes: tile to place
// returns 0

// for clarity:
short int x = tile.x;
short int y = tile.y;

// shoot a blank?
if ( rand()%100 < blotch_type->blank_shot_chance ) {return 1;}

else {
	switch(blotch_type->brush) {
		case RE: // regular 1-tile
			ApplyTile(x, y);
			break;

		// CIRCLES
		case LC: // large circle
			for (short int a = -15; a <= 15; a++) {
				for (short int b = -15; b <= 15; b++) {
					if ( sqrt( float(a*a) + float(b*b) ) < 15) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case MC: // medium circle
			for (short int a = -8; a <= 8; a++) {
				for (short int b = -8; b <= 8; b++) {
					if (sqrt( float(a*a) + float(b*b) ) < 8) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case SC: // small circle
			for (short int a = -3; a <= 3; a++) {
				for (short int b = -3; b <= 3; b++) {
					if (sqrt( float(a*a) + float(b*b) ) < 3) { ApplyTile(x+a, y+b); }
					}
				}
			break;

		// RINGS
		case LR: // large ring
			for (short int a = -15; a <= 15; a++) {
				for (short int b = -15; b <= 15; b++) {
					float h = sqrt( float(a*a) + float(b*b) );
					if (h < 15 && h >= 13) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case MR: // medium ring
			for (short int a = -8; a <= 8; a++) {
				for (short int b = -8; b <= 8; b++) {
					float h = sqrt( float(a*a) + float(b*b) );
					if (h < 8 && h >= 6) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case SR: // small ring
			for (short int a = -3; a <= 3; a++) {
				for (short int b = -3; b <= 3; b++) {
					float h = sqrt( float(a*a) + float(b*b) );
					if (h < 3 && h >= 2) { ApplyTile(x+a, y+b); }
					}
				}
			break;


		// X-MARKS
		case SX: // small X
			ApplyTile(x, y);
			for (short int a = -1; a <= 1; a++) {
				for (short int b = -1; b <= 1; b++) {
					if ( AbsValue(a) == AbsValue(b) ) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case MX: // medium X
			ApplyTile(x, y);
			for (short int a = -3; a <= 3; a++) {
				for (short int b = -3; b <= 3; b++) {
					if ( AbsValue(a) == AbsValue(b) ) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case LX: // medium X
			ApplyTile(x, y);
			for (short int a = -8; a <= 8; a++) {
				for (short int b = -8; b <= 8; b++) {
					if ( AbsValue(a) == AbsValue(b) ) { ApplyTile(x+a, y+b); }
					}
				}
			break;


		// PLUS MARKS (+)
		case SP: // small +
			ApplyTile(x, y);
			for (short int a = -1; a <= 1; a++) {
				for (short int b = -1; b <= 1; b++) {
					if (  (a == 0 && b != 0)  ||  (a != 0 && b == 0)  ) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case MP: // medium +
			ApplyTile(x, y);
			for (short int a = -3; a <= 3; a++) {
				for (short int b = -3; b <= 3; b++) {
					if (  (a == 0 && b != 0)  ||  (a != 0 && b == 0)  ) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case LP: // large +
			ApplyTile(x, y);
			for (short int a = -8; a <= 8; a++) {
				for (short int b = -8; b <= 8; b++) {
					if (  (a == 0 && b != 0)  ||  (a != 0 && b == 0)  ) { ApplyTile(x+a, y+b); }
					}
				}
			break;


		// SQUARES
		case SS: // small square
			for (short int a = -1; a <= 1; a++) {
				for (short int b = -1; b <= 1; b++) {
					ApplyTile(x+a, y+b);
					}
				}
			break;
		case MS: // medium square
			for (short int a = -3; a <= 3; a++) {
				for (short int b = -3; b <= 3; b++) {
					ApplyTile(x+a, y+b);
					}
				}
			break;
		case LS: // large square
			for (short int a = -8; a <= 8; a++) {
				for (short int b = -8; b <= 8; b++) {
					ApplyTile(x+a, y+b);
					}
				}
			break;


		// BOXES
		case SB: // small box
			for (short int a = -1; a <= 1; a++) {
				for (short int b = -1; b <= 1; b++) {
					if (  (AbsValue(a) == 1) && (AbsValue(b) != 1)  ) { ApplyTile(x+a, y+b); }
					else if (  (AbsValue(a) == 1) || (AbsValue(b) == 1)  ) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case MB: // medium box
			for (short int a = -3; a <= 3; a++) {
				for (short int b = -3; b <= 3; b++) {
					if (  (AbsValue(a) == 3) && (AbsValue(b) != 3)  ) { ApplyTile(x+a, y+b); }
					else if (  (AbsValue(a) == 3) || (AbsValue(b) == 3)  ) { ApplyTile(x+a, y+b); }
					}
				}
			break;
		case LB: // large box
			for (short int a = -8; a <= 8; a++) {
				for (short int b = -8; b <= 8; b++) {
					if (  (AbsValue(a) == 8) && (AbsValue(b) != 8)  ) { ApplyTile(x+a, y+b); }
					else if (  (AbsValue(a) == 8) || (AbsValue(b) == 8)  ) { ApplyTile(x+a, y+b); }
					}
				}
			break;



		default: // defaults to [RE]gular
			ApplyTile(x, y);
			break;
		}
 	}


return(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Blotchmaker::ApplyTile (short int x, short int y) {
// Applies the individual tile to the map in a variety of ways
// Takes: tile to place
// returns zero always.

// last validity check. We need this again because previous checks only checked for the core tile in the brush
//	it did not check for the exterior brushed tiles in the brushe's stroke path.
if ( !(map->IsValid(x,y)) ) { return 0;}

else if ( rand()%100 < blotch_type->edge_det ) {return 0;}

else {
	switch(blotch_mode) {
		case full:
			if ( map->GetObst(x, y) ) { return 1;}
			else { map->SetObst(x, y, blotch_type->blotch_type); }
			break;
		case chaotic:
			map->SetObst(x, y, blotch_type->blotch_type);
   			break;
		case x_or:
			if ( map->GetObst(x, y) == blotch_type->blotch_type ) { map->SetObst(x, y, 0);}
			else {map->SetObst(x, y, blotch_type->blotch_type);}
   			break;
		case clear:
			map->SetObst(x, y, 0);
   			break;
		default: // defaults to full
			if ( map->GetObst(x, y) ) { return 1;}
			else { map->SetObst(x, y, blotch_type->blotch_type); }
		}
 	}

return(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////







/////////////////////////////////////////////////////////////////////////////////////////////////////////
int Blotchmaker::ExtractVector (struct pos prev_tile, struct pos next_tile) {
// Gets a vector based on the current tile and the last tile placed.
// Takes: the previous tile position, the current tile position
//             0 (F)
//                |
// 270 (L) ---+--- 90 (R)
//                |
//           180 (B)
short int delta_x = prev_tile.x - this_tile.x;
short int delta_y = prev_tile.y - this_tile.y;
if (delta_x == -1) { return 90;}
else if (delta_x == 1) { return 270;}
else if (delta_y == -1) { return 0;}
else if (delta_y == 1) { return 180;}
return(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
int Blotchmaker::GetTurnAngle () {
// Gets a randomly generated turn, based on turn persuasions.
//
float roll = rand()%100;
for (short int x = 0; x < num_turn_types; x++) {
	if (  roll <= blotch_type->turn_chances[x]  ) {
		if (verbose) {cout << "Turn angle: " << turn_types[x] << '\n';}
		return (turn_types[x]);
		}
	}
	return (0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Blotchmaker::GetNextTileCoords (short int vector, struct pos &this_tile, struct pos &next_tile) {
// gets the coordinates of a tile using a final vector (abs turn angle) and the previous tile
// modifies "next_tile" instead of returning it.
// takes:
//	the vector (turn angle),
//	ref to the current tile
//	ref to the next_tile
// returns: 0 if good, 1 if failed and terminate blotch
switch (vector) {
	case 0:
		next_tile.x = this_tile.x;
		next_tile.y = this_tile.y + blotch_type->step;
		break;
	case 90:
		next_tile.x = this_tile.x + blotch_type->step;
		next_tile.y = this_tile.y;
		break;
	case 180:
		next_tile.x = this_tile.x;
		next_tile.y = this_tile.y - blotch_type->step;
		break;
	case 270:
		next_tile.x = this_tile.x - blotch_type->step;
		next_tile.y = this_tile.y;
		break;
	}
// check for validity
if (  map->IsValid(next_tile.x, next_tile.y)  ) {return 0;}
// otherwise TERMINATE BLOTCH
else {
	tile_count = blotch_type->tiles_per_blotch; // max out counter
 	return(1); // termination signal
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////
int Blotchmaker::AdjustForEdgeAttraction (char axis, short int coord) {
// Adjusts the first tile placed by the gravity of the edge in that particular blotch type
// Takes:
//	the axis: (char) X or Y
//	the current position on that axis (say, "32"...)
// Returns: the coord of where you ought to place the tile.

//find the center
float center;
if (axis == 'X' || axis == 'x') {center = (map_cols-1) / 2.0;} else {center = (map_rows-1) / 2.0;}
// minus one because this is for the array index which starts at zero!!!

//find the distance from the edge
float edge_dist = center - AbsValue(coord - center);
if (edge_dist == 0) {edge_dist = 1;} // quick fix for possible divide by zero

//apply the formula
float attr = (blotch_type->edge_grav * center * (1 / edge_dist ) ) - blotch_type->edge_grav;

//get the new tile on the axis to move to
short int ans;
if (coord < center) {
	ans = coord - RoundValue(attr);
	if (ans > center) {ans = int(center);} // adjusted for neg. gravity
	}
else {
	ans = coord + RoundValue(attr);
	if (ans < center) {ans = int(center);} // adjusted for neg. gravity
	}

// check for out-of-bounds
if (ans < 0) {ans = 0;}
if (ans > (center*2)) {ans = int(center*2);}

return(ans);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEMPLATE VARS ADJUSTING FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Blotchmaker::SetMapInit(short int x){
map_init = obstacle(x);
}

void Blotchmaker::SetBlotchType(short int type, short int x) {
vars_template.type[type].blotch_type = x;
}

void Blotchmaker::SetTilesPerBlotch(short int type, short int x) {
vars_template.type[type].tiles_per_blotch = x;
}


void Blotchmaker::SetModeChances(short int type, short int full, short int chaotic, short int x_or, short int clear) {
vars_template.type[type].mode_chances[0] = full;
vars_template.type[type].mode_chances[1] = chaotic;
vars_template.type[type].mode_chances[2] = x_or;
vars_template.type[type].mode_chances[3] = clear;
}


void Blotchmaker::SetTurnChances(short int type, short int forward, short int left, short int right, short int back) {
vars_template.type[type].turn_chances[0] = forward;
vars_template.type[type].turn_chances[1] = left;
vars_template.type[type].turn_chances[2] = right;
vars_template.type[type].turn_chances[3] = back;
}


void Blotchmaker::SetBlankShotChance(short int type, short int x) {
vars_template.type[type].blank_shot_chance = x;
}


void Blotchmaker::SetEdgeGrav(short int type, short int x) {
vars_template.type[type].edge_grav = x;
}


void Blotchmaker::SetEdgeDet(short int type, short int x) {
vars_template.type[type].edge_det = x;
}


void Blotchmaker::SetStep(short int type, short int x) {
vars_template.type[type].step = x;
}


void Blotchmaker::SetBrush(short int type, enum brush brush) {
vars_template.type[type].brush = brush;
}


void Blotchmaker::SetMaxBlotches(short int x) {
vars_template.max_blotches = x;
}


void Blotchmaker::SetTypeChances(
	short int a,
	short int b,
	short int c,
	short int d,
	short int e,
	short int f,
	short int g,
	short int h,
	short int i,
	short int j ) {
vars_template.type_chances[0] = a;
vars_template.type_chances[1] = b;
vars_template.type_chances[2] = c;
vars_template.type_chances[3] = d;
vars_template.type_chances[4] = e;
//vars_template.type_chances[5] = f;
//vars_template.type_chances[6] = g;
//vars_template.type_chances[7] = h;
//vars_template.type_chances[8] = i;
//vars_template.type_chances[9] = j;
}








//////////////////////////////////////////////////////////////////////////////////////////////////////////
// OTHER FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Blotchmaker::CreateHBs(){

// in the future, we'll think of a more clever way to randomly
// place these guys, but for now, just put them in pre-arranged configurations

// REMEMBER: we are zero indexed so minus one!

// if two players:
if (num_players == 2) {
	// lower left
	hb_locs[0].x = 0 + hbsize;
	hb_locs[0].y = 0 + hbsize;
	// upper right
	hb_locs[1].x = map_cols - hbsize -1;
	hb_locs[1].y = map_rows - hbsize -1;
	}
// if 3 players
if (num_players == 3) {
 	// upper left
	hb_locs[0].x = 0 + hbsize;
	hb_locs[0].y = map_rows - hbsize -1;
	// upper right
	hb_locs[1].x = map_cols - hbsize -1;
	hb_locs[1].y = map_rows - hbsize -1;
	// bottom side
	hb_locs[2].x = int(map_cols / 2)-1;
	hb_locs[2].y = 0 + hbsize ;
	}
// if 4 players
if (num_players == 4) {
	// lower left
	hb_locs[0].x = 0 + hbsize;
	hb_locs[0].y = 0 + hbsize;
	// upper right
	hb_locs[1].x = map_cols - hbsize -1;
	hb_locs[1].y = map_rows - hbsize -1;
	// lower right
	hb_locs[2].x = map_cols - hbsize -1;
	hb_locs[2].y = 0 + hbsize;
	// upper left
	hb_locs[3].x = 0 + hbsize;
	hb_locs[3].y = map_rows - hbsize -1;
	}

// now fill them in
for (int x = 0; x < num_players; x++) { // foreach HB
	for (short int a = (hb_locs[x].x - hbsize); a <= (hb_locs[x].x + hbsize); a++) {
		for (short int b = (hb_locs[x].y - hbsize); b <= (hb_locs[x].y + hbsize); b++) {
			map->SetObst(a,b, hbtypes[x]);
			}
		}
	}
}





bool Blotchmaker::VerifyMap(){
// Checks to make sure HBs are accessable by eachother. no blocking obstacles.
// returns 1 if success, zero if failed

//pointer to the search
AStarSearch<MapSearchNode>* astarsearch;

//cout << "Begin Map Verification:\n";

int timeout = ((map_cols * map_rows) / 8);

// foreach corner position
for (short int here = 0; here < num_players; here++) {
	for (short int there = here+1; there < num_players; there++) {

		if (here == there) {continue;} //skip if finding path to self

		//cout << "	Now Checking: (" << hb_locs[here].x << ',' << hb_locs[here].y << ") to (" << hb_locs[there].x << ',' << hb_locs[there].y << ") \n";

		// Create an instance of the search class...
		astarsearch = new AStarSearch<MapSearchNode>;

		// set the pointer to the map
		MapSearchNode::SetMapPointer(*map);


		// Create and set start/goal states
		MapSearchNode nodeStart(hb_locs[here].x, hb_locs[here].y);
		MapSearchNode nodeEnd(hb_locs[there].x, hb_locs[there].y);
		astarsearch->SetStartAndGoalStates( nodeStart, nodeEnd );

		 int SearchState;
		 int SearchSteps = 0;

		do {
			SearchState = astarsearch->SearchStep();
			SearchSteps++;
			//cout << "*";
			}
		while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING &&
			//SearchSteps < ((map_cols + map_rows) * 2)  );
			SearchSteps < timeout );

		// if solution found, display:
		if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED ) {
			// Once you're done with the solution you can free the nodes up
			astarsearch->FreeSolutionNodes();
			//cout << "		Succeeded. " << SearchSteps << " steps searched\n";
			}
		// otherwise, kill and return 0
		else { //if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED ) {
			//cout << "		Failed. " << SearchSteps << " steps searched on this route\n";
			delete astarsearch;
			return 0;
			}

		delete astarsearch;
		}
	}

//cout << "End Map Verification:\n";

return 1;
}





// set the square size of home bases in tiles
void Blotchmaker::SetHBSize(short int x) {
	hbsize = x;
	}


// set the placement of home bases. 0=Corners, 1=Sides, [future: 2=random?]
void Blotchmaker::SetHBPlacement(short int x){
	hbplacement = x;
	}


// loads the axis colors to place on the map
void Blotchmaker::SetAxisPalette(short int a, short int b, short int c, short int d, short int e, short int f, short int g, short int h) {
	num_players = 0; // in case of do-overs and such, don't just keep adding :-)

	hbtypes[0] = obstacle(a);
	hbtypes[1] = obstacle(b);
	hbtypes[2] = obstacle(c);
	hbtypes[3] = obstacle(d);
	hbtypes[4] = obstacle(e);
	hbtypes[5] = obstacle(f);
	hbtypes[6] = obstacle(g);
	hbtypes[7] = obstacle(h);

	// count them up
	if (a != 0) { num_players++; }
	if (b != 0) { num_players++; }
	if (c != 0) { num_players++; }
	if (d != 0) { num_players++; }
	if (e != 0) { num_players++; }
	if (f != 0) { num_players++; }
	if (g != 0) { num_players++; }
	if (h != 0) { num_players++; }
	}







