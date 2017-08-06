#ifndef _TEST_H
#define _TEST_H

#include "chip8.h"

bool test_instr(bool (*test_instr_ptr)(Chip8 *));
bool test_new(Chip8 *c8);
bool test_0NNN(Chip8 *c8);
bool test_00E0(Chip8 *c8);
bool test_00EE(Chip8 *c8);
bool test_1NNN(Chip8 *c8);
bool test_2NNN(Chip8 *c8);
bool test_3XNN(Chip8 *c8);
bool test_4XNN(Chip8 *c8);
bool test_5XY0(Chip8 *c8);
bool test_6XNN(Chip8 *c8);
bool test_7XNN(Chip8 *c8);
bool test_8XY0(Chip8 *c8);
bool test_8XY1(Chip8 *c8);
bool test_8XY2(Chip8 *c8);
bool test_8XY3(Chip8 *c8);
bool test_8XY4(Chip8 *c8);
bool test_8XY5(Chip8 *c8);
bool test_8XY6(Chip8 *c8);
bool test_8XY7(Chip8 *c8);
bool test_8XYE(Chip8 *c8);
bool test_9XY0(Chip8 *c8);
bool test_ANNN(Chip8 *c8);
bool test_BNNN(Chip8 *c8);
bool test_CXNN(Chip8 *c8);
bool test_DXYN(Chip8 *c8);
bool test_EX9E(Chip8 *c8);
bool test_EXA1(Chip8 *c8);
bool test_FX07(Chip8 *c8);
bool test_FX0A(Chip8 *c8);
bool test_FX15(Chip8 *c8);
bool test_FX18(Chip8 *c8);
bool test_FX1E(Chip8 *c8);
bool test_FX29(Chip8 *c8);
bool test_FX33(Chip8 *c8);
bool test_FX55(Chip8 *c8);
bool test_FX65(Chip8 *c8);

#endif
