#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <SDL.h>
#include "test.h"
#include "chip8.h"
#include "display.h"

#define IPS 500

const int KEY_MAP[16] =
{
	SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
	SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
	SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
	SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
};

void test_all_instrs()
{
	assert(test_instr(test_new));
	assert(test_instr(test_0NNN));
	assert(test_instr(test_00E0));
	assert(test_instr(test_00EE));
	assert(test_instr(test_1NNN));
	assert(test_instr(test_2NNN));
	assert(test_instr(test_3XNN));
	assert(test_instr(test_4XNN));
	assert(test_instr(test_5XY0));
	assert(test_instr(test_6XNN));
	assert(test_instr(test_7XNN));
	assert(test_instr(test_8XY0));
	assert(test_instr(test_8XY1));
	assert(test_instr(test_8XY2));
	assert(test_instr(test_8XY3));
	assert(test_instr(test_8XY4));
	assert(test_instr(test_8XY5));
	assert(test_instr(test_8XY6));
	assert(test_instr(test_8XY7));
	assert(test_instr(test_8XYE));
	assert(test_instr(test_9XY0));
	assert(test_instr(test_ANNN));
	assert(test_instr(test_BNNN));
	assert(test_instr(test_CXNN));
	assert(test_instr(test_DXYN));
	assert(test_instr(test_EX9E));
	assert(test_instr(test_EXA1));
	assert(test_instr(test_FX07));
	assert(test_instr(test_FX0A));
	assert(test_instr(test_FX15));
	assert(test_instr(test_FX18));
	assert(test_instr(test_FX1E));
	assert(test_instr(test_FX29));
	assert(test_instr(test_FX33));
	assert(test_instr(test_FX55));
	assert(test_instr(test_FX65));
}

int main(int argc, char *argv[])
{
	test_all_instrs();
	
	Chip8 *c8 = new_chip8();
	
	if(argc < 2) {
		printf("Usage: %s <rom_path>\n", argv[0]);
		return 1;
	}
	
	if(!load_rom(c8, argv[1])) {
		printf("Could not load rom %s\n", argv[1]);
		return 2;
	}
	
	uint8_t scale = 10;
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Failed to initialize output.\n");
		return 3;
	}
	
	SDL_Window *window = SDL_CreateWindow("Chip8 - ",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							64 * scale,
							32 * scale,
							SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printf("Could not create SDL window: %s\n", SDL_GetError());
		return 4;
	}
	
	SDL_Surface *window_surface = SDL_GetWindowSurface(window);
	SDL_Surface *c8_surface = create_c8_surface();
	
	if(window_surface == NULL || c8_surface == NULL) {
		printf("Could not create surfaces: %s\n", SDL_GetError());
		return 5;
	}
	
	uint32_t colors[] = { 0x000000FF, 0xFF0000FF };
	bool quit = false;
	SDL_Event e;
	while(!quit) {
		uint32_t starttime = SDL_GetTicks();
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}
		
		uint8_t *keystate = SDL_GetKeyboardState(NULL);
		for(int i = 0; i < 16; i++) {
			c8->input[i] = keystate[KEY_MAP[i]];
		}
		
		uint16_t op = ((c8->memory[c8->PC] << 8) | c8->memory[c8->PC + 1]);
		exec_instr(c8, op);
		
		if(c8->draw) {
			draw_c8_display(c8, c8_surface, colors, window_surface);
			SDL_UpdateWindowSurface(window);
		}
		
		if(SDL_GetTicks() - starttime < 1000 / IPS) {
			SDL_Delay(1000 / IPS);
		}
	}
	
	SDL_Quit();
	
	return 0;
}
