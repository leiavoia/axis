#include "AxisMiniMapWidget.h"
#include "pgdraw.h"
#include "pglog.h"

AxisMiniMapWidget::AxisMiniMapWidget(PG_Widget* parent, const PG_Rect& rect) : PG_ThemeWidget(parent, rect, true),
mapToDraw(NULL), miniMapSurface(NULL){
	//Everything is initialized above, really.
}

AxisMiniMapWidget::~AxisMiniMapWidget() {
	// here we could do some cleanup
	delete miniMapSurface;
}

void AxisMiniMapWidget::setMap(Map* map) {

	/*
	miniMapSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
	10, 10, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	for(int r=1; r<=10; r++) {
	for(int c=1; c<=10; c++) {
	if((r == c) || ((r + c) == 10)) {
	putPixel(miniMapSurface, c, r, SDL_MapRGB(miniMapSurface->format, 200, 50, 10));
	}
	}
	} 
	*/

	//This only affects the "blurryness" of the scaled minimap. The visible size will not be affected.
	int tileWidth = 10;
	int tileHeight = 10;

	mapToDraw = map;

	//If the map is not null, can create a new surface out of it
	if(mapToDraw != NULL) {
		int o = 0;

		if(miniMapSurface != NULL) {
			SDL_FreeSurface(miniMapSurface);
		}

		miniMapSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
			mapToDraw->GetCols() * tileWidth, mapToDraw->GetRows() * tileHeight,
			32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

		//This guy should be defined globally somewhere...
		Uint32 colors[14] = {
			SDL_MapRGB(miniMapSurface->format, 0xff, 0xff, 0xff),    //Empty
				SDL_MapRGB(miniMapSurface->format, 0x00, 0x00, 0x00),    //Block
				SDL_MapRGB(miniMapSurface->format, 0x00, 0xc0, 0xff),    //Water
				SDL_MapRGB(miniMapSurface->format, 0x65, 0x3c, 0x25),    //Pit
				SDL_MapRGB(miniMapSurface->format, 0x65, 0x65, 0x65),    //Barrier
				SDL_MapRGB(miniMapSurface->format, 0x65, 0x65, 0x65),    //Paint
				SDL_MapRGB(miniMapSurface->format, 0xff, 0x00, 0x00),    //Red Base
				SDL_MapRGB(miniMapSurface->format, 0xff, 0x89, 0x13),    //Orange Base
				SDL_MapRGB(miniMapSurface->format, 0xff, 0xff, 0x00),    //Yellow Base
				SDL_MapRGB(miniMapSurface->format, 0x00, 0xff, 0x00),    //Green Base
				SDL_MapRGB(miniMapSurface->format, 0x00, 0x00, 0xff),    //Blue Base
				SDL_MapRGB(miniMapSurface->format, 0x99, 0x00, 0x99),    //Violet Base
				SDL_MapRGB(miniMapSurface->format, 0x20, 0x20, 0x20),    //White Base
				SDL_MapRGB(miniMapSurface->format, 0x65, 0x65, 0x65)};   //Default Base

			//if (SDL_LockSurface(miniMapSurface) < 0) {
			//	fprintf(stderr, "Can't lock MiniMap surface: %s\n", SDL_GetError());
			//	return;
			//}

			for(int r=0; r<mapToDraw->GetRows(); r++) {
				for(int c=0; c<mapToDraw->GetCols(); c++) {
					o = mapToDraw->GetObst(c, r);

					if( o < 0 || o >= 14) {
						fprintf(stderr, "Bad value of obstacle received: %d at (%d, %d)\n", o, c, r);
						//PG_LogMSG("Bad value of o received from BM: %i", o);
						//PG_LogConsole::Update();
						continue;
					}
					//putPixel(miniMapSurface, c, r, colors[o]);
					PG_Rect tile(c * tileWidth, r * tileHeight, tileWidth, tileHeight);
					SDL_FillRect(miniMapSurface, tile.SDLRect(), colors[o]);
				}
			}
			//SDL_UnlockSurface(miniMapSurface);
	}
	else {
		miniMapSurface = NULL;
	}
}

void AxisMiniMapWidget::putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

void AxisMiniMapWidget::eventDraw(SDL_Surface* surface, const PG_Rect& rect) {
	//Call the parent's drawing method...
	PG_ThemeWidget::eventDraw(surface, rect);

	/**
	SDL_Color my_color;

	// the red value
	my_color.r = 200;
	// the green value
	my_color.g = 50;
	// the blue value
	my_color.b = 10;

	miniMapSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
	10, 10, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	for(int r=1; r<=10; r++) {
	for(int c=1; c<=10; c++) {
	if((r == c) || ((r + c) == 10)) {
	putPixel(miniMapSurface, c, r, SDL_MapRGB(miniMapSurface->format, my_color.r, my_color.g, my_color.b));
	}
	}
	} */

	if(miniMapSurface != NULL) {
		//Scale the minimap surface to the size of the widget:
		SDL_Surface* scaledMap = PG_Draw::ScaleSurface(miniMapSurface,
			static_cast<Uint16>(surface->w),
			static_cast<Uint16>(surface->h));

		//Don't scale the surface:
		//SDL_Surface* scaledMap = miniMapSurface;

		//PG_Rect* rectSrc = new PG_Rect(0, 0, miniMapSurface->w, miniMapSurface->h);
		PG_Rect* rectSrc = new PG_Rect(0, 0, scaledMap->w, scaledMap->h);
		PG_Rect* rectDst = new PG_Rect(rect);

		//SDL_FreeSurface(surface);
		PG_ThemeWidget::eventDraw(surface, rect);

		SDL_BlitSurface(scaledMap, rectSrc->SDLRect(),
			surface, rectDst->SDLRect());
	}
}