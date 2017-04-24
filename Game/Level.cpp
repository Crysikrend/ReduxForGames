#include "Level.h"

void Level::Initialise(const vector<vector<Tile::TileType>>& layout, const vector<vector<int>>& floorLayout, const string& caption, float levelTime, float width, float padding, int dim, float safeTime, float fallSpeedSafe, float fallSpeedDead)
{
	// Pause any animations
	levelFinishedLoading = false;

	// Set level specific params
	tileWidth = width;
	cellDim = dim;
	tilePadding = padding;
	levelCaption = caption;

	// Set values for the 'explosive' transitions
	maxAdditionalPadding = 40;
	levelTransitionSpeed = 26;
	additionalPadding = maxAdditionalPadding;

	// Initialise bools to control the flow of the game
	levelReadyToPlay = false;
	playerInPlay = false;
	endTheGame = false;
	playerBeatLevel = false;
	readyToTransition = false;
	playerFrozen = false;

	// In prep for counting pickups
	pickupNo_ = 0;
	trueCollectedNo_ = 0;

	// Before initialising the next level, release anything stored in tile vectors
	Release();

	// Initialise both foreground and background
	for (int i = 0; i < dim; i++) {
		vector<Tile*> tempLevel;
		vector<TileFloor*> tempBack;

		for (int j = 0; j < dim; j++) {
			// Create and intialise tiles and place it in the appropriate vector
			tempBack.push_back(createFloorTile(layout[i][j], j, i, tileWidth, tilePadding + additionalPadding, anchorPos, safeTime, fallSpeedSafe, fallSpeedDead, floorLayout[i][j]));
			tempLevel.push_back(createTile(layout[i][j],j, i, tileWidth,tilePadding,anchorPos));

			countPickups(layout[i][j]);
		}

		// Add the row of tiles to the level's storage and repeat
		floor.push_back(tempBack);
		level.push_back(tempLevel);
	}

	// Fancy effects
	waveEffect = 0;
	waveLevel = true;

	// Allow the level to animate
	levelFinishedLoading = true;

	timer.pauseTimer();
	levelMaxTime = levelTime;
	timer.setTimer(levelMaxTime);
}

void Level::Release()
{
	for (int i = 0; i < floor.size(); i++)
	{
		for (int j = 0; j < floor.size(); j++)
		{
			// Delete dynamically allocated memory
			delete floor[i][j];
			delete level[i][j];
		}
		// Ensure that the sub-vector is cleared of any pointers
		floor[i].clear();
		level[i].clear();
	}

	// Finally, empty the main vectors
	floor.clear();
	level.clear();
}

void Level::Update(float dTime)
{
	// If the tiles have finished generating
	if (levelFinishedLoading)
	{
		// Updates the timer for later use
		timer.updateTimer(dTime);
		if (timer.getTimer() <= 0)
			endTheGame = true;

		// Transitions in/out depending on game conditions and resets amount collected
		checkGameState(dTime);
		collectedNo_ = 0;

		// Iterate through the 2D array, updating and then checking their info
		for (int i = 0; i < cellDim; i++)
			for (int j = 0; j < cellDim; j++) {
				floor[i][j]->Update(dTime, tilePadding + additionalPadding);  
				level[i][j]->Update(dTime, tilePadding + additionalPadding);

				// These functions check tile-types and acts on info gathered
				checkTileFloor(floor[i][j]);
				checkTile(level[i][j]);
			}

		if (collectedNo_ > trueCollectedNo_)
			trueCollectedNo_ = collectedNo_;
	}
}

void Level::Render(float dTime)
{
	// If the level is set to wave, animate it
	if (waveLevel)
		waveEffect += dTime / 500;
	
	for (int i = 0; i < cellDim; i++)
		for (int j = 0; j < cellDim; j++) {

			if (waveLevel)
				floor[i][j]->Render(dTime, (sinf(waveEffect + (i + j) / (glm::pi<float>() / 2))) / 4); 
			else
				floor[i][j]->Render(dTime);

			// The main level uses the floor's positioning
			level[i][j]->Render(dTime, floor[i][j]->getPosition().z, true);
		}
}

Tile* Level::createTile(const Tile::TileType& type, int x, int y, float width, float pad, const glm::vec3& anch)
{
	// Declare the pointer that will be returned later
	Tile* peter;

	// Depending on the type of tile, create an instance of the appropriate class
	switch (type) {
	case Tile::TileType::eBasic:
		peter = new Tile(modelToUse, type, x, y, width, pad, anch, cellDim);
		break;

	case Tile::TileType::eEmpty:
		peter = new Tile(modelToUse, type, x, y, width, pad, anch, cellDim, true, true);
		break;

	case Tile::TileType::ePickup:
		peter = new TilePickup(modelToUse, type, x, y, width, pad, anch, cellDim, false, true);
		break;

	case Tile::TileType::eStart:
		peter = new TileStart(modelToUse, type, x, y, width, pad, anch, cellDim, false, true);
		break;

	case Tile::TileType::eEnd:
		peter = new TileEnd(modelToUse, type, x, y, width, pad, anch, cellDim, false, true);
		break;

	default:
		peter = new Tile(modelToUse, type, x, y, width, pad, anch, cellDim);
		break;
	}
	
	// Initialise the tile with the mesh in randy reference
	peter->Initialise();
	return peter;
}

TileFloor* Level::createFloorTile(const Tile::TileType& type, int x, int y, float width, float pad, const glm::vec3& anch, float safeTime, float fallSpeedSafe, float fallSpeedDead, int protection)
{
	// Declare the pointer that will be returned later
	TileFloor* peter;

	// Depending on the floor tile specified, create an appropriate floor (For now, just spawn the default floor)
	switch (type) {
	default:
		peter = new TileFloor(modelToUse, type, x, y, width, pad, anch, cellDim, safeTime, fallSpeedSafe, fallSpeedDead, false, true, protection);
		break;
	}

	// Return the initialised tile
	peter->Initialise();
	return peter;
}

// This function receives a position and a direction and returns the location the player should be along with if it was successful
glm::vec3 Level::move(const glm::vec3& pos, const glm::vec2& dir, bool& success)
{
	// If the player is able to move
	if (levelReadyToPlay && !playerFrozen && !endTheGame)
	{
		// Get the cell coordinates of the player's current and next position
		const glm::vec2 pPos = getCellFromCoords(pos);
		const glm::vec2 newPos = pPos + dir;

		success = (newPos.x >= 0 && newPos.x < cellDim && newPos.y >= 0 && newPos.y < cellDim);
		if (success)
		{
			// 'Bump' lets the new tile know that the player wishes to move there, while returning if it's possible
			success = level[newPos.x][newPos.y]->bump() && floor[newPos.x][newPos.y]->bump();

			if (success) {
				// Move off of the previous tile and floor
				floor[pPos.x][pPos.y]->moveOff();
				level[pPos.x][pPos.y]->moveOff();

				// Call any important functions for moving onto a new tile
				floor[newPos.x][newPos.y]->moveOn();
				level[newPos.x][newPos.y]->moveOn();


				return getCoordsFromCell(newPos, pos);
			}
			else
				return getCurrentLocationOfTile(pos);
		}
		else
			return getCurrentLocationOfTile(pos);
	}
	else
		return getCurrentLocationOfTile(pos);
}

glm::vec3 Level::getStartingPosition() const
{
	for (int i = 0; i < cellDim; i++)
	{
		for (int j = 0; j < cellDim; j++)
			if (level[i][j]->getTileType() == Tile::eStart)
				return getCoordsFromCell(glm::vec2(i, j));
	}

	// If this function gets this far, there is no starting tile and MUST be fixed
	assert(false);
}

glm::vec3 Level::getEndingPosition() const
{
	for (int i = 0; i < cellDim; i++)
	{
		for (int j = 0; j < cellDim; j++)
			if (level[i][j]->getTileType() == Tile::eEnd)
				return getCoordsFromCell(glm::vec2(i, j));
	}

	// If this function gets this far, there is no ending tile and MUST be fixed
	assert(false);
}

// This function takes a position and returns where the player would be if he's on the same tile
// This is necessary due to the amount of movement the tiles can do, and thus makes positioning 
// the player easier.
glm::vec3 Level::getCurrentLocationOfTile(const glm::vec3 & pos)
{
	glm::vec3 pPos;

	if (!endTheGame && playerInPlay) {
		const glm::vec3 temp = { std::floor(pos.x * 100) / 100, std::floor(pos.y * 100) / 100, pos.z };
		const glm::vec2 v2 = getCellFromCoords(temp);
		pPos = getCoordsFromCell(v2);
	}
	else if (endTheGame)
		pPos = getEndingPosition();
	else 
		pPos = getStartingPosition();

	return pPos;
}

float Level::getTimer() const
{
	return timer.getTimer();
}

glm::vec2 Level::getCellFromCoords(const glm::vec3& pos) const
{
	return glm::vec2(std::ceil((pos.y - anchorPos.x) / (tileWidth + tilePadding + additionalPadding)) + cellDim / 2, std::ceil((pos.x - anchorPos.y) / (tileWidth + tilePadding + additionalPadding)) + cellDim / 2);
}

glm::vec3 Level::getCoordsFromCell(const glm::vec2& cell, const glm::vec3& prevPos) const
{
	return glm::vec3((cell.y - cellDim / 2) * (tileWidth + tilePadding + additionalPadding) + anchorPos.x, (cell.x - cellDim / 2) * (tileWidth + tilePadding + additionalPadding) + anchorPos.y, floor[cell.x][cell.y]->getPosition().z);
}

int Level::getPickupNo() const
{
	return pickupNo_;
}

int Level::getCollectedNo() const
{
	return trueCollectedNo_;
}

void Level::countPickups(const Tile::TileType& layout){
	if (layout == Tile::ePickup){
		pickupNo_++;
	}
}

void Level::checkTile(Tile* tile)
{
	// BLOCKS:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Count how many pickups have been collected
	if (tile->getTileType() == Tile::ePickup) {

		if (tile->getInfo())
			collectedNo_++;
	}
	// Check if the player has begun the level
	else if (tile->getTileType() == Tile::eStart) {

		if (!playerInPlay && tile->getInfo()) {
			timer.startTimer();
			playerInPlay = true;
		}
	}
	// Check if the player has reached the end
	else if (tile->getTileType() == Tile::eEnd) {

		if (tile->getInfo() && (trueCollectedNo_ >= pickupNo_)) {
			timer.pauseTimer();
			playerBeatLevel = true;
			endTheGame = true;
		}
	}
}

void Level::checkTileFloor(TileFloor* tile)
{
	// FLOORS:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Check if the floor tiles are killing the player
	const int status = tile->getInfo();
	if (status > 0)
	{
		timer.pauseTimer();

		if (status == 1)
			playerFrozen = true;
		else
			endTheGame = true;
	}
}

void Level::checkGameState(float dTime)
{
	// Depending on if the game is over or starting, explode (or implode) the level
	if (endTheGame) {
		levelReadyToPlay = false;

		if (additionalPadding >= maxAdditionalPadding) {
			additionalPadding = maxAdditionalPadding;
			readyToTransition = true;
			endTheGame = false;
		}

		additionalPadding += dTime * levelTransitionSpeed;
	}
	else if (!levelReadyToPlay)
	{
		if (additionalPadding <= 0) {
			additionalPadding = 0;
			levelReadyToPlay = true;
		}

		additionalPadding -= dTime * levelTransitionSpeed;
	}
}

bool Level::getLevelStarted() const
{
	return playerInPlay;
}

bool Level::getLevelEnded() const
{
	return endTheGame;
}

bool Level::getLevelSwitch() const
{
	return readyToTransition;
}

bool Level::getWinStatus() const
{
	return playerBeatLevel;
}

string Level::getMessage() const
{
	if (!endTheGame)
		return levelCaption;

	else if (endTheGame && playerBeatLevel) {
		const float time = levelMaxTime - timer.getTimer();
		const string temp = std::to_string(time);
		return "Congratulations! You beat the level in " + temp + " seconds!";
	}
	else
		return "Try again!";
}

