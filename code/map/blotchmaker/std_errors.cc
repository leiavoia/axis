/*******************************************

ERRORS.h
==========================
STANDARD ERROR CLASSES
==========================
version 0.0.1

*******************************************/

//#include <iostream>
//#include <stdio.h>
#include <string.h>



using namespace std;





//=====================================================================

// GENERIC ERROR
// reports a simple string message - 75 Char limit
class err_generic {
public:
	char msg[75]; // the error report string message
	//constructor - initializes with report string
	err_generic(char message[]) {
		if (strlen(message) < 75) { strcpy(msg, message); }
		else {strcpy(msg, "INTERNAL ERROR: Error String Message Too Long"); }
		}
};


//=====================================================================

// OUT OF BOUNDS - INTEGER
// reports min/max required bounds and number user attempted.
class err_bounds_int {
public:
	int min; // minimum boundary
	int max; // maximum boundary
	int attempted; // user tried this out-of-bounds number

	//constructor - initializes with report vars
	err_bounds_int(int min_in=0, int max_in=0, int attempted_in=0) {
		min = min_in;
		max = max_in;
		attempted = attempted_in;
		}
};

//=====================================================================




/*
int main() {

err_bounds_int anerror(0,100,234);

cout << anerror.min << anerror.max << anerror.attempted <<"\n";

return 0;
}
*/