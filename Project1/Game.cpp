#include "Game.h"
Game::Game() {
	position[0] = 0;
	position[1] = 0;
}

Game::~Game() {
	delete[]position;
}

float* Game::getPosition() {
	return position;
}
