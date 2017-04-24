#ifndef TILE_H
#define TILE_H


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
#include <glm\gtc\type_ptr.hpp>

#include <vector>
#include <future>

#include "../Core/Model.h"
#include "../Core/Texture.h"

//wrap up common behaviors, initialization+shutdown
class Tile
{
public:

	enum TileType
	{
		eEmpty, eBasic, eStart, eEnd, ePickup
	};

	//start up and shut down
	Tile(Model* model, const TileType& type, int x, int y, float width, float pad, const glm::vec3& anch, int gridDim, bool isHidden = false, bool isTraverseable = false) 
		:tileType(type), cellX(x), cellY(y), cellWidth(width), padding(pad), anchor(anch), gridDimensions(gridDim), hidden(isHidden), traverseable(isTraverseable), tile(model){}

	~Tile() {
		Release();
	}	
	
	//game logic, called constantly, elapsed time passed in
	virtual void Update(float dTime, float newPadding = -1);

	//render images, called constantly, elapsed time passed in
	virtual void Render(float dTime, float zAdjust = 0, bool setInsteadOfAdd = false);

	virtual void Initialise();// Mesh& tileMesh);
	virtual void Release();

	//game models that reference meshes
	Model* tile;

	// getters & setters
	int getCellX();
	int getCellY();
	int getDim();

	glm::vec3 getAnchor();
	virtual glm::vec3 getPosition();

	glm::vec3 getDisplacement();
	void addDisplacement(const glm::vec3& disp);
	void resetDisplacement();

	TileType getTileType();
	bool getHidden() const;
	virtual int getInfo() const;
	void setHidden(bool b);
	bool getTraverseable() const;
	void setTraverseable(bool b);

	float getPadding();
	float getCellWidth();

	// movement functions
	virtual bool bump();
	virtual void moveOn();
	virtual void moveOff();

	glm::vec3 _pos, _rot, _sca;
	glm::vec4 _col;

private:

	const TileType tileType;
	bool traverseable;
	// attributes for positioning
	const int cellX;
	const int cellY;
	const float cellWidth;
	float padding;
	const glm::vec3 anchor;
	const int gridDimensions;

	// vector for displacing the tile
	glm::vec3 adjustVector;

	// visibility of the tile
	bool hidden;
};

#endif




