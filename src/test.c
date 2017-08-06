#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "test.h"
#include "chip8.h"

bool test_instr(bool (*test_instr_ptr)(Chip8 *))
{
	Chip8 *c8 = new_chip8();
	bool result = c8 != NULL && test_instr_ptr(c8);
	free(c8);
	return result;
}

bool test_new(Chip8 *c8)
{
	return c8 != NULL && c8->PC == 0x200;
}

bool test_0NNN(Chip8 *c8)
{
	// TODO
	return true;
}

bool test_00E0(Chip8 *c8)
{
	memset(c8->display, 1, 64 * 32 * sizeof(*c8->display));
	
	exec_instr(c8, 0x00E0);
	
	for(int i = 0; i < 64 * 32; i++) {
		if(c8->display[i]) return false;
	}
	
	return true;
}

bool test_00EE(Chip8 *c8)
{
	c8->PC = 0x400;
	c8->stack[0] = 0x200;
	c8->SP = 1;
	
	exec_instr(c8, 0x00EE);
	
	return c8->SP == 0 && c8->PC == 0x202;
}

bool test_1NNN(Chip8 *c8)
{
	c8->PC = 0x400;
	
	exec_instr(c8, 0x1200);
	
	return c8->PC == 0x200;
}

bool test_2NNN(Chip8 *c8)
{
	c8->PC = 0x400;
	c8->SP = 0;
	
	exec_instr(c8, 0x2200);
	
	return c8->PC == 0x200 && c8->SP == 1;
}

bool test_3XNN(Chip8 *c8)
{
	c8->PC = 0x200;
	c8->V[0] = 0xFF;
	
	exec_instr(c8, 0x30FF);
	
	return c8->PC == 0x204;
}

bool test_4XNN(Chip8 *c8)
{
	c8->PC = 0x200;
	c8->V[0] = 0xFF;
	
	exec_instr(c8, 0x4000);
	
	return c8->PC == 0x204;
}

bool test_5XY0(Chip8 *c8)
{
	c8->PC = 0x200;
	c8->V[0] = 0xFF;
	c8->V[1] = 0xFF;
	
	exec_instr(c8, 0x5010);
	
	return c8->PC == 0x204;
}

bool test_6XNN(Chip8 *c8)
{
	c8->V[0] = 0x00;
	
	exec_instr(c8, 0x60FF);
	
	return c8->V[0] == 0xFF;
}

bool test_7XNN(Chip8 *c8)
{
	c8->V[0] = 0x01;
	
	exec_instr(c8, 0x7001);
	
	return c8->V[0] == 0x02;
}

bool test_8XY0(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x02;
	
	exec_instr(c8, 0x8010);
	
	return c8->V[0] == 0x02;
}

bool test_8XY1(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x8011);
	
	return c8->V[0] == 0x11;
}

bool test_8XY2(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x8012);
	
	return c8->V[0] == 0x01;
}

bool test_8XY3(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x8013);
	
	return c8->V[0] == 0x10;
}

bool test_8XY4(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x8014);
	
	return c8->V[0] == 0x12 && c8->V[0xF] == 0;
}

bool test_8XY5(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x8015);
	
	return c8->V[0] == 0xF0 && c8->V[0xF] == 0;
}

bool test_8XY6(Chip8 *c8)
{
	c8->V[0] = 0x03;
	
	exec_instr(c8, 0x8016);
	
	return c8->V[0] == 0x01 && c8->V[0xF] == 0x01;
}

bool test_8XY7(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x8017);
	
	return c8->V[0] == 0x10 && c8->V[0xF] == 0x01;
}

bool test_8XYE(Chip8 *c8)
{
	c8->V[0] = 0x03;
	
	exec_instr(c8, 0x801E);
	
	return c8->V[0] == 0x06 && c8->V[0xF] == 0x00;
}

bool test_9XY0(Chip8 *c8)
{
	c8->PC = 0x200;
	c8->V[0] = 0x01;
	c8->V[1] = 0x11;
	
	exec_instr(c8, 0x9010);
	
	return c8->PC == 0x204;
}

bool test_ANNN(Chip8 *c8)
{
	c8->I = 0x400;
	
	exec_instr(c8, 0xA200);
	
	return c8->I == 0x200;
}

bool test_BNNN(Chip8 *c8)
{
	c8->V[0] = 0x02;
	c8->PC = 0x400;
	
	exec_instr(c8, 0xB200);
	
	return c8->PC == 0x202;
}

bool test_CXNN(Chip8 *c8)
{
	c8->V[0] == 0x01;
	
	exec_instr(c8, 0xC002);
	
	return c8->V[0] == 0x00 || c8->V[0] == 0x02;
}

bool test_DXYN(Chip8 *c8)
{
	c8->V[0] = 0x00;
	c8->V[1] = 0x00;
	c8->I = 0x200;
	
	int h = 8;
	for(int y = 0; y < h; y++) {
		c8->memory[c8->I + y] = 0xFF;
	}
	
	exec_instr(c8, 0xD018);
	
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < 8; x++) {
			if(!c8->display[y * 64 + x]) 
				return false;
		}
	}
		
	return c8->V[0xF] == 0;
}

bool test_EX9E(Chip8 *c8)
{
	c8->PC = 0x200;
	c8->V[0] = 0x00;
	c8->input[0] = 0x01;
	
	exec_instr(c8, 0xE09E);
	
	return c8->PC == 0x204;
}

bool test_EXA1(Chip8 *c8)
{
	
	c8->PC = 0x200;
	c8->V[0] = 0x00;
	c8->input[0] = 0x00;
	
	exec_instr(c8, 0xE0A1);
	
	return c8->PC == 0x204;
}

bool test_FX07(Chip8 *c8)
{
	c8->delay_timer = 11;
	
	exec_instr(c8, 0xF007);
	
	return c8->V[0] == 11;
}

bool test_FX0A(Chip8 *c8)
{
	// TODO
	return true;
}

bool test_FX15(Chip8 *c8)
{
	c8->V[0] = 11;
	
	exec_instr(c8, 0xF015);
	
	return c8->delay_timer == 10;
}

bool test_FX18(Chip8 *c8)
{
	c8->V[0] = 11;
	
	exec_instr(c8, 0xF018);
	
	return c8->sound_timer == 10;
}

bool test_FX1E(Chip8 *c8)
{
	c8->V[0] = 0x01;
	
	exec_instr(c8, 0xF01E);
	
	return c8->I == 0x01;
}

bool test_FX29(Chip8 *c8)
{
	// TODO
	
	return true;
}

bool test_FX33(Chip8 *c8)
{
	c8->V[0] = 123;
	
	exec_instr(c8, 0xF033);
	
	return c8->memory[c8->I] == 1 && c8->memory[c8->I + 1] == 2 && c8->memory[c8->I + 2] == 3;
}

bool test_FX55(Chip8 *c8)
{
	c8->V[0] = 0x01;
	c8->V[1] = 0x02;
	c8->V[2] = 0x03;
	c8->V[3] = 0x04;
	
	exec_instr(c8, 0xF355);
	
	return c8->memory[c8->I] == 0x01 && c8->memory[c8->I + 1] == 0x02 && 
			c8->memory[c8->I + 2] && c8->memory[c8->I + 3];
}

bool test_FX65(Chip8 *c8)
{
	c8->memory[c8->I	] = 0x01;
	c8->memory[c8->I + 1] = 0x02;
	c8->memory[c8->I + 2] = 0x03;
	c8->memory[c8->I + 3] = 0x04;
	
	exec_instr(c8, 0xF365);
	
	return c8->V[0] == 0x01 && c8->V[1] == 0x02 && 
			c8->V[2] == 0x03 && c8->V[3] == 0x04;
}
