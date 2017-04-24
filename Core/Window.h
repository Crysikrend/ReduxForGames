#ifndef WINDOWH
#define WINDOWH

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

#include <stdio.h>
#include <string>
#include <vector>

class Window {
public:
	Window(int initialWidth, int initialHeight, bool windowed);

	void checkWindow(SDL_Event);
	void toggleFullscreen();
	void swapWindow();

	bool getWindowOK() const;

	bool getWindowResized();
	int getWindowWidth() const;
	int getWindowHeight() const;

	void release();


private:
	bool _windowed;
	int _screenWidth, _screenHeight;

	SDL_Window* _window;
	SDL_GLContext _context;
	Uint32 _flags;

	bool _windowResized;
	bool _windowOK;

	void newWindow();
};
#endif