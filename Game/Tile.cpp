#include "Tile.h"

void Tile::Initialise()//Mesh& tileMesh)
{
	//tile.Initialise(tileMesh);

	// Remove any displacement to reset the location
	adjustVector = { 0, 0, 0 };
}

void Tile::Release()
{
	// Release the used data (somehow)
}

void Tile::Update(float dTime, float newPadding)
{
	if (newPadding > 0)
		padding = newPadding;
}


void Tile::Render(float dTime, float zAdjust, bool setInsteadOfAdd)
{
	if (!getHidden())
	{
		//mFX->Render(tile, gd3dImmediateContext);

		//tile.GetPosition().x = anchor.x + (cellX - gridDimensions / 2) * (cellWidth + padding);
		_pos.x = anchor.x + (cellX - gridDimensions / 2) * (cellWidth + padding);
		_pos.y = anchor.y + (cellY - gridDimensions / 2) * (cellWidth + padding);

		if (!setInsteadOfAdd)
			_pos.z = anchor.z + cellWidth + padding + zAdjust;
		else
			_pos.z = zAdjust + 1.3 * padding;

		_pos += adjustVector;

		_sca = glm::vec3(cellWidth, cellWidth, cellWidth);

		tile->_pos = _pos;
		tile->_rot = _rot;
		tile->_sca = _sca;
		tile->_col = _col;
		tile->render();
	}
}

///////////////////////////////////////////////////////
// Getters
///////////////////////////////////////////////////////

glm::vec3 Tile::getPosition() {
	return _pos;
}

int Tile::getCellX() {
	return cellX;
}

int Tile::getCellY() {
	return cellY;
}

int Tile::getDim()
{
	return gridDimensions;
}

float Tile::getPadding() {
	return padding;
}
float Tile::getCellWidth() {
	return cellWidth;
}

bool Tile::bump()
{
	return getTraverseable();
}

void Tile::moveOn()
{
}

void Tile::moveOff()
{
}

glm::vec3 Tile::getAnchor() {
	return anchor;
}

glm::vec3 Tile::getDisplacement() {
	return adjustVector;
}

bool Tile::getHidden() const{
	return hidden;
}

void Tile::setHidden(bool b)
{
	hidden = b;
}

bool Tile::getTraverseable() const
{
	return traverseable;
}

void Tile::setTraverseable(bool b)
{
	traverseable = b;
}

Tile::TileType Tile::getTileType() {
	return tileType;
}

///////////////////////////////////////////////////////
// Setters
///////////////////////////////////////////////////////

void Tile::addDisplacement(const glm::vec3& disp) {
	adjustVector += disp;
}

void Tile::resetDisplacement() {
	adjustVector = { 0, 0, 0 };
}

int Tile::getInfo() const
{
	return 0;
}