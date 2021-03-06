#ifndef TILESTART_H
#define TILESTART_H

#include "Tile.h"
class TileStart : public Tile
{
public:
	TileStart(Model* model, const TileType& type, int x, int y, float width, float pad, const glm::vec3& anch, int gridDim, bool isHidden = false, bool isTraverseable = false)
		:Tile(model, type, x, y, width, pad, anch, gridDim, isHidden, isTraverseable) {};

	void Initialise();// Mesh& tileMesh) override;
	void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false) override;

	// movement functions
	void moveOff() override;

	int getInfo() const override;

private:
	bool safe;

};
#endif
