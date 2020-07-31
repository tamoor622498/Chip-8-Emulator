#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Chip-8.h"


using namespace std;


// Override base class with your custom functionality
class Platform : public olc::PixelGameEngine
{
public:
	Chip8 emu;
	float timecounter = 0;
	float delay = 0.0021f;
	Platform()
	{
		// Name you application
		sAppName = "Chip-8 Emulator";
	}

public:
	bool OnUserCreate() override
	{
		emu.init();
		emu.ROM("Brick (Brix hack, 1990).ch8");

		// Called once at the start, so create things here
		return true;
	}



	bool ProcessInput(uint8_t* keys) {
		bool quit = false;

		if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::ESCAPE).bHeld) {
			quit = true;
		} else {
			quit = false;
		}

		if (GetKey(olc::Key::X).bPressed || GetKey(olc::Key::X).bHeld) {
			keys[0] = 1;
		} else {
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

	bool OnUserUpdate(float fElapsedTime) override {
	uint32_t vid[32][64];

	timecounter += fElapsedTime;
	if (timecounter >= delay)
	{
		bool q = ProcessInput(emu.keypad);

		if (q) {
			exit(0);
			cout << endl;
		}

		emu.Cycle();

		int i = 0;
		for (int k = 0; k < 32; k++)
		{
			for (int j = 0; j < 64; j++)
			{
				/*vid[k][j] = emu.video[i];*/
				if (emu.video[i] == 0xFFFFFFFF) {
							Draw(j, k, olc::Pixel(255, 255, 255));
						}
						else {
							Draw(j, k, olc::Pixel(0, 0, 0));
						}
				i++;
			}
		}
		


		//for (int k = 0; k < 32; k++)
		//{
		//	for (int j = 0; j < 64; j++)
		//	{
		//		if (vid[k][j] == 0xFFFFFFFF) {
		//			Draw(j, k, olc::Pixel(255, 255, 255));
		//		}
		//		else {
		//			Draw(j, k, olc::Pixel(0, 0, 0));
		//		}
		//	}
		//}
		timecounter -= delay;
	}
	
		return true;
	}
};

int main()
{
	Platform demo;
	if (demo.Construct(64, 32, 25, 25, false, false))
		demo.Start();
	return 0;
}