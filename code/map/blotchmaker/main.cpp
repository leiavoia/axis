#include <time.h>
#include "AxisMapMenu.h"

int main(int argc, char* args[]) {
	//Seed the random numbers:
	srand(time(NULL));

	AxisMapMenu* amm = new AxisMapMenu();
	amm->app->Run();

	return true;
}