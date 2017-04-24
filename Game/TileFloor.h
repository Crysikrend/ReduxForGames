#ifndef TILEFLOOR_H
#define TILEFLOOR_H

#include "Tile.h"
#include "Timer.h"


//wrap up common behaviors, initialization+shutdown
class TileFloor : public Tile
{
public:
	//start up and shut down
	TileFloor(Model* model, const TileType& type, int x, int y, float width, float pad, const glm::vec3& anch, int gridDim, float safeTime, float safeFallSpeed, float pingFallSpeed, bool isHidden = false, bool isTraverseable = false, int prot = 0)
		:Tile(model, type, x, y, width, pad, anch, gridDim, isHidden, isTraverseable), unstableTime(safeTime), unstableSpeed(safeFallSpeed), pingSpeed(pingFallSpeed), protection(prot) {}

	void Initialise();// Mesh& tileMesh) override;
	void Update(float dTime, float newPadding = -1) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false) override;

	glm::vec3 getPosition() override;

	void moveOn() override;
	void moveOff() override;

	int getInfo() const override;

	

private:
	enum DestroyStatus
	{
		stable, unstable, degrading, destroyed
	} destroyStatus;

	// Speeds and attributes
	Timer destroyTimer;
	const float unstableTime;
	const float unstableSpeed;
	const float pingSpeed;

	void stepOnTile();
	void stepOffTile();
	void killTile();
	void removeTile();

	void recolour();


	bool playerIsOn;	// Is the player on the tile?
	bool floorIsDead;	// Is the floor deadly?


	int protection;

};

#endif




