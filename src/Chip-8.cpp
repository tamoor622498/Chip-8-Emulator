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

