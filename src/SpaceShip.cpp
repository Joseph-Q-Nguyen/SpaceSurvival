#include "SpaceShip.h"


SpaceShip::SpaceShip(float width, float height) { // create a spaceship with width and height (length)
	this->width = width;
	this->height = height;
	this->haveImage = false;
	if (this->image.load("images//myspaceship2.png")) {
		this->haveImage = true;
	}
}

//Translate ship with velocity
void SpaceShip::update() {
	position += velocity;
}