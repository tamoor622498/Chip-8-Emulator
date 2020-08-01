#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Chip-8.h"

using namespace std;

class Platform : public olc::PixelGameEngine
// Engine functionality inherated.
{
public:
	Chip8 emu;
	// Emulator object

	float timecounter = 0;
	// timer

	float delay = 1.0/240.0f;
	// Length of delay

	Platform()
	{
		sAppName = "Chip-8 Emulator";
		// Application name.
	}

	bool OnUserCreate() override
	{
		emu.init();
		// Initialized emulator

		emu.ROM("ROMS/IBM Logo.ch8");
		// ROM loaded

		return true;
	}
	// Called once at the start.

	bool ProcessInput(uint8_t* keys) {
		bool quit = false;

		if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::ESCAPE).bHeld) {
			quit = true;
		}
		else {
			quit = false;
		}

		if (GetKey(olc::Key::X).bPressed || GetKey(olc::Key::X).bHeld) {
			keys[0] = 1;
		}
		else {
			keys[0] = 0;
		}

		if (GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::K1).bHeld) {
			keys[1] = 1;
		}
		else {
			keys[1] = 0;
		}

		if (GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::K2).bHeld) {
			keys[2] = 1;
		}
		else {
			keys[2] = 0;
		}

		if (GetKey(olc::Key::K3).bPressed || GetKey(olc::Key::K3).bHeld) {
			keys[3] = 1;
		}
		else {
			keys[3] = 0;
		}

		if (GetKey(olc::Key::Q).bPressed || GetKey(olc::Key::Q).bHeld) {
			keys[4] = 1;
		}
		else {
			keys[4] = 0;
		}

		if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::W).bHeld) {
			keys[5] = 1;
		}
		else {
			keys[5] = 0;
		}

		if (GetKey(olc::Key::E).bPressed || GetKey(olc::Key::E).bHeld) {
			keys[6] = 1;
		}
		else {
			keys[6] = 0;
		}

		if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::A).bHeld) {
			keys[7] = 1;
		}
		else {
			keys[7] = 0;
		}

		if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::S).bHeld) {
			keys[8] = 1;
		}
		else {
			keys[8] = 0;
		}

		if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::D).bHeld) {
			keys[9] = 1;
		}
		else {
			keys[9] = 0;
		}

		if (GetKey(olc::Key::Z).bPressed || GetKey(olc::Key::Z).bHeld) {
			keys[0xA] = 1;
		}
		else {
			keys[0xA] = 0;
		}

		if (GetKey(olc::Key::C).bPressed || GetKey(olc::Key::C).bHeld) {
			keys[0xB] = 1;
		}
		else {
			keys[0xB] = 0;
		}

		if (GetKey(olc::Key::K4).bPressed || GetKey(olc::Key::K4).bHeld) {
			keys[0xC] = 1;
		}
		else {
			keys[0xC] = 0;
		}

		if (GetKey(olc::Key::R).bPressed || GetKey(olc::Key::R).bHeld) {
			keys[0xD] = 1;
		}
		else {
			keys[0xD] = 0;
		}

		if (GetKey(olc::Key::F).bPressed || GetKey(olc::Key::F).bHeld) {
			keys[0xE] = 1;
		}
		else {
			keys[0xE] = 0;
		}

		if (GetKey(olc::Key::V).bPressed || GetKey(olc::Key::V).bHeld) {
			keys[0xF] = 1;
		}
		else {
			keys[0xF] = 0;
		}

		return quit;
	}
	// Maps the buttons to the Chip-8 keypad.
	// If a button is pressed or held, one is loaded into the array.

	bool OnUserUpdate(float fElapsedTime) override {

		timecounter += fElapsedTime;
		// Adds time passed.

		if (timecounter >= delay)
		{ // Only runs when enough time hsa passed.

			bool q = ProcessInput(emu.keypad);
			// Ret true if escape key is pressed.

			if (q) {
				exit(0);
				cout << endl;
			}
			// Exits if escape is pressesd.

			emu.Cycle();
			// Runs opcode

			int i = 0;
			for (int k = 0; k < 32; k++)
			{
				for (int j = 0; j < 64; j++)
				{
					if (emu.video[i] == 0xFFFFFFFF) {
						Draw(j, k, olc::Pixel(255, 255, 255));
					}
					else {
						Draw(j, k, olc::Pixel(0, 0, 0));
					}
					i++;
				}
			}
			// Single array to 2D array.

			timecounter -= delay;
			// Resets timer
		}

		return true;
	}
};

int main()
{
	Platform out;
	if (out.Construct(64, 32, 25, 25, false, false, true))
		out.Start();
	return 0;
}