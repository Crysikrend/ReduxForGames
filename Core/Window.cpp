#include "Window.h"

Window::Window(int initialWidth, int initialHeight, bool windowed) 
	:_screenWidth(initialWidth), _screenHeight(initialHeight), _windowed(windowed) {

	_windowOK = true;
	_windowResized = true;
	
	//Use OpenGL 3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	newWindow();
}

void Window::checkWindow(SDL_Event e) {
	if (_windowOK) {

		//If the window resized
		if (e.type == SDL_WINDOWEVENT)
			switch (e.window.event) {
			case (SDL_WINDOWEVENT_RESIZED):
				//Resize the screen
				_screenWidth = e.window.data1;
				_screenHeight = e.window.data2;
				_windowResized = true;
			}
	}
}

void Window::toggleFullscreen() {
	_windowed = !_windowed;
}

void Window::swapWindow() {
	SDL_GL_SwapWindow(_window);
}

bool Window::getWindowOK() const{
	return _windowOK;
}

bool Window::getWindowResized() {
	if (_windowResized) {
		_windowResized = false;
		return true;
	}

	return false;
}

int Window::getWindowWidth() const {
	return _screenWidth;
}

int Window::getWindowHeight() const {
	return _screenHeight;
}

void Window::release() {
	//Destroy window
	SDL_DestroyWindow(_window);
	_window = NULL;
}

void Window::newWindow() {

	release();

	//Set up the screen
	//screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);

	if (_windowed)
		_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	else
		_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;

	_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, _flags);
	if (_window == NULL) {
		printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
		_windowOK = false;
	}

	// Set up OpenGL
	_context = SDL_GL_CreateContext(_window);

	SDL_GL_MakeCurrent(_window, _context);
	glewInit();

	if (_context == NULL) {
		printf("OpenGL initialisation failed. SDL_Error: %s\n", SDL_GetError());
		_windowOK = false;
	}
	else {
		GLenum OGLerror = glGetError();
		if (OGLerror != GL_NO_ERROR) {
			printf("OpenGL initialisation failed. OpenglError: %s\n", glewGetErrorString(OGLerror));
			_windowOK = false;
		}
	}

	// Additional settings
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	GLenum OGLerror = glGetError();
	if (OGLerror != GL_NO_ERROR) {
		printf("Something went wrong in additional OpenGL setup - OpenglError: %s\n", glewGetErrorString(OGLerror));
		_windowOK = false;
	}

}