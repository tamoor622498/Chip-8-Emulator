#pragma once

#include "olcPixelGameEngine.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <time.h>
#include <string>

using namespace std;

const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;
const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

class Chip8 : public olc::PixelGameEngine {
public:
	Chip8();
	// Start up.
	void ROM(string filename);
	// Reads in rom.
	void Cycle();
	// Emulator cycle/

	uint8_t keypad[KEY_COUNT];
	// Hex values 0 to F.
	uint32_t video[VIDEO_HEIGHT * VIDEO_WIDTH];
	// Either a 1 or 0
	uint8_t reg[REGISTER_COUNT];
	// 16 8-bit registers, V0 to VF. VF is Carry flag.
	uint8_t memory[MEMORY_SIZE];
	// 4096 bytes of memeory.
	// 0X000 - 0X1FF = Chip-8 Interpreter.
	// 0X050 - 0X0A0 = Storage space for the 16 (hex) characters.
	// 0X200 - 0XFFF = Storage of loaded rom.
	uint16_t indexReg;
	// Stores memory addresses/
	uint16_t pc = 0x200;
	// Holds the instructions to execute, starting at 0X200.
	uint16_t stack[STACK_LEVELS];
	// CALLS pc addess pushed to stack. RET pc addess popped
	uint8_t sp;
	// Stack "pointer".
	uint8_t delayTimer;
	// Decs at 60Hz. Controls game speed.
	uint8_t soundTimer;
	// Decs at 60Hz. Emits sound when not 0.
	uint16_t opcode;
	// The instruction being executed


//-------------- Opcodes --------------//
	void OP_00E0();
	// CLS
	void OP_00EE();
	// RET
	void OP_1nnn();
	// Jump to location nnn.
	void OP_2nnn();
	// Call subroutine at nnn.
	void OP_3xkk();
	// Skip next instruction if Vx = kk.
	void OP_4xkk();
	// Skip next instruction if Vx != kk.
	void OP_5xy0();
	// Skip next instruction if  Vx = Vy.
	void OP_6xkk();
	// Set Vx = kk.
	void OP_7xkk();
	// Set vx = Vx + kk.
	void OP_8xy0();
	// Set Vx = Vy.
	void OP_8xy1();
	// set OR Vx, Vy;
	void OP_8xy2();
	// Set Vx = Vx AND Vy.
	void OP_8xy3();
	// Set Vx = Vx XOR Vy.
	void OP_8xy4();
	// Set Vx = Vx + Vy, set VF = carry if needed.
	void OP_8xy5();
	// Set Vx = Vx - Vy, set VF = NOT borrow.
	void OP_8xy6();
	// Set Vx = Vx SHR 1.
	void OP_8xy7();
	// Set Vx = Vy - Vx, set VF = NOT borrow.
	void OP_8xyE();
	// Set Vx = Vx SHL 1.
	void OP_9xy0();
	// Skip next instruction if Vx != Vy.
	void OP_Annn();
	// Set I = nnn.
	void OP_Bnnn();
	// Set jump to location nnn + V0.
	void OP_Cxkk();
	// Set Vx = random byte AND kk.
	void OP_Dxyn();
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	void OP_Ex9E();
	// Skip next instruction if key with the value of Vx is pressed.
	void OP_ExA1();
	// Skip next instruction if key with the value of Vx is not pressed.
	void OP_Fx07();
	// Set Vx = dely timer value.
	void OP_Fx0A();
	// Set Wait for a key press, store the value of the key in Vx.
	void OP_Fx15();
	// Set delay timer = Vx.
	void OP_Fx18();
	// Set sound timer = Vx.
	void OP_Fx1E();
	// Set I = I + Vx.
	void OP_Fx29();
	// Set I = location of sprite for digit Vx.
	void OP_Fx33();
	// Store BCD representation of Vx in memory locations I, I+1, and I+2.
	void OP_Fx55();
	// Store registers V0 thorugh Vx in memory starting at location I.
	void OP_Fx65();
	// Read registers V0 through Vx from memory starting at location I.
	void OP_NULL();
	// Does nothing.
};