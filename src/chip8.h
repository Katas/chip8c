#ifndef _CHIP8_H
#define _CHIP8_H

#include <stdint.h>
#include <stdbool.h>

extern const char FONT_DATA[];

typedef struct
{
	uint8_t 	memory[4096];
	uint8_t 	V[16];
	uint16_t 	I;
	uint16_t	stack[16];
	uint8_t		SP;
	uint16_t	PC;
	uint8_t		delay_timer;
	uint8_t		sound_timer;
	uint8_t		display[64 * 32];
	uint8_t		input[16];
	bool		draw;
} Chip8;

Chip8 *new_chip8();

bool load_rom(Chip8 *c8, char *path);

void exec_instr(Chip8 *c8, uint16_t op);

#endif
