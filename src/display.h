#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "chip8.h"

SDL_Surface *create_c8_surface();
void draw_c8_display(Chip8 *c8, SDL_Surface *small_surface, uint32_t colors[], SDL_Surface *screen_surface);

#endif
