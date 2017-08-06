#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "chip8.h"

const char FONT_DATA[] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8 *new_chip8()
{
	Chip8 *c8 = malloc(sizeof(Chip8));
	
	c8->I 	= 0;
	c8->SP 	= 0;
	c8->PC	= 0x200;
	c8->delay_timer = 0;
	c8->sound_timer = 0;
	c8->draw = true;
	
	memset(c8->memory, 0, 4096 * sizeof(*c8->memory));
	memset(c8->V, 0, 16 * sizeof(*c8->V));
	memset(c8->stack,0, 16 * sizeof(*c8->stack));
	memset(c8->display, 0, 64 * 32 * sizeof(*c8->display));
	memset(c8->input, 0, 16 * sizeof(*c8->input));

	memcpy(c8->memory, FONT_DATA, 80 * sizeof(*FONT_DATA));
	
	return c8;
}

bool load_rom(Chip8 *c8, char *path) 
{
	FILE *file = fopen(path, "rb");
	if(!file) 
		return false;
	
	fread(&c8->memory[512], 4096 - 512, 1, file); // First 512b for fontset
	if(ferror(file))
		return false;
		
	return fclose(file) != EOF;
}

void exec_instr(Chip8 *c8, uint16_t op)
{
	uint8_t x = ((op & 0x0F00) >> 8);
	uint8_t y = ((op & 0x00F0) >> 4);
	
	bool inc_PC = true;
	
	switch(op & 0xF000) {
		case 0x0000:
			switch(op & 0x0FFF) {
				case 0x00E0:
					memset(c8->display, 0, 64 * 32 * sizeof(*c8->display));
					c8->draw = true;
					break;
				case 0x00EE:
					c8->PC = c8->stack[--c8->SP];
					break;
				default:				
					break;
			}
			break;
		case 0x1000:
			c8->PC = (op & 0x0FFF);
			inc_PC = false;
			break;
		case 0x2000:
			c8->stack[c8->SP++] = c8->PC;
			c8->PC = (op & 0x0FFF);
			inc_PC = false;
			break;
		case 0x3000:
			if(c8->V[x] == (op & 0x00FF)) c8->PC += 2;
			break;
		case 0x4000:
			if(c8->V[x] != (op & 0x00FF)) c8->PC += 2;
			break;
		case 0x5000:
			if(c8->V[x] == c8->V[y]) c8->PC += 2;
			break;
		case 0x6000:
			c8->V[x] = (op & 0x00FF);
			break;
		case 0x7000:
			c8->V[x] += (op & 0x00FF);
			break;
		case 0x8000:
			switch(op & 0x000F) {
				case 0x0000:
					c8->V[x] = c8->V[y];
					break;
				case 0x0001:
					c8->V[x] = (c8->V[x] | c8->V[y]);
					break;
				case 0x0002:
					c8->V[x] = (c8->V[x] & c8->V[y]);
					break;
				case 0x0003:
					c8->V[x] = (c8->V[x] ^ c8->V[y]);
					break;
				case 0x0004:
					if((c8->V[x] + c8->V[y]) > 0xFF) {
						c8->V[0xF] = 1;
					} else {
						c8->V[0xF] = 0;
					}
					c8->V[x] += c8->V[y];
					break;
				case 0x0005:
					if(c8->V[x] - c8->V[y] < 0) {
						c8->V[0xF] = 0;
					} else {
						c8->V[0xF] = 1;
					}
					c8->V[x] -= c8->V[y];
					break;
				case 0x0006:
					c8->V[0xF] = c8->V[x] & 0x1;
					c8->V[x] >>= 1;
					break;
				case 0x0007:
					if(c8->V[y] - c8->V[x] < 0) {
						c8->V[0xF] = 0;
					} else {
						c8->V[0xF] = 1;
					}
					c8->V[x] = c8->V[y] - c8->V[x];
					break;
				case 0x000E:
					c8->V[0xF] = ((c8->V[x] & 0x80) >> 7);
					c8->V[x] <<= 1;
					break;
			}
			break;
		case 0x9000:
			if(c8->V[x] != c8->V[y]) {
				c8->PC += 2;
			}
			break;
		case 0xA000:
			c8->I = (op & 0x0FFF);
			break;
		case 0xB000:
			c8->PC = (((op & 0x0FFF) + c8->V[0]) & 0xFFF);
			inc_PC = false;
			break;
		case 0xC000:
			c8->V[x] = (rand() & op & 0xFF);
			break;
		case 0xD000:; // Necessary...
			uint8_t w = 8;
			uint8_t h = (op & 0x000F);
			
			c8->V[0xF] = 0;
			for(int ypos = 0; ypos < h; ypos++) {
				uint8_t abs_y = (c8->V[y] + ypos) % 32;;
				for(int xpos = 0; xpos < w; xpos++) {
					uint8_t abs_x = (c8->V[x] + xpos) % 64;
					if(c8->memory[c8->I + ypos] & (0x80 >> xpos)) {
						if(c8->display[abs_y * 64 + abs_x]) {
							c8->V[0xF] = 1;
						}
						c8->display[abs_y * 64 + abs_x] ^= 1;
					}
				}
			}
			c8->draw = true;
			break;
		case 0xE000:
			switch(op & 0x00FF) {
				case 0x009E:
					if(c8->input[c8->V[x]]) {
						c8->PC += 2;
					}
					break;
				case 0x00A1:
					if(!c8->V[x]) {
						c8->PC += 2;
					}
					break;
			}
			break;
		case 0xF000:
			switch(op & 0x00FF) {
				case 0x0007:
					c8->V[x] = c8->delay_timer;
					break;
				case 0x000A:
					c8->V[x] = 0;
					for(int i = 0; i < 16; i++) {
						if(c8->input[i]) {
							c8->V[x] = i;
							break;
						}
					}
					
					if(!c8->V[x]) return;
					
					break;
				case 0x0015:
					c8->delay_timer = c8->V[x];
					break;
				case 0x0018:
					c8->sound_timer = c8->V[x];
					break;
				case 0x001E:
					if(c8->I + c8->V[x] > 0x0FFF) {
						c8->V[0xF] = 1;
					} else {
						c8->V[0xF] = 0;
					}
					c8->I += c8->V[x];
					break;
				case 0x0029:
					c8->I = c8->V[x] * 5;
					break;
				case 0x0033:
					c8->memory[c8->I]		= c8->V[x] / 100;
					c8->memory[c8->I + 1]	= (c8->V[x] % 100) / 10;
					c8->memory[c8->I + 2]	= c8->V[x] % 10;
					break;
				case 0x0055:
					for(int i = 0; i <= x; i++) {
						c8->memory[c8->I + i] = c8->V[i];
					}
					break;
				case 0x0065:
					for(int i = 0; i <= x; i++) {
						c8->V[i] = c8->memory[c8->I + i];
					}
					break;
			}
			break;
	}
	if(inc_PC) 
		c8->PC += 2;
	
	if(c8->delay_timer)
		c8->delay_timer--;
	if(c8->sound_timer)
		c8->sound_timer--;
}
