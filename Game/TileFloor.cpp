#include "TileFloor.h"

void TileFloor::Initialise()//Mesh& tileMesh)
{
	Tile::Initialise();// tileMesh);

	playerIsOn = false;
	floorIsDead = false;
	destroyStatus = stable;

	switch (protection) {
	case -1:
		// Invincible tile
		break;
	case -2:
		// Destroyed tile
		protection = 0;
		floorIsDead = true;
		removeTile();
		break;
	}

	recolour();
}

void TileFloor::Update(float dTime, float newPadding) {

	Tile::Update(dTime, newPadding);

	destroyTimer.updateTimer(dTime);

	switch (destroyStatus) {
		case stable:
			// A happy floor tile
			break;
		case unstable:
			// When the player is standing on the tile, soon to be destroyed if they don't jump off quick
			addDisplacement(glm::vec3(0, 0, -unstableSpeed * dTime));
			if (destroyTimer.getTimer() <= 0)
				killTile();
			break;
		case degrading:
			// When the floor is actually being destroyed, make it sink
			addDisplacement(glm::vec3(0, 0, -pingSpeed * dTime));

			if (destroyTimer.getTimer() <= 0)
				destroyStatus = destroyed;
			break;
		case destroyed:
			// When the floor has disappeared, flick a few bools and make it act like normal
			// If Destroyed has been triggered twice, the player caused it and will flag a death
			addDisplacement(glm::vec3(0, 0, -pingSpeed * dTime));
			if (!floorIsDead) {
				removeTile();
			}
			break;
	}
}

void TileFloor::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	if (!getHidden())
	{
		//mFX->Render(tile, gd3dImmediateContext);

		_pos.x = getAnchor().x + (getCellX() - getDim() / 2) * (getCellWidth() + getPadding());
		_pos.y = getAnchor().y + (getCellY() - getDim() / 2) * (getCellWidth() + getPadding());

		_sca = glm::vec3(getCellWidth(), getCellWidth(), 0.2);

		if (!setInsteadOfAdd)
			_pos.z = getAnchor().z + getCellWidth() - (getCellWidth() * _sca.z) + zAdjust + getPadding();
		else
			_pos.z = zAdjust;

		_pos += getDisplacement();

		tile->_pos = _pos;
		tile->_rot = _rot;
		tile->_sca = _sca;
		tile->_col = _col;

		tile->render();
	}
}

glm::vec3 TileFloor::getPosition()
{
	glm::vec3 temp = Tile::getPosition();

	if (floorIsDead)
		temp.z = getDisplacement().z + getPadding() + getCellWidth() - (getCellWidth() * _sca.z);

	return temp;
}

void TileFloor::moveOn()
{
	stepOnTile();
}

void TileFloor::moveOff()
{
	stepOffTile();
	recolour();
}

int TileFloor::getInfo() const
{
	if (!playerIsOn || destroyStatus == unstable)
		return 0;		
	else // playerIsOn BUT it's not unstable (therefore DEGRADING OR DESTROYED)
	{
		if (destroyStatus == degrading)
			return 1;
		else 
			return 2;
	}
}

///////////////////////////////////////////////////////
// TileFloor Functions
///////////////////////////////////////////////////////

void TileFloor::stepOnTile()
{
	playerIsOn = true;

	if (!floorIsDead) {
		destroyStatus = unstable;

		destroyTimer.setTimer(unstableTime);
		destroyTimer.startTimer();
	}
	else {
		removeTile();
		killTile();
	}
}

void TileFloor::stepOffTile()
{
	destroyTimer.resetTimer();

	playerIsOn = false;
	if (protection != 0)
	{
		if (protection > 0)
			protection -= 1;
		resetDisplacement();
		destroyStatus = stable;
	}

	else if (protection == 0)
		killTile();
}

void TileFloor::killTile() {

	// Restart the timer and prepare to countdown
	destroyTimer.resetTimer();
	// Set the status to the melting/falling animation (which is timed)
	destroyStatus = degrading;
	floorIsDead = true;

	destroyTimer.setTimer(1); // 1 Second Until Death
	destroyTimer.startTimer();

}

void TileFloor::removeTile()
{
	setHidden(true);
	resetDisplacement();
	destroyStatus = stable;
}

void TileFloor::recolour() {
	switch (protection) {
	case 0:
		_col = glm::vec4(0, 0, 0, 1);
		break;
	case 1:
		_col = glm::vec4(1, 1, 0, 1);
		break;
	case -1:
		// Invincible tile
		_col = glm::vec4(1, 1, 1, 1);
		break;
	case -2:
		// Destroyed tile
		break;
	}

	switch (getTileType()) {
	case Tile::eStart:
		_col = glm::vec4(0, 1, 0, 1);
		break;
	case Tile::eEnd:
		_col = glm::vec4(1, 0, 1, 1);
		break;
	}
}
