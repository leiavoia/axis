#ifndef __MINIMAP_WIDGET__
#define __MINIMAP_WIDGET__

#include "pgthemewidget.h"
#include "map.h"

/**
* This widget will draw the minimap on the screen.
*/
class AxisMiniMapWidget : public PG_ThemeWidget {
public:
	// the constructor
	AxisMiniMapWidget(PG_Widget* parent, const PG_Rect& rect);
	// the destructor
	~AxisMiniMapWidget();

	void setMap(Map* mp);
	Map* getMap() { return mapToDraw; }

protected:
	// our custom event handler to redraw our stuff
	void eventDraw(SDL_Surface* surface, const PG_Rect& rect);

private:
	Map* mapToDraw;
	SDL_Surface* miniMapSurface;

	void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
};

#endif  //__MINIMAP_WIDGET__