#if defined(__linux__)
/* Linux */
#include <SDL2/SDL.h>
#else
/* Windows/else */
#include <SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif

//#include "gl_core_3_2.hpp"

#include <cstdio>
#include <string>
#include "Game.h"
#include "Window.h"

// create and start the window
bool initialise(Window*&, int, int);
// Set up additional OpenGL functionality
//bool initGL(SDL_Window*, SDL_GLContext&);
//void setupGL();
// free up all space
void release(Game&, Window*);

int main(int argc, char* args[]) {

	Window* window = NULL;
	float screenWidth = 1920;
	float screenHeight = 1080;

	bool quit = false;

	//Event handler
	SDL_Event e;

	if (initialise(window, screenWidth, screenHeight)) {

		Game game(window);
		game.initialise();

		float dTime = 0;

		while (!quit) {


			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				window->checkWindow(e);

				//User requests quit
				if (e.type == SDL_QUIT)
					quit = true;

			}

			if (!quit) {
				float ticks = SDL_GetTicks();
				quit = game.update((ticks - dTime) / 1500, e);
				game.render(ticks - dTime);

				dTime = ticks;
			}
		}
		// Release game data and SDL window
		release(game, window);
	}

	return 0;
}

bool initialise(Window*& window, int gScreenWidth, int gScreenHeight) {
	
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	window = new Window(gScreenWidth, gScreenHeight, true);
	return window->getWindowOK();
}

void release(Game& game, Window* window)
{
	// Release game memory
	game.release();
	window->release();
	delete window;

	//Quit SDL subsystems
	SDL_Quit();
}