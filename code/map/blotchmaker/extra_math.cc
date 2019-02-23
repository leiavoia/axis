# include <math.h>

// extra_math.cc for blotchmaker





//////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConvertToGoalMarkers (float list[], short int num_list_items) {
// converts lists of random-chance parts into a list of % goal markers for if statement decisions
// takes:
//	a list ref
//	number of items in list
// returns: nothing (just transforms list)

short int counter = 0;
float max_chances = 0;
float running_total = 0;

//get total
for (counter = 0; counter < num_list_items; counter++) {
	max_chances += list[counter];
	}
//convert to percentages
for (counter = 0; counter < num_list_items; counter++) {
	list[counter] = ( list[counter] / max_chances ) * 100;
	}
//convert to goal markers
for (counter = 0; counter < num_list_items; counter++) {
	float was = list[counter];
	list[counter] += running_total;
	running_total += was;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////






