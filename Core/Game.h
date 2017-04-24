#ifndef GAMEH
#define GAMEH

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
#include <fstream>
#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "Window.h"
#include "ShaderManager.h"
#include "Model.h"
#include "../Game/LevelMGR.h"

class Game {
public:
	// Constructors
	Game(Window*);

	// Major functions
	void initialise();
	void initialiseNewRes();
	void release();

	bool update(float dTime);
	void render(float dTime);



private:
	Window* _window;
	float _screenWidth, _screenHeight;
	bool _quitGame;

	GLuint _vao, _vbo, _ebo;
	float tempAngle = 0;

	Mesh* testMesh;
	Texture* testTex;
	ShaderManager* _shaders;

	LevelMGR* levelMGR;

	void quit();
};

#endif
