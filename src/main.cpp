#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name you application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		int array[64*32];
		int d[64][32];

		for (int i = 0; i < (64*32); i++)
		{
			array[i] = (i+1) % 2;
		}

		int i = 0;

		for (int k = 0; k < 64; k++)
		{
			for (int j = 0; j < 32; j++)
			{
				d[k][j] = array[i];
				i++;
			}
		}

		for (int k = 0; k < 64; k++)
		{
			for (int j = 0; j < 32; j++)
			{
				if (d[k][j] == 0) {
					Draw(k, j, olc::Pixel(0, 0, 0));
				}
				else {
					Draw(k, j, olc::Pixel(255, 255, 255));
				}
			}
		}
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(64, 32, 25, 25))
		demo.Start();
	return 0;
}