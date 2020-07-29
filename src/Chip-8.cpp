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

void Chip8::ROM(string filename){
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

		for (unsigned int i = 0; i < size; i++)
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
void Chip8::OP_3xkk()
{

}

void Chip8::OP_4xkk()
{
}

void Chip8::OP_5xy0()
{
}

void Chip8::OP_6xkk()
{
}

void Chip8::OP_7xkk()
{
}

void Chip8::OP_8xy0()
{
}

void Chip8::OP_8xy1()
{
}

void Chip8::OP_8xy2()
{
}

void Chip8::OP_8xy3()
{
}

void Chip8::OP_8xy4()
{
}

void Chip8::OP_8xy5()
{
}

void Chip8::OP_8xy6()
{
}

void Chip8::OP_8xy7()
{
}

void Chip8::OP_8xyE()
{
}

void Chip8::OP_9xy0()
{
}

void Chip8::OP_Annn()
{
}

void Chip8::OP_Bnnn()
{
}

void Chip8::OP_Cxkk()
{
}

void Chip8::OP_Dxyn()
{
}

void Chip8::OP_Ex9E()
{
}

void Chip8::OP_ExA1()
{
}

void Chip8::OP_Fx07()
{
}

void Chip8::OP_Fx0A()
{
}

void Chip8::OP_Fx15()
{
}

void Chip8::OP_Fx18()
{
}

void Chip8::OP_Fx1E()
{
}

void Chip8::OP_Fx29()
{
}

void Chip8::OP_Fx33()
{
}

void Chip8::OP_Fx55()
{
}

void Chip8::OP_Fx65()
{
}

void Chip8::OP_NULL()
{

}


