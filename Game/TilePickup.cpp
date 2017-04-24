#include "TilePickup.h"

void TilePickup::Initialise()//Mesh & tileMesh)
{
	Tile::Initialise();//tileMesh);

	spinAngle = 0;
	isCollected = false;
}

void TilePickup::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	spinAngle += dTime / 800;

	if (!getHidden())
	{
		//mFX->Render(tile, gd3dImmediateContext);

		//tile.GetPosition().x = anchor.x + (cellX - gridDimensions / 2) * (cellWidth + padding);
		_pos.x = getAnchor().x + (getCellX() - getDim() / 2) * (getCellWidth() + getPadding());
		_pos.y = getAnchor().y + (getCellY() - getDim() / 2) * (getCellWidth() + getPadding());

		if (!setInsteadOfAdd)
			_pos.z = getAnchor().z + getCellWidth() + getPadding() + zAdjust;
		else
			_pos.z = zAdjust + 1.3 * getPadding();

		_pos += getDisplacement();

		_rot = glm::vec3(glm::pi<float>() / 4, glm::pi<float>() / 4, spinAngle);
		_sca = glm::vec3(0.5, 0.5, 0.5);

		tile->_pos = _pos;
		tile->_rot = _rot;
		tile->_sca = _sca;
		tile->_col = _col;
		tile->render();
	}

}

bool TilePickup::bump()
{
	return Tile::bump();
}

void TilePickup::moveOn()
{
	if (!isCollected && !getHidden())
		collect();
}

void TilePickup::moveOff()
{
}

int TilePickup::getInfo() const
{
	return getCollected();
}

void TilePickup::collect()
{
	isCollected = true;
	setHidden(true);
}

bool TilePickup::getCollected() const
{
	return isCollected;
}

