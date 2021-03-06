#include "LevelMGR.h"

LevelMGR::LevelMGR(glm::vec3 anch, ShaderManager* shaders) {

	//CUBE
	std::vector<GLfloat> vertices = {
		-1.f, -1.f, -1.f,  0.0f, 0.0f,
		1.f, -1.f, -1.f,  1.0f, 0.0f,
		1.f,  1.f, -1.f,  1.0f, 1.0f,
		1.f,  1.f, -1.f,  1.0f, 1.0f,
		-1.f,  1.f, -1.f,  0.0f, 1.0f,
		-1.f, -1.f, -1.f,  0.0f, 0.0f,

		-1.f, -1.f,  1.f,  0.0f, 0.0f,
		1.f, -1.f,  1.f,  1.0f, 0.0f,
		1.f,  1.f,  1.f,  1.0f, 1.0f,
		1.f,  1.f,  1.f,  1.0f, 1.0f,
		-1.f,  1.f,  1.f,  0.0f, 1.0f,
		-1.f, -1.f,  1.f,  0.0f, 0.0f,

		-1.f,  1.f,  1.f,  1.0f, 0.0f,
		-1.f,  1.f, -1.f,  1.0f, 1.0f,
		-1.f, -1.f, -1.f,  0.0f, 1.0f,
		-1.f, -1.f, -1.f,  0.0f, 1.0f,
		-1.f, -1.f,  1.f,  0.0f, 0.0f,
		-1.f,  1.f,  1.f,  1.0f, 0.0f,

		1.f,  1.f,  1.f,  1.0f, 0.0f,
		1.f,  1.f, -1.f,  1.0f, 1.0f,
		1.f, -1.f, -1.f,  0.0f, 1.0f,
		1.f, -1.f, -1.f,  0.0f, 1.0f,
		1.f, -1.f,  1.f,  0.0f, 0.0f,
		1.f,  1.f,  1.f,  1.0f, 0.0f,

		-1.f, -1.f, -1.f,  0.0f, 1.0f,
		1.f, -1.f, -1.f,  1.0f, 1.0f,
		1.f, -1.f,  1.f,  1.0f, 0.0f,
		1.f, -1.f,  1.f,  1.0f, 0.0f,
		-1.f, -1.f,  1.f,  0.0f, 0.0f,
		-1.f, -1.f, -1.f,  0.0f, 1.0f,

		-1.f,  1.f, -1.f,  0.0f, 1.0f,
		1.f,  1.f, -1.f,  1.0f, 1.0f,
		1.f,  1.f,  1.f,  1.0f, 0.0f,
		1.f,  1.f,  1.f,  1.0f, 0.0f,
		-1.f,  1.f,  1.f,  0.0f, 0.0f,
		-1.f,  1.f, -1.f,  0.0f, 1.0f
	};

	mesh = new Mesh("cube", vertices, 5, 3, true);
	texture = new Texture("Data/block.jpg", 512, 512);
	modelToUse = new Model(mesh, shaders, "simple", texture);

	level = new Level(anch, modelToUse);

	currentLevel = 0;
	changeLevel(currentLevel);
}

void LevelMGR::Initialise(const glm::vec4& persp, const glm::mat4& viewPos)
{
	//BuildCube(*mMeshMgr);
	modelToUse->initialise(persp, viewPos);

	/*if (!level->getLevelStarted() && !level->getLevelEnded())
		changeLevel(currentLevel);*/
}

void LevelMGR::Release()
{
	level->Release();
	delete level;

	delete modelToUse;
	delete texture;
	delete mesh;
}

void LevelMGR::Update(float dTime)
{
	level->Update(dTime);
	if (level->getLevelSwitch())
	{
		if (level->getWinStatus())
			currentLevel += 1;

		if (currentLevel > 3)
			currentLevel = 0;

		changeLevel(currentLevel);
	}
}


void LevelMGR::Render(float dTime)
{
	level->Render(dTime);
}

void LevelMGR::changeLevel(int levelNo) {
	levelTemplate nextLevel = getLayout(levelNo);
	level->Initialise(nextLevel.tileList, nextLevel.floorList, nextLevel.levelCaption, nextLevel.levelTime, nextLevel.tileWidth, nextLevel.tilePadding, nextLevel.tileDim, nextLevel.safeTime, nextLevel.fallSpeedSafe, nextLevel.fallSpeedDead);
}

// Complex one. Pass in your coordinate position and your direction (0/1 x, 0/1 y) and a bool.
// The function will return the location the player should go to at the end of the function.
// The bool will change depending on a whether a move was made.
glm::vec3 LevelMGR::move(const glm::vec3& pos, const glm::vec2& dir, bool& success)
{
	return level->move(pos, dir, success);
}

glm::vec3 LevelMGR::getStartingPosition() const
{
	return level->getStartingPosition();
}

glm::vec3 LevelMGR::getCurrentLocationOfTile(const glm::vec3 & pos)
{
	return level->getCurrentLocationOfTile(pos);
}

LevelMGR::levelTemplate LevelMGR::getLayout(int layoutNo) {

	levelTemplate nextLevel;

	switch (layoutNo)
	{
		case -1: // JUST A TEST
		{
			nextLevel.levelTime = 30;		// Time the player has to complete the level
			nextLevel.levelCaption = "Get to the end with both pickups!";

			nextLevel.safeTime = 3;			// Players have 3 seconds before death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 10;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1;		// Width of tles
			nextLevel.tileDim = 10;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.25;	// Padding between blocks

			nextLevel.tileList = {

				{Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::ePickup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eStart, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::ePickup, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic},
				{Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEnd, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic}

			};

			nextLevel.floorList = {

				{ -2, -1, -1, 0, 0, 0, 0, 0, 0, 0 },
				{ -1, -1, -1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

			};

			break;
		}
		case 0:
		{
			nextLevel.levelTime = 3;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 1: Reach the end";

			nextLevel.safeTime = 3;			// Players have 3 seconds before death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 10;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 3;		// Width of tles
			nextLevel.tileDim = 4;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 3.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty},
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty},
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEnd, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{-2,-2,-2,-2},
				{-1,-1,-1,-2},
				{-2,-2,-1,-2},
				{-2,-2,-1,-2}
			};
			break;
		}
		case 1:
		{
			nextLevel.levelTime = 4;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 2: Watch your step";

			nextLevel.safeTime = 3;			// Players have 3 seconds before death
			nextLevel.fallSpeedSafe = 3;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 10;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 2;		// Width of tles
			nextLevel.tileDim = 6;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eStart, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEnd, Tile::eBasic, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty }
			};

			nextLevel.floorList = {
				{ -2,-2,-2,-2,-2, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2, 0, 0, 0, 0, -2 },
				{ -2,-2,-2,-2,-2, -2 },
			};

			break;
		}
		case 2:
		{
			nextLevel.levelTime = 5;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 3: Collect the pickup before finishing";

			nextLevel.safeTime = 2;			// Players have 3 seconds before death
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 2;		// Width of tles
			nextLevel.tileDim = 5;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 2.25;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eEmpty, Tile::eEmpty, Tile::ePickup, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eStart, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty },
				{ Tile::eEmpty, Tile::eEmpty, Tile::eEnd, Tile::eEmpty, Tile::eEmpty }

			};

			nextLevel.floorList = {
				{ -2, 0, 0,-2,-2 },
				{ -2, 0, 0, 0,-2 },
				{  0, 0, 0, 0,-2 },
				{ -2,-2, 0, 0,-2 },
				{ -2,-2, 0,-2,-2 },
			};

			break;
		}
		case 3:
		{
			nextLevel.levelTime = 10;		// Time the player has to complete the level
			nextLevel.levelCaption = "Lv 4: Plan your moves";

			nextLevel.safeTime = 2;			// Players have 3 seconds before death
			nextLevel.fallSpeedSafe = 5;	// Speed at which the tiles fall when safe
			nextLevel.fallSpeedDead = 12;	// Speed at which the tiles fall when not safe

			nextLevel.tileWidth = 1;		// Width of tles
			nextLevel.tileDim = 8;			// Amount of tiles (n*n)
			nextLevel.tilePadding = 1.45;	// Padding between blocks

			nextLevel.tileList = {
				{ Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic },
				{ Tile::eBasic, Tile::ePickup, Tile::eEmpty, Tile::ePickup, Tile::eBasic, Tile::ePickup, Tile::ePickup, Tile::eEnd },
				{ Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
				{ Tile::eStart, Tile::ePickup, Tile::eEmpty, Tile::ePickup, Tile::eBasic, Tile::eEmpty, Tile::ePickup, Tile::eBasic },
				{ Tile::eBasic, Tile::eEmpty, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::ePickup, Tile::eEmpty, Tile::eBasic },
				{ Tile::eBasic, Tile::eEmpty, Tile::ePickup, Tile::ePickup, Tile::eEmpty, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
				{ Tile::eBasic, Tile::eEmpty, Tile::ePickup, Tile::ePickup, Tile::eBasic, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },
				{ Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::eBasic, Tile::ePickup, Tile::eEmpty, Tile::eEmpty, Tile::eBasic },

			};

			nextLevel.floorList = {
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 1, 0, 0 }
			};

			break;
		}
	}
	return nextLevel;
}

int LevelMGR::getPickupNo() const
{
	return level->getPickupNo();
}

int LevelMGR::getCollectedNo() const
{
	return level->getCollectedNo();
}

float LevelMGR::getTimer() const
{
	return level->getTimer();
}

string LevelMGR::getMessage() const
{
	return level->getMessage();
}

bool LevelMGR::getLevelStarted() const
{
	return level->getLevelStarted();
}
