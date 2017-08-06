#include <SDL.h>
#include "display.h"

SDL_Surface *create_c8_surface()
{
	uint32_t rmask, gmask, bmask, amask;
	
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xFF000000;
	gmask = 0x00FF0000;
	bmask = 0x0000FF00;
	amask = 0x000000FF;
//#else
//	rmask = 0x000000FF;
//	gmask = 0x0000FF00;
//	bmask = 0x00FF0000;
//	amask = 0xFF000000;
//#endif
	
	return SDL_CreateRGBSurface(0, 64, 32, 32, rmask, gmask, bmask, amask);
}

void draw_c8_display(Chip8 *c8, SDL_Surface *small_surface, uint32_t colors[], SDL_Surface *screen_surface) // Source surface expected to be 64 x 32
{	
	if(SDL_MUSTLOCK(small_surface)) SDL_LockSurface(small_surface);
	
	uint32_t *pixels = small_surface->pixels;
	
	for(int y = 0; y < 32; y++) {
		for(int x = 0; x < 64; x++) {
			pixels[y * 64 + x] = colors[c8->display[y * 64 + x]];
		}
	}
	
	if(SDL_MUSTLOCK(small_surface)) SDL_UnlockSurface(small_surface);
	
	SDL_BlitScaled(small_surface, NULL, screen_surface, NULL);
	
	c8->draw = false;
}
