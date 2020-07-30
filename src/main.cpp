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
	float delay = 1/500.0f;
	Platform()
	{
		// Name you application
		sAppName = "Chip-8 Emulator";
	}

public:
	bool OnUserCreate() override
	{
		emu.init();
		emu.ROM("TETRIS.ch8");

		// Called once at the start, so create things here
		return true;
	}



	bool ProcessInput(uint8_t* keys) {
		bool quit = false;

	}

	bool OnUserUpdate(float fElapsedTime) override {
	uint32_t vid[32][64];

	timecounter += fElapsedTime;
	if (timecounter >= delay)
	{
		
		emu.Cycle();

		int i = 0;
		for (int k = 0; k < 32; k++)
		{
			for (int j = 0; j < 64; j++)
			{
				vid[k][j] = emu.video[i];
				i++;
			}
		}
		


		for (int k = 0; k < 32; k++)
		{
			for (int j = 0; j < 64; j++)
			{
				if (vid[k][j] == 0xFFFFFFFF) {
					Draw(j, k, olc::Pixel(255, 255, 255));
				}
				else {
					Draw(j, k, olc::Pixel(0, 0, 0));
				}
			}
		}
		timecounter -= delay;
	}
	
		return true;
	}
};

int main()
{
	Platform demo;
	if (demo.Construct(64, 32, 25, 25))
		demo.Start();
	return 0;
}