#ifndef PLAYER_H
#define PLAYER_H

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


#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "LevelMGR.h"

class Player{
public:
	Player(Model*, LevelMGR*);

	void Initialise();
	void Release();
	void Update(float, const SDL_Event&);
	void Render(float);
	void makePlayerOne();
	void makePlayerTwo();
	bool getMoveRequest();
	glm::vec3 getPlayerPostion();
	glm::vec2 getMoveDirection();

	//game models that reference meshes
	Model* player;

private:
	int cellX;
	int cellY;
	int moveWait;
	bool buttonHold;
	bool moveRequest;
	bool playerOne;
	void move(const SDL_Event&);
	glm::vec3 adjustVector;
	glm::vec2 moveDirection;
	LevelMGR* levelManager;
};

#endif