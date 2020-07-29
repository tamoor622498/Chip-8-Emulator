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

class Chip8 {
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

};