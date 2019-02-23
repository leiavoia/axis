#ifndef __AXIS_MAP_MENU__
#define __AXIS_MAP_MENU__

/********************************* Includes **********************************/

#include <stdio.h>
#include <time.h>

#include "paragui.h"
#include "pgapplication.h"
#include "pgbutton.h"
#include "pgdropdown.h"
#include "pgeventobject.h"
#include "pglabel.h"
#include "pglineedit.h"
#include "pgradiobutton.h"

#include "AxisMiniMapWidget.h"

#include "map.h"
#include "blotchmaker.h"

/******************************* End Includes ********************************/



/**************************** Application Settings ***************************/
//Since this is the only GUI class so far, we need to define the app-wide 
// settings here. Later, they will be defined elsewhere

/** The width of the screen in pixels */
#define RES_X 1024
/** The height of the screen in pixels */
#define RES_Y 768
/** Desired bits per pixel */
#define BPP 32
/** Double buffering? */
#define DBL_BUFFER 1

/************************** End Application Settings *************************/



/************************* Map Menu Default Settings *************************/

/** Default Map Width */
#define DEFAULT_MAP_WIDTH 100
/** Default Map Height */
#define DEFAULT_MAP_HEIGHT 100
/** Default tiles per blotch */
#define DEFAULT_TILES_PER_BLOTCH 100
/** Default Deterioration */
#define DEFAULT_DETERIORATION 10
/** Default Edge Gravity */
#define DEFAULT_EDGE_GRAVITY 0

/*********************** End Map Menu Default Settings ***********************/



/***************************** Class Definitions *****************************/

/**
 * This class represents the menu that allows the user to select the
 * appropriate settings for map generation. It is, in other words, a way to
 * manipulate blotchmaker in a graphical fashion.
 *
 * @author <A HREF="mailto:gtg308i@mail.gatech.edu">Vladimir Urazov</A>
 * @version Version 1.0, Apr. 19, 2003
 */
class AxisMapMenu : public PG_EventObject {
public:

	//////////////////////////////// *structors ///////////////////////////////
	AxisMapMenu();
	~AxisMapMenu();
	////////////////////////////// End *structors /////////////////////////////

	////////////////////////// Accessor and Modifiers /////////////////////////
	/** Returns the pointer to the map generated in this menu. It should be
	  * retreieved by the application when the user chooses to go on to the 
	  * next menu or into the game.
	  * 
	  * @return the map pointer.
	  */
	Map* getMap() { return theMap; }
	//////////////////////// End Accessor and Modifiers ///////////////////////

	/////////////////////////// Other public methods //////////////////////////
	PARAGUI_CALLBACK(handle_button_press);
	int generateMap();
	///////////////////////// End Other public methods ////////////////////////

	/////////////////////////////// Application ///////////////////////////////
	/** This is the application pointer. Again, when we add more to the 
	  * application, it will need to be removed from here. */
	PG_Application* app;
	///////////////////////////// End Application /////////////////////////////

private:
	///////////////////////////// Non-GUI members /////////////////////////////
	/** The map to be filled up randomly */
	Map* theMap;
	/** The blotchmaker utility to randomize the map */
	Blotchmaker* blotcher;

	/** The width of the map as requested by the user */
	int mapWidth;
	/** The height of the map as requested by the user */
	int mapHeight;
	/** The number of tiles per blotch, as requested by the user */
	int tilesPerBlotch;
	/** Deterioration rate, as requested by the user */
	int deterioration;
	/** Edge gravity, as requested by the user */
	int edgeGravity;
	/////////////////////////// End Non-GUI members ///////////////////////////

	////////////////////////////// Custom Widgets /////////////////////////////
	/** Will display the preview of the generated map. */
	AxisMiniMapWidget* miniMap;
	//////////////////////////// End Custom Widgets ///////////////////////////

	////////////////////////////////// Labels /////////////////////////////////
	/** The title of the View. (Should say something like "Create Map" in the
	  * top-middle of the window.) */
	PG_Label* lblTitle;
	/** The label for the Load section of the view. Should be the head of the
	  * map and settings loaders. */
	PG_Label* lblLoad;
	/** The label before the Map Load textbox. */
	PG_Label* lblLoadMap;
	/** The label before the Settings Load textbox. */
	PG_Label* lblLoadSettings;
	/** The label of the map size drop-down. */
	PG_Label* lblMapSize;
	/** The label of the Tiles Per blotch pane. */
	PG_Label* lblTilesPerBlotch;
	/** The label of the deterioration pane. */
	PG_Label* lblDeterioration;
	/** The label of the edge gravity pane. */
	PG_Label* lblEdgeGravity;
	//////////////////////////////// End Labels ///////////////////////////////

	///////////////////////////////// Buttons /////////////////////////////////
	/** Go back to the previous menu */
	PG_Button* btnBack;
	/** Save the current settings and go to the next menu */
	PG_Button* btnForward;
	/** Save the current map into a file */
	PG_Button* btnSaveMap;
	/** Save the current settings into a file */
	PG_Button* btnSaveSettings;
	/** Load Map from a file */
	PG_Button* btnLoadMap;
	/** Load settings from a file */
	PG_Button* btnLoadSettings;
	/** Generate a random map using the current settings */
	PG_Button* btnGenerateMap;
	/** Toggle between regualr and advanced settings */
	PG_Button* btnAdvancedView;
	/////////////////////////////// End Buttons ///////////////////////////////

	////////////////////////////// Radio Buttons //////////////////////////////
	/** Set the number of tiles per blotch to few */
	PG_RadioButton* radTilesPerBlotchFew;
	/** Set the number of tiles per blotch to regular */
	PG_RadioButton* radTilesPerBlotchNormal;
	/** Set the number of tiles per blotch to a lot */
	PG_RadioButton* radTilesPerBlotchMany;

	/** Set the Deterioration to Weak */
	PG_RadioButton* radDeteriorationWeak;
	/** Set the Deterioration to Regular */
	PG_RadioButton* radDeteriorationRegular;
	/** Set the Deterioration to Strong */
	PG_RadioButton* radDeteriorationStrong;

	/** Set the Edge Gravity to Repell */
	PG_RadioButton* radEdgeGravityRepell;
	/** Set the Edge Gravity to None */
	PG_RadioButton* radEdgeGravityNone;
	/** Set the Edge Gravity to Attract */
	PG_RadioButton* radEdgeGravityAttract;
	//////////////////////////// End Radio Buttons ////////////////////////////

	///////////////////////////// Drop-Down Menus /////////////////////////////
	/** The drop-down menu that allows the user to change the size of the
	  * map. */
	PG_DropDown* ddMapSize;
	/////////////////////////// End Drop-Down Menus ///////////////////////////

	//////////////////////////////// Text Boxes ///////////////////////////////
	/** The text box that will accept the name of the file to write to, or load
	  * from, a map. */
	PG_LineEdit* txtMapFile;
	/** The text box that will accept the name of the file to write to, or load
	  * from, settings. */
	PG_LineEdit* txtSettingsFile;
	////////////////////////////// End Text Boxes /////////////////////////////

	//////////////////////// Application-Wide Functions ///////////////////////
	bool initApplication();
	////////////////////// End Application-Wide Functions /////////////////////

	//////////////////////// Initialization Functions /////////////////////////
	bool initWidgets();
	bool registerCallbacks();
	////////////////////// End Initialization Functions ///////////////////////

	///////// Callback processor functions (should be called from one /////////
	/////////////////// of the PARAGUI_CALLBACK functions). ///////////////////
	bool handleBack();
	bool handleForward();
	bool handleSaveMap();
	bool handleSaveSettings();
	bool handleLoadMap();
	bool handleLoadSettings();
	bool handleGenerateMap();
	bool handleAdvancedView();
	///////////////////////// End Callback processors /////////////////////////

	////////////////////////////// Other methods //////////////////////////////
	bool processSettings();
	void initMap();

	void dumpState(FILE *out);
	//////////////////////////// End Other methods ////////////////////////////
};

/*************************** End Class Definitions ***************************/










class performanceTest {
public:
	performanceTest(int max, FILE* out) : start(clock()){ test(max, out); } //start counting time

	void test(int max, FILE* out) {
		for(int i=0; i<=max; i++) {
			/*
			printf("%d is prime: ", i);
			useDivision(i)?printf("true"):printf("false");
			printf("\n");
			*/
			useDivision(i);
		}
		clock_t total = clock() - start; //get elapsed time
		fprintf(out, "Run time: %d ticks\n", total);
		fprintf(out, "          %f seconds\n\n", total/1000.0);
	}

	bool useDivision(int primeNum) {
		for(int i=2; i<primeNum-1; i++) {
			if((primeNum % i) == 0) {
				return false;
			}
		}
		return true;
	}

private:
	clock_t start;
};
















#endif  //__AXIS_MAP_MENU__