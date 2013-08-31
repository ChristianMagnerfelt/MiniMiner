///
///	Author: Christian Magnerfelt
///	
///	Description: Mini Miner is a simple tile-matching puzzle game similar to Midas Miner
///
///
#include <SDL.h>

#include <cstdint>

int main( int argc, char* argv[] )
{
	SDL_Window * window;
	int32_t screenWidth = 800;
	int32_t screenHeight = 600;
	bool isRunning = true;

	SDL_Init(SDL_INIT_VIDEO);

	// Create an application window with the following settings:
	window = SDL_CreateWindow( 
		"MiniMiner",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_OPENGL
	);

	if(window == nullptr)
	{
			return 1;
	}

	// Enter main loop
	SDL_Event event;
	while(isRunning)
	{
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				isRunning = false;
		}
	}

	SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}