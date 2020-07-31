#include "Chip-8.h"

using namespace std;

Chip8::Chip8() {

}

void Chip8::init() {
	srand(time(NULL));
	// Seed rand

	pc = 0x200;
	// starting at addres of first op (0x200).
	opcode = 0;
	// Opcode reset;
	indexReg = 0;
	// Index register reset.
	sp = 0;
	// Stack pointer reset

	for (auto i = 0; i < (VIDEO_HEIGHT * VIDEO_WIDTH); i++)
	{
		memory[i] = 0;
	}
	// Clearing display.
	for (auto i = 0; i < STACK_LEVELS; i++)
	{
		stack[i] = 0;
	}
	// Clearing stack.
	for (auto i = 0; i < REGISTER_COUNT; i++)
	{
		reg[i] = 0;
	}
	// Clearing registers
	for (auto i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}
	// Clearing memory

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

	delayTimer = 0;
	soundTimer = 0;
	// Timers set.
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
	else {
		cout << "FILE INPUT ERROR!" << endl;
	}

}


void Chip8::Cycle() {
	opcode = (memory[pc] << 8u) | memory[pc + 1];
	// Opcodes are 2 bytes, memory is 1.
	// Need to grab first part, shift it left by a byte (8 bits).
	// Then inserting the second half by ORing with pc + 1.

	pc += 2;
	// pc incremented.

	switch (opcode & 0xF000) {
		case 0x1000:
			OP_1nnn();
			break;
		case 0x2000:
			OP_2nnn();
			break;
		case 0x3000:
			OP_3xkk();
			break;
		case 0x4000:
			OP_4xkk();
			break;
		case 0x5000:
			OP_5xy0();
			break;
		case 0x6000:
			OP_6xkk();
			break;
		case 0x7000:
			OP_7xkk();
			break;
		case 0x9000:
			OP_9xy0();
			break;
		case 0xA000:
			OP_Annn();
			break;
		case 0xB000:
			OP_Bnnn();
			break;
		case 0xC000:
			OP_Cxkk();
			break;
		case 0xD000:
			OP_Dxyn();
			break;
		case 0x8000:

			switch (opcode & 0x000F) {
				case 0x0000:
					OP_8xy0();
					break;
				case 0x0001:
					OP_8xy1();
					break;
				case 0x0002:
					OP_8xy2();
					break;
				case 0x0003:
					OP_8xy3();
					break;
				case 0x0004:
					OP_8xy4();
					break;
				case 0x0005:
					OP_8xy5();
					break;
				case 0x0006:
					OP_8xy6();
					break;
				case 0x0007:
					OP_8xy7();
					break;
				case 0x000E:
					OP_8xyE();
					break;

			default:
				printf("\n Invalid op code: %.4X\n", opcode);
				exit(0);
			}
			break;
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x0000:
					OP_00E0();
					break;
				case 0x000E:
					OP_00EE();
					break;
			default:
				printf("\n Invalid op code: %.4X\n", opcode);
				exit(0);
			}
			break;
		case 0xE000:
			switch (opcode & 0x00FF) {
				case 0x00A1:
					OP_ExA1();
					break;
				case 0x009E:
					OP_Ex9E();
					break;
			default:
				printf("\n Invalid op code: %.4X\n", opcode);
				exit(0);
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF) {
				case 0x0007:
					OP_Fx07();
					break;
				case 0x000A:
					OP_Fx0A();
					break;
				case 0x0015:
					OP_Fx15();
					break;
				case 0x0018:
					OP_Fx18();
					break;
				case 0x001E:
					OP_Fx1E();
					break;
				case 0x0029:
					OP_Fx29();
					break;
				case 0x0033:
					OP_Fx33();
					break;
				case 0x0055:
					OP_Fx55();
					break;
				case 0x0065:
					OP_Fx65();
					break;
			default:
				printf("\n Invalid op code: %.4X\n", opcode);
				exit(0);;
			}
			break;
	default:
		printf("\n Invalid op code: %.4X\n", opcode);
		exit(0);
	}
	// Massive Switch statement to decode and run opcodes.
	if (delayTimer > 0) {
		--delayTimer;
	}
	// Decaments delay timer.
	if (soundTimer > 0) {
		cout << '\a';
		--soundTimer;
	}
	// Decrements sound timer.
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
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (reg[Vx] != reg[Vy]) {
		pc += 2;
	}
}
// Grab Vx and Vy through bit masking.
// If Vx and Vy not equal, skip next instruction.
void Chip8::OP_Annn() {
	uint16_t address = opcode & 0x0FFFu;

	indexReg = address;
}
// Get nnn through bit masking.
// Set indexReg to nnn.
void Chip8::OP_Bnnn() {
	uint16_t address = opcode & 0x0FFFu;

	pc = reg[0] + address;
}
// Grab nnn via bit masking.
// Jump to nnn + V0. V0 is the first register.
void Chip8::OP_Cxkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	reg[Vx] = (rand() % 256u) + byte;
}
// Grab Vx and kk through bit masking.
// reg Vx is equal a random number between 0 255 and kk
void Chip8::OP_Dxyn() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	uint8_t xPos = reg[Vx] % VIDEO_WIDTH;
	uint8_t yPos = reg[Vy] % VIDEO_HEIGHT;

	//uint8_t xPos = reg[Vx];
	//uint8_t yPos = reg[Vy];

	//if (xPos > VIDEO_WIDTH || xPos < 0) {
	//	xPos = VIDEO_WIDTH/2;
	//}
	//if (yPos > VIDEO_HEIGHT || yPos < 0) {
	//	yPos = VIDEO_HEIGHT-3;
	//}


	reg[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[indexReg + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			if (spritePixel) {
				if (*screenPixel == 0xFFFFFFFF) {
					reg[0xF] = 1;
				}
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}
// Grabs Vx Vy height through bit masking.
// Iterates over new pixels and XORs them.
// If a pixel is already active, carry flag is set.
void Chip8::OP_Ex9E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[reg[Vx]])
	{
		pc += 2;
	}
}
// Grab Vx via bit masking.
// If key in Vx is pressed, skip next instruction.
void Chip8::OP_ExA1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (!keypad[reg[Vx]])
	{
		pc += 2;
	}
}
// Grab Vx via bit masking.
// If key in Vx is not pressed, skip next instruction.
void Chip8::OP_Fx07() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	reg[Vx] = delayTimer;
}
// Set Vx equal to delay timer.
void Chip8::OP_Fx0A() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[0])
	{
		reg[Vx] = 0;
	}
	else if (keypad[1])
	{
		reg[Vx] = 1;
	}
	else if (keypad[2])
	{
		reg[Vx] = 2;
	}
	else if (keypad[3])
	{
		reg[Vx] = 3;
	}
	else if (keypad[4])
	{
		reg[Vx] = 4;
	}
	else if (keypad[5])
	{
		reg[Vx] = 5;
	}
	else if (keypad[6])
	{
		reg[Vx] = 6;
	}
	else if (keypad[7])
	{
		reg[Vx] = 7;
	}
	else if (keypad[8])
	{
		reg[Vx] = 8;
	}
	else if (keypad[9])
	{
		reg[Vx] = 9;
	}
	else if (keypad[10])
	{
		reg[Vx] = 10;
	}
	else if (keypad[11])
	{
		reg[Vx] = 11;
	}
	else if (keypad[12])
	{
		reg[Vx] = 12;
	}
	else if (keypad[13])
	{
		reg[Vx] = 13;
	}
	else if (keypad[14])
	{
		reg[Vx] = 14;
	}
	else if (keypad[15])
	{
		reg[Vx] = 15;
	}
	else
	{
		pc -= 2;
	}
}
// Wait till key is pressed, save in Vx.
void Chip8::OP_Fx15() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = reg[Vx];
}
// Set delay timer to Vx
void Chip8::OP_Fx18() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = reg[Vx];
}
// Set soundTimer equal to Vx.
void Chip8::OP_Fx1E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	indexReg += reg[Vx];
}
// Add Vx to indexReg.
void Chip8::OP_Fx29() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = reg[Vx];

	indexReg = FONTSET_START_ADDRESS + (5 * digit);
}
// indexReg is set to character Vx (Loaded in earlier).
void Chip8::OP_Fx33() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = reg[Vx];

	memory[indexReg + 2] = value % 10;
	value /= 10;

	memory[indexReg + 1] = value % 10;
	value /= 10;

	memory[indexReg] = value % 10;
}
// Storing Binary Coded Decimal representation of Vx in indexReg to indexReg+2.
void Chip8::OP_Fx55() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[indexReg + i] = reg[i];
	}
}
// Storing V0 to Vx in memory.
void Chip8::OP_Fx65() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		reg[i] = memory[indexReg + i];
	}
}
// Reading in register V0 to Vx from memory starting at indexReg.
void Chip8::OP_NULL() {
	// A whole bunch of nothing.
}


