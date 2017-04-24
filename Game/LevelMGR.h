#ifndef LEVELMGR_H
#define LEVELMGR_H

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

#include "Level.h"
#include "Tile.h"

//wrap up common behaviors, initialization+shutdown
class LevelMGR
{
public:
	//start up and shut down
	LevelMGR(glm::vec3 anch, ShaderManager* shaders);
	~LevelMGR() {
		Release();
	}
	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	void Render(float dTime);

	void Initialise(const glm::vec4& persp, const glm::mat4& viewPos = glm::mat4());
	void Release();

	void changeLevel(int levelNo);

	//player interaction
	glm::vec3 move(const glm::vec3& pos, const glm::vec2& dir, bool& success);
	glm::vec3 getStartingPosition() const;
	glm::vec3 getCurrentLocationOfTile(const glm::vec3& pos);

	//Number of pickups on current level
	int getPickupNo() const;
	int getCollectedNo() const;
	float getTimer() const;
	string getMessage() const;

	// Get whether the player has started
	bool getLevelStarted() const;

private:
	Mesh* mesh;
	Texture* texture;
	Model* modelToUse;

	struct levelTemplate
	{
		float levelTime;		// The time the player has to complete the level
		string levelCaption;	// The hint of the level displayed on the UI

		float safeTime;			// The time the player can spend on a tile before they die
		float fallSpeedSafe;	// The speed of the falling tile before the player jumps off
		float fallSpeedDead;	// The speed of the tile after the player has left

		float tileWidth;		// The size of the tiles
		int tileDim;			// The size of the grid
		float tilePadding;		// The size of the padding between the tiles

		vector<vector<Tile::TileType>> tileList;
		vector<vector<int>> floorList;
	};

	int currentLevel;

	levelTemplate getLayout(int LevelNo);

	Level* level;

	int pickupNo_;
};
#endif