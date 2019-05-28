#pragma once

#include "ofMain.h"
#include "Sprite.h"

class SpaceShip : public Sprite {
public:
	SpaceShip(float, float);
	void update();
};