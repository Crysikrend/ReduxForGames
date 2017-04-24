#include "Game.h"

Game::Game(Window* window) 
	: _window(window) {}

void Game::initialise()
{
	_shaders = new ShaderManager();

	_shaders->newShader("Shaders/", "simple");

	levelMGR = new LevelMGR(glm::vec3(0,0,0), _shaders);


		// Will always be true, but it sets bools for later
	if (_window->getWindowResized())
		initialiseNewRes();

}
void Game::initialiseNewRes() {
	_screenWidth = _window->getWindowWidth();
	_screenHeight = _window->getWindowHeight();
	glViewport(0, 0, _screenWidth, _screenHeight);

	const glm::vec4& persp = glm::vec4(45, _screenWidth / _screenHeight, 0.1f, 100.f);

	// Changed these coordinates up a bit to match orientations of previous version
	glm::vec3 cameraPos = glm::vec3(-18.0f, 18.0f, 25.0f);
	glm::vec3 cameraFront = glm::vec3(1.0f, -1.0f, -1.5f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	// Initialise objects
	levelMGR->Initialise(persp, view);

}

void Game::release() {
	// Do any last-minute saving etc
	quit();

	// Release data
	delete testMesh;
	delete testTex;
	delete _shaders;
	delete levelMGR;

}
bool Game::update(float dTime) {

	if (_window->getWindowResized())
		initialiseNewRes();

	levelMGR->Update(dTime);

	// Return whether the game should continue
    return _quitGame;
}
void Game::render(float dTime) {
	
	//Initialize clear color
	glClearColor(0.2f, 0.f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	levelMGR->Render(dTime);

	/*SDL_GL_SwapWindow(_window->getWindowOK());*/
	_window->swapWindow();
}
void Game::quit() {
    printf("Game is quitting.\n");
}