#include "Player.h"

Player::Player(Model* model, LevelMGR* levelPointer) 
	:player(model), levelManager(levelPointer){
}

void Player::Initialise(){

	adjustVector = levelManager->getStartingPosition();
	buttonHold = false;
	moveWait = 0;
	playerOne = true;

}
void Player::Release(){

}
void Player::Update(float dTime, const SDL_Event& e){
	bool success = false;

	move(e);

	if ((!buttonHold || (buttonHold && moveWait <= 0)) && (moveDirection.x != 0 || moveDirection.y != 0)){
		adjustVector = levelManager->move(adjustVector, moveDirection, success);
		moveWait = 250;
	}
	else // If the player is not moving, retrieve the current tile's coords
		adjustVector = levelManager->getCurrentLocationOfTile(adjustVector);

	if (moveDirection != glm::vec2(0, 0))
		buttonHold = true;
	else
		buttonHold = false;

	moveWait -= dTime;

	moveDirection = glm::vec2(0, 0);
}
void Player::Render(float dTime){
	adjustVector.z += 2;
	player->_pos = adjustVector;
	player->_sca = glm::vec3(0.5, 0.5, 0.5);
	player->_col = glm::vec4(0, 1, 1, 1);

	// This is so that the player sits on top of his tile. You need to add the size of the player on the end so that he's not inside it
	player->render();
}
bool Player::getMoveRequest(){
	bool state = moveRequest;
	moveRequest = false;
	return state;
}
glm::vec3 Player::getPlayerPostion(){
	return adjustVector;
}
glm::vec2 Player::getMoveDirection(){
	return moveDirection;
}
void Player::makePlayerOne(){
	playerOne = true;
}
void Player::makePlayerTwo(){
	playerOne = false;
}
void Player::move(const SDL_Event& e){
	if (e.type == SDL_KEYDOWN && playerOne){

		switch (e.key.keysym.sym) {
		case SDLK_w:
		case SDLK_UP:
			moveDirection = glm::vec2(0, 1);
			break;
		case SDLK_a:
		case SDLK_LEFT:
			moveDirection = glm::vec2(-1, 0);
			break;
		case SDLK_s:
		case SDLK_DOWN:
			moveDirection = glm::vec2(0, -1);
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			moveDirection = glm::vec2(1, 0);
			break;
		}
	}
}