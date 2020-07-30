#include "Chip-8.h"

using namespace std;

Chip8::Chip8() {

	pc = START_ADDRESS;
	// starting at addres of first op (0x200).
	opcode = 0;
	// Opcode reset;
	indexReg = 0;
	// Index register reset.
	sp = 0;
	// Stack pointer reset

	// Need to:
	// Set video to zero.
	// Clear stack
	// Clear registers
	// Clear memory

	uint8_t fontset[FONTSET_SIZE] =
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
	// The Hex characters.

	for (auto i = 0; i < FONTSET_SIZE; i++)
	{
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
	//  Characters loaded into memory addresses 0x50 to 0xA0.
}

void Chip8::ROM(string filename) {
	ifstream file(filename, ios::binary | ios::ate);
	// File is opened as binary (ios::binary) and file pointe is moved to end (ios::ate)

	if (file.ifstream::is_open()) {

		streampos size = file.tellg();
		char* buffer = new char[size];
		// Get's the size of file based on the position of pointer.
		// Allocates a buffer the of that size;

		file.seekg(0, ios::beg);
		// Moves pointer to the start of the file.
		file.read(buffer, size);
		// File data loaded into buffer
		file.close();

		for (auto i = 0; i < size; i++)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}
		// File data loaded into memory starting at 0x200.

		delete[] buffer;
		// Buffer freed.

	}

}


void Chip8::Cycle() {

}

void Chip8::OP_00E0() {
	for (unsigned int i = 0; i < (VIDEO_HEIGHT * VIDEO_WIDTH); i++)
	{
		video[i] = 0; // 0  = black square
	}
}
// Clears the screen
void Chip8::OP_00EE() {
	--sp;
	pc = stack[sp];
}
// Top of stack has address of one instruction past we need.
// Program counter set to top of stack.
void Chip8::OP_1nnn() {
	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}
// Using bit masking, grab the address nnn.
// Set program counter to nnn.
void Chip8::OP_2nnn() {
	uint16_t address = opcode & 0x0FFFu;

	stack[sp] = pc;
	++sp;
	pc = address;
}
// Using bit masking grab address nnn.
// Current instruction put on stack.
// sp moved up.
// Program counter set to address.
void Chip8::OP_3xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	if (reg[Vx] == byte) {
		pc += 2;
	}
}
// Using bit masking, grab Vx and kk.
// Then shift Vx left by 8 bits (reducing order).
// If register Vx and kk equal, increment pc by 2.
void Chip8::OP_4xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	if (reg[Vx] != byte) {
		pc += 2;
	}
}
// Using bit masking, grab Vx and kk.
// Then shift Vx left by 8 bits (reducing order).
// If register Vx and kk not equal, increment pc by 2.
void Chip8::OP_5xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (reg[Vx] == reg[Vy]) {
		pc += 2;
	}
}
// Using bit masking, grab Vx and Vy.
// Then shift Vx left by 8 bits and Vy by 4 (reducing order).
// If register Vx and Vy equal, increment pc by 2.
void Chip8::OP_6xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	reg[Vx] = byte;
}
// Bit mask x and kk. Need to shift Vx to reduce order.
// Set Vx to byte
void Chip8::OP_7xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	reg[Vx] = reg[Vx] + byte;
}
// Bit mask x and kk. Need to shift Vx to reduce order.
// Add byte to Vx.
void Chip8::OP_8xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	reg[Vx] = reg[Vy];
}
// Bit masking x and y then shifting to reduce order.
// Set reg Vx equal to Vy.
void Chip8::OP_8xy1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	reg[Vx] = reg[Vx] | reg[Vy];
}
//  Bit masking x and y then shifting to reduce order.
// Set reg Vx equal to reg Vx OR Vy.
void Chip8::OP_8xy2() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	reg[Vx] = reg[Vx] & reg[Vy];
}
//  Bit masking x and y then shifting to reduce order.
// Set reg Vx equal to reg Vx AND Vy.
void Chip8::OP_8xy3() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	reg[Vx] = reg[Vx] ^ reg[Vy];
}
//  Bit masking x and y then shifting to reduce order.
// Set reg Vx equal to reg Vx XOR Vy.
void Chip8::OP_8xy4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = reg[Vx] + reg[Vy];

	if (sum > 255u) {
		reg[0xF] = 1;
	}
	else {
		reg[0xF] = 0;
	}

	reg[Vx] = sum & 0xFF;
}
//  Bit masking x and y then shifting to reduce order.
// The 2 8-bit Vx and Vy registers are added to 16-bit sum.
// If sum is greater than a byte (255 bits), carry flag is set.
// Vf is the 16th register, 0xF = 16.
// Vx is set to last 8 bits of sum, grab them by bit masking.
void Chip8::OP_8xy5() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (reg[Vx] > reg[Vy]) {
		reg[0xF] = 1;
	}
	else {
		reg[0xF] = 0;
	}

	reg[Vx] = reg[Vx] - reg[Vy];
}
// Vx and Vy are grabbed via bit shifting.
// If Vx is greater than Vy, carry is set to 1, else 0;
// Vx is set to Vx - Vy.
void Chip8::OP_8xy6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	reg[0xF] = (reg[Vx] & 0x1u);

	reg[Vx] = reg[Vx] >> 1;
}
// vx is grabbed via bit masking.
// The least signicant bit is saved in Vf.
// reg Vx is divided by 2 via shifitng left by 1.
void Chip8::OP_8xy7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (reg[Vy] > reg[Vx]) {
		reg[0xF] = 1;
	}
	else {
		reg[0xF] = 0;
	}

	reg[Vx] = reg[Vy] - reg[Vx];
}
// Vx and Vy are grabbed via bit shifting.
// If Vy is greater than Vx, carry is set to 1, else 0;
// Vx is set to Vy - Vx.
void Chip8::OP_8xyE() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	reg[0xF] = (reg[Vx] & 0x80u) >> 7u;

	reg[Vx] = reg[Vx] << 1;
}
// Vx is grabbed via bit masking.
// Most significant bit is grabbed and shifted to left, saved in carry.
// Vx is multiplied by shifting right by 1.
void Chip8::OP_9xy0() {
}

void Chip8::OP_Annn() {
}

void Chip8::OP_Bnnn() {
}

void Chip8::OP_Cxkk() {
}

void Chip8::OP_Dxyn() {
}

void Chip8::OP_Ex9E() {
}

void Chip8::OP_ExA1() {
}

void Chip8::OP_Fx07() {
}

void Chip8::OP_Fx0A() {
}

void Chip8::OP_Fx15() {
}

void Chip8::OP_Fx18() {
}

void Chip8::OP_Fx1E() {
}

void Chip8::OP_Fx29() {
}

void Chip8::OP_Fx33() {
}

void Chip8::OP_Fx55() {
}

void Chip8::OP_Fx65() {
}

void Chip8::OP_NULL() {
	// A whole bunch of nothing.
}


