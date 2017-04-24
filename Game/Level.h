#ifndef LEVEL_H
#define LEVEL_H

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

#include "Tile.h"
#include "TileFloor.h"
#include "TilePickup.h"
#include "TileStart.h"
#include "TileEnd.h"


//wrap up common behaviors, initialization+shutdown
class Level
{
public:
	//start up and shut down
	Level(const glm::vec3& anch, Model* pModelToUse)
		: anchorPos(anch), modelToUse(pModelToUse)
	{}

	~Level() {
		Release();
	}

	struct TextureInfo
	{

	} textureInfo;

	//game logic, called constantly, elapsed time passed in
	void Update(float dTime);

	//render images, called constantly, elapsed time passed in
	void Render(float dTime);

	// Called everytime a new level is created
	void Initialise(const vector<vector<Tile::TileType>>& layout, const vector<vector<int>>& floorLayout, const string& caption, float levelTime, float width = 1, float padding = 1.25, int dim = 10, float safeTime = 5, float fallSpeedSafe = 3, float fallSpeedDead = 10);

	// player interaction
	glm::vec3 move(const glm::vec3& pos, const glm::vec2& dir, bool& success);
	glm::vec3 getStartingPosition() const;
	glm::vec3 getEndingPosition() const;
	glm::vec3 getCurrentLocationOfTile(const glm::vec3& pos);

	float getTimer() const;

	void Release();

	//Number of pickups on current level
	int getPickupNo() const;
	int getCollectedNo() const;

	// Gets whether the level has started/ended
	bool getLevelStarted() const;
	bool getLevelEnded() const;
	bool getLevelSwitch() const;
	bool getWinStatus() const;

	// Get any messages to print to the UI
	string getMessage() const;

private:

	Model* modelToUse;

	int pickupNo_;
	int collectedNo_;
	int trueCollectedNo_; // This is necessary so that, when checked, the collectedNo isn't in the process of counting
	const glm::vec3 anchorPos;
	float tileWidth;
	string levelCaption;

	float levelMaxTime;
	Timer timer;

	bool playerBeatLevel;		// When the level transitions, this is used to check whether to restart or proceed

	bool levelFinishedLoading;  // For pausing any updates called in the Init function
	bool levelReadyToPlay;		// When the level has finished transitioning
	bool playerInPlay;			// The player has moved away from the start and is playing
	bool endTheGame;			// The player has touched the end tile of the game or died
	bool readyToTransition;		// The level has 'exploded' and is ready to change level

	bool playerFrozen;			// Whether the player can move

	int cellDim;
	float tilePadding;
	float additionalPadding;
	float maxAdditionalPadding;
	float levelTransitionSpeed;

	float waveEffect;
	bool waveLevel;

	// Arrays of tile pointers
	vector<vector<TileFloor*>> floor;
	vector<vector<Tile*>> level;

	// Tile creation
	Tile* createTile(const Tile::TileType& type, int x, int y, float width, float pad, const glm::vec3& anch);
	TileFloor* createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const glm::vec3& anch, float safeTime, float fallSpeedSafe, float fallSpeedDead, int protection = 0);

	// Locating functions
	glm::vec2 getCellFromCoords(const glm::vec3& pos) const;
	glm::vec3 getCoordsFromCell(const glm::vec2& cell, const glm::vec3& prevPos = glm::vec3(0, 0, 0)) const;

	//Count number of pickups on current level
	void countPickups(const Tile::TileType& layout);

	// Stat updating
	void checkTile(Tile* tile);
	void checkTileFloor(TileFloor* tile);

	// Controlling the flow of play
	void checkGameState(float dTime);		
};

#endif




