/**
* \file AxisMapMenu.cpp
*
* This file defines the behaviour of the AxisMapMenu class, which is
* basically a graphical representation of the menu that allows the
* player to manipulate BlotchMaker and thus easily create unique
* random maps. It contains all the necessary functionality to draw
* the different widgets, as well as generate the map. Note, however,
* that the widgets' positions are currently hard-coded in, and that
* will need to be changed sooner or later by abstracting this data
* into the appropriate XML file.
*
* Also, the pointer to the map that is generated or loaded by this
* menu can be accessed via the getMap() method, which should be used
* to fetch the map when the user is done with this screen and is
* ready to go on to the next menu, or start the game.
*
*  @author <A HREF="mailto:gtg308i@mail.gatech.edu">Vladimir Urazov</A>
*  @version Version 1.0, April 20, 2003
*/

#include "AxisMapMenu.h"

#include "pgwindow.h"
#include "pgrichedit.h"
#include "pglog.h"

//===== Constructor =========================================================//
/** Default Constructor */
AxisMapMenu::AxisMapMenu():
mapWidth(DEFAULT_MAP_WIDTH),
mapHeight(DEFAULT_MAP_HEIGHT),
tilesPerBlotch(DEFAULT_TILES_PER_BLOTCH),
deterioration(DEFAULT_DETERIORATION),
edgeGravity(DEFAULT_EDGE_GRAVITY) {

	//Right now, the application resides in this only screen.
	// That will need to be changed.
	// Initialize application
	if(!initApplication()) {
		exit(1);
	}

	//Initialize widgets:
	initWidgets();

	//Register callback methods for widgets
	registerCallbacks();

	initMap();

	//PG_LogConsole::Update();
	//PG_LogConsole::Show();
}

//===== Destructor ==========================================================//
/** Default Destructor */
AxisMapMenu::~AxisMapMenu() {
	//Kill the pointers to all the widgets, as well as blotchmaker, but not
	// the map.
	delete blotcher;

	delete miniMap;

	delete lblTitle;
	delete lblLoad;
	delete lblLoadMap;
	delete lblLoadSettings;
	delete lblMapSize;
	delete lblTilesPerBlotch;
	delete lblDeterioration;
	delete lblEdgeGravity;

	delete btnBack;
	delete btnForward;
	delete btnSaveMap;
	delete btnSaveSettings;
	delete btnLoadMap;
	delete btnLoadSettings;
	delete btnGenerateMap;
	delete btnAdvancedView;

	delete radTilesPerBlotchFew;
	delete radTilesPerBlotchNormal;
	delete radTilesPerBlotchMany;
	delete radDeteriorationWeak;
	delete radDeteriorationRegular;
	delete radDeteriorationStrong;
	delete radEdgeGravityRepell;
	delete radEdgeGravityNone;
	delete radEdgeGravityAttract;

	delete ddMapSize;

	delete txtMapFile;
	delete txtSettingsFile;
}

//===== bool AxisMapMenu::initApplication() =================================//
/**
* Initializes the application-wide settings. Note that this method should be
* moved out of this class, when we get to more screens, and other application
* stuff.
*
* @return true if the application has been initialized successfully, false on
*         failure.
*/
bool AxisMapMenu::initApplication() {
	// every ParaGUI application need an application-object
	app = new PG_Application();
	app->SetEmergencyQuit(true);

	// every application needs a theme (the look & feel of the widgets)
	if(!app->LoadTheme("default")) {
		fprintf(stderr, "Could not load theme \"axis\".\n");
		return false;
	}

	if(!app->InitScreen(RES_X, RES_Y, BPP, SDL_SWSURFACE)) {
		fprintf(stderr, "Could not initialize screen:\n"
			"\tResolution: %dx%d\n"
			"\tBits per pixel: %d\n"
			"\tFlags: %d\n",
			RES_X, RES_Y, BPP, SDL_SWSURFACE);
		return false;
	}

	return true;
}

//===== bool AxisMapMenu::initWidgets() =====================================//
/**
* Initializes the widgets and puts them on the screen. Note that right now the
* positions of the widgets are hard-coded. In the future, it would be a good
* idea to put that out into an XML file, which could then be read from this 
* method, and the widgets would be laid out accordingly.
*
* @return true upon success; false upon failure.
*/
bool AxisMapMenu::initWidgets() {
	miniMap = new AxisMiniMapWidget(NULL, PG_Rect(604, 324, 398, 425));
	miniMap->setMap(new Map(200, 200));
	miniMap->Show();

	////////////////////////////////// Labels /////////////////////////////////
	lblTitle = new PG_Label(NULL,
		PG_Rect(app->GetScreenWidth()/2 - 110, 7, 220, 40),
		"Generate Random Map");
	lblTitle->Show();

	lblLoad = new PG_Label(NULL,
		PG_Rect(30, 70, 200, 25),
		"File Manipulation:");
	lblLoad->Show();

	lblLoadMap = new PG_Label(NULL,
		PG_Rect(60, 100, 200, 25),
		"Map:");
	lblLoadMap->Show();

	lblLoadSettings = new PG_Label(NULL,
		PG_Rect(60, 160, 200, 25),
		"Settings:");
	lblLoadSettings->Show();

	lblMapSize = new PG_Label(NULL,
		PG_Rect(30, 285, 100, 25),
		"Map Size:");
	lblMapSize->Show();

	lblTilesPerBlotch = new PG_Label(NULL,
		PG_Rect(30, 350, 150, 25),
		"Obstacles (TPB):");
	lblTilesPerBlotch->Show();

	lblDeterioration = new PG_Label(NULL,
		PG_Rect(205, 350, 150, 25),
		"Erosion:");
	lblDeterioration->Show();

	lblEdgeGravity = new PG_Label(NULL,
		PG_Rect(378, 350, 150, 25),
		"Edge Gravity:");
	lblEdgeGravity->Show();
	//////////////////////////////// End Labels ///////////////////////////////

	////////////////////////////////// Buttons ////////////////////////////////
	btnLoadMap = new PG_Button(NULL, 110,
		PG_Rect(370, 125, 100, 25), "Load");
	btnLoadMap->Show();

	btnLoadSettings = new PG_Button(NULL, 111,
		PG_Rect(370, 185, 100, 25), "Load");
	btnLoadSettings->Show();

	btnSaveMap = new PG_Button(NULL, 112,
		PG_Rect(480, 125, 100, 25), "Save");
	btnSaveMap->Show();

	btnSaveSettings = new PG_Button(NULL, 113,
		PG_Rect(480, 185, 100, 25), "Save");
	btnSaveSettings->Show();

	btnBack = new PG_Button(NULL, 114,
		PG_Rect(10, 7, 150, 40),
		"<< Back");
	btnBack->Show();

	btnForward = new PG_Button(NULL, 115,
		PG_Rect(app->GetScreenWidth() - 160, 7, 150, 40),
		"Done >>");
	btnForward->Show();

	btnAdvancedView = new PG_Button(NULL, 116,
		PG_Rect(684, 125, 200, 40), "Advanced View");
	btnAdvancedView->Show();

	btnGenerateMap = new PG_Button(NULL, 117,
		PG_Rect(684, 195, 200, 40), "Generate Map");
	btnGenerateMap->Show();

	//////////////////////////////// End Buttons //////////////////////////////

	////////////////////////////// Radio Buttons //////////////////////////////
	//Tiles per blotch settings:
	radTilesPerBlotchNormal = new PG_RadioButton(NULL, 102,
		PG_Rect(40, 440, 100, 25), "Average");
	radTilesPerBlotchNormal->Show();

	radTilesPerBlotchFew = new PG_RadioButton(NULL, 101,
		PG_Rect(40, 400, 100, 25), "Small",
		radTilesPerBlotchNormal);
	radTilesPerBlotchFew->Show();

	radTilesPerBlotchMany = new PG_RadioButton(NULL, 103,
		PG_Rect(40, 480, 100, 25), "Large",
		radTilesPerBlotchNormal);
	radTilesPerBlotchMany->Show();

	//Deterioration settings:
	radDeteriorationRegular = new PG_RadioButton(NULL, 105,
		PG_Rect(215, 440, 100, 25), "Regular");
	radDeteriorationRegular->Show();

	radDeteriorationWeak = new PG_RadioButton(NULL, 104,
		PG_Rect(215, 400, 100, 25), "Weak",
		radDeteriorationRegular);
	radDeteriorationWeak->Show();

	radDeteriorationStrong = new PG_RadioButton(NULL, 106,
		PG_Rect(215, 480, 100, 25), "Strong",
		radDeteriorationRegular);
	radDeteriorationStrong->Show();

	//Edge Gravity Settings:
	radEdgeGravityNone = new PG_RadioButton(NULL, 108,
		PG_Rect(388, 440, 100, 25), "None");
	radEdgeGravityNone->Show();

	radEdgeGravityRepell = new PG_RadioButton(NULL, 109,
		PG_Rect(388, 400, 100, 25), "Repell",
		radEdgeGravityNone);
	radEdgeGravityRepell->Show();

	radEdgeGravityAttract = new PG_RadioButton(NULL, 107,
		PG_Rect(388, 480, 100, 25), "Attract",
		radEdgeGravityNone);
	radEdgeGravityAttract->Show();
	//////////////////////////// End Radio Buttons ////////////////////////////

	///////////////////////////// Drop-Down Menus /////////////////////////////
	ddMapSize = new PG_DropDown(NULL, 100,
		PG_Rect(130, 285, 200, 25));
	ddMapSize->SetEditable(false);
	ddMapSize->SetIndent(5);
	ddMapSize->SetText("Normal");
	ddMapSize->AddItem("Tiny");
	ddMapSize->AddItem("Small");
	ddMapSize->AddItem("Normal");
	ddMapSize->AddItem("Large");
	ddMapSize->AddItem("Huge");
	ddMapSize->Show();
	/////////////////////////// End Drop-Down Menus ///////////////////////////

	//////////////////////////////// Text Boxes ///////////////////////////////
	txtMapFile = new PG_LineEdit(NULL,
		PG_Rect(60, 125, 300, 25));
	txtMapFile->Show();

	txtSettingsFile = new PG_LineEdit(NULL,
		PG_Rect(60, 185, 300, 25));
	txtSettingsFile->Show();
	////////////////////////////// End Text Boxes /////////////////////////////

	return true;
}

//===== bool AxisMapMenu::registerCallbacks() ===============================//
/**
* Registers the callbacks for the widgets. As of now, only buttons need 
* callbacks, but that might need to change in the future.
*
* @return true, since right now there is nothing that can go wrong.
*/
bool AxisMapMenu::registerCallbacks() {
	// This is the callback function that will be called for every button click.
	MSG_CALLBACK_OBJ cb = (MSG_CALLBACK_OBJ)(&AxisMapMenu::handle_button_press);

	btnBack->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnForward->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnSaveMap->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnSaveSettings->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnLoadMap->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnLoadSettings->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnGenerateMap->SetEventObject(MSG_BUTTONCLICK, this, cb);
	btnAdvancedView->SetEventObject(MSG_BUTTONCLICK, this, cb);

	return true;
}

//===== PARAGUI_CALLBACK(AxisMapMenu::handle_button_press) ==================//
/** Handles button click callbacks. */
PARAGUI_CALLBACK(AxisMapMenu::handle_button_press) {
	if(widget == btnBack) {
		handleBack();
	} else if(widget == btnForward) {
		handleForward();
	} else if(widget == btnSaveMap) {
		handleSaveMap();
	} else if(widget == btnSaveSettings) {
		handleSaveSettings();
	} else if(widget == btnLoadMap) {
		handleLoadMap();
	} else if(widget == btnLoadSettings) {
		handleLoadSettings();
	} else if(widget == btnGenerateMap) {
		handleGenerateMap();
	} else if(widget == btnAdvancedView) {
		handleAdvancedView();
	} else {
		fprintf(stderr, "Attempted to handle an unknown widget event:\n");
		fprintf(stderr, "\tClass:  AxisMapMenu\n");
		fprintf(stderr, "\tMethod: PARAGUI_CALLBACK(handle_button_press)\n");
		fprintf(stderr, "\tWidget: %s (%p)\n\n", widget->GetName(), widget);
		return false;
	}
	return true;
}

//===== int thread_generateMap(void *data) ==================================//
/**
 * Calls the genearteMap() method of the AxisMapMenu passed in.
 * This method is here because one can't create threads using member functions.
 * Thus, it was necessary to create a separate function that calls a member
 * method of an object when the thread is executed.
 *
 * Usage:
 * Do not call this method directly, for there is no point in it. Rather, use
 * it to create a new SDL thread, like so:
 * SDL_CreateThread(thread_generateMap, amm);
 * amm being the pointer to an AxisMapMenu object.
 */
int thread_generateMap(void *data) {
	return ((AxisMapMenu*)data)->generateMap();
}


//===== bool AxisMapMenu::handleBack() ======================================//
/**
* 
*/
bool AxisMapMenu::handleBack() {
	return true;
}

//===== bool AxisMapMenu::handleForward() ===================================//
/**
* 
*/
bool AxisMapMenu::handleForward() {
	return true;
}

//===== bool AxisMapMenu::handleSaveMap() ===================================//
/**
* 
*/
bool AxisMapMenu::handleSaveMap() {
	return true;
}

//===== bool AxisMapMenu::handleSaveSettings() ==============================//
/**
* 
*/
bool AxisMapMenu::handleSaveSettings() {
	return true;
}

//===== bool AxisMapMenu::handleLoadMap() ===================================//
/**
* 
*/
bool AxisMapMenu::handleLoadMap() {
	return true;
}

//===== bool AxisMapMenu::handleLoadSettings() ==============================//
/**
* 
*/
bool AxisMapMenu::handleLoadSettings() {
	return true;
}

//===== bool AxisMapMenu::handleGenerateMap() ===============================//
/**
* 
*/
bool AxisMapMenu::handleGenerateMap() {
	//Generate the map...

	if(!processSettings()) {
		fprintf(stderr, "AxisMapMenu: Could not process settings...");
		dumpState(stderr);
		return false;
	}

	/*
	if(!generateMap(this)) {
		fprintf(stderr, "Failed to generate the map.\n");
		return false;
	}*/
	//Run in a separate thread...
	SDL_Thread* thrd = SDL_CreateThread(thread_generateMap, this);

	//PG_LogMSG("Generated map in %i ms.\n", SDL_GetTicks() - start_ticks);
	//PG_LogConsole::Update();

	//FILE* out = fopen("output.txt", "w");
	//theMap->dump(out);
	//fclose(out);

	//Since we new the map in the generateMap() method, we need to reassign it
	// to the miniMap.
	//miniMap->setMap(theMap);

	//Finally, redraw the minimap widget:
	//miniMap->Redraw();

	return true;
}

//===== bool AxisMapMenu::handleAdvancedView() ==============================//
/**
* 
*/
bool AxisMapMenu::handleAdvancedView() {
	return true;
}

//===== bool AxisMapMenu::processSettings() =================================//
/**
* Processes the current settings that the user has selected. Note that this 
* function should be called when the user user clicks the generate map button.
* When this event (or any other that requires the map to be regenerated) is
* received, this method should be called to load the information from the
* widgets into the private data members of the object.
*
* @return true if everything has been processed successfully; false otherwise.
*/
bool AxisMapMenu::processSettings() {

	//Process Map Dimensions:
	const char* txMapSize = ddMapSize->GetText();
	if(strcmp(txMapSize, "Tiny") == 0) {
		mapWidth = 50;
		mapHeight = 50;
	}
	else if(strcmp(txMapSize, "Small") == 0) {
		mapWidth = 60;
		mapHeight = 60;
	}
	else if(strcmp(txMapSize, "Normal") == 0) {
		mapWidth = 70;
		mapHeight = 70;
	}
	else if(strcmp(txMapSize, "Large") == 0) {
		mapWidth = 80;
		mapHeight = 80;
	}
	else if(strcmp(txMapSize, "Huge") == 0) {
		mapWidth = 90;
		mapHeight = 90;
	}

	//Process Tiles Per Blotch
	if(radTilesPerBlotchFew->GetPressed()) {
		tilesPerBlotch = 1;
	} else if(radTilesPerBlotchNormal->GetPressed()) {
		tilesPerBlotch = 2;
	} else if(radTilesPerBlotchMany->GetPressed()) {
		tilesPerBlotch = 3;
	}

	//Process Deterioration
	if(radDeteriorationWeak->GetPressed()) {
		deterioration = 10;
	} else if(radDeteriorationRegular->GetPressed()) {
		deterioration = 20;
	} else if(radDeteriorationStrong->GetPressed()) {
		deterioration = 40;
	}

	//Process Edge Gravity
	if(radEdgeGravityAttract->GetPressed()) {
		edgeGravity = 10;
	} else if(radEdgeGravityNone->GetPressed()) {
		edgeGravity = 0;
	} else if(radEdgeGravityRepell->GetPressed()) {
		edgeGravity = -10;
	}

	return true;
}

void AxisMapMenu::initMap() {

	mapWidth = DEFAULT_MAP_WIDTH;
	mapHeight = DEFAULT_MAP_HEIGHT;
	tilesPerBlotch = DEFAULT_TILES_PER_BLOTCH;
	deterioration = DEFAULT_DETERIORATION;
	edgeGravity = DEFAULT_EDGE_GRAVITY;

	theMap = new Map(mapWidth, mapHeight);
	blotcher = new Blotchmaker(*theMap);

	blotcher->SetAxisPalette(RED, GREEN, VIOLET, ORANGE);

	//generateMap();
}

//===== bool AxisMapMenu::generateMap() =====================================//
/**
* Generates the new map according to the settings the user has set.
*
* @return true upon success; false upon failure.
*/
int AxisMapMenu::generateMap() {
	//tilesPerBlotch = 0;
	
	Uint32 start_ticks = SDL_GetTicks();

	theMap->InitializeMap(0);

	// Group vars:
	blotcher->SetMaxBlotches(20);
	blotcher->SetTypeChances(2, 1, 0, 0, 0, 0, 0, 0, 0, 0); // lottery chances of each of the first two slots.

	// SLOT 0 - configured for "curliqueous clear"
	blotcher->SetBlotchType(0, 0); // obstacle zero = CLEAR mode
	blotcher->SetTilesPerBlotch(0, tilesPerBlotch);
	blotcher->SetModeChances(0, 0, 1, 0, 0); //chaotic
	blotcher->SetTurnChances(0, 20, 1, 1, 0); // mostly forward
	blotcher->SetBlankShotChance(0, 0);
	blotcher->SetEdgeGrav(0, edgeGravity);
	blotcher->SetEdgeDet(0, deterioration);
	blotcher->SetBrush(0, SP); // regular tile brush
	blotcher->SetStep(0, 1);

	/**/
	// SLOT 1 - configured for "bubbles"
	blotcher->SetBlotchType(1, 1); // obstacle 1 = BLOCK mode
	blotcher->SetTilesPerBlotch(1, tilesPerBlotch);
	blotcher->SetModeChances(1, 0, 1, 0, 0);
	blotcher->SetTurnChances(1, 1, 0, 0, 0);
	blotcher->SetBlankShotChance(1, 0);
	blotcher->SetEdgeGrav(1, edgeGravity);
	blotcher->SetEdgeDet(1, deterioration);
	blotcher->SetBrush(1, LC); // large circle brush
	blotcher->SetStep(1, 1);
	/**/

	//Blotch!
	blotcher->StartBlotching();

	/*
	if(bDEBUG) {
	FILE* output = fopen("axismapmenu_status.txt", "w");
	dumpState(output);
	}
	*/

	//Since we new the map in the generateMap() method, we need to reassign it
	// to the miniMap.
	//theMap = new Map(200, 200);
	//theMap->InitializeMap(1);
	miniMap->setMap(theMap);

	//Finally, redraw the minimap widget:
	miniMap->Redraw();

	fprintf(stdout, "Generated map in %d ms.\n", SDL_GetTicks() - start_ticks);


	return true;
}

//===== void AxisMapMenu::dumpState(FILE *out) ==============================//
/**
* Dumps the state of the menu into the file that was passed in.
*
* @param out the pointer to the file into which we want to write the status
*        of the menu.
*/
void AxisMapMenu::dumpState(FILE *out) {
	fprintf(out, "AxisMapMenu status:\n");
	fprintf(out, "\tMap Width: %d\n", mapWidth);
	fprintf(out, "\tMap Height: %d\n", mapHeight);
	fprintf(out, "\tTiles Per Blotch: %d\n", tilesPerBlotch);
	fprintf(out, "\tDeterioration: %d\n", deterioration);
	fprintf(out, "\tEdge Gravity: %d\n", edgeGravity);
	fprintf(out, "\tMap: %p\n", theMap);
	if(theMap != NULL) {
		theMap->dump(out);
	}
}