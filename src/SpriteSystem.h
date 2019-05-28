#pragma once

#include "ofMain.h"
#include "Sprite.h"

//  Manages all Sprites in a system.  You can create multiple systems
//

class SpriteSystem {
public:
	vector<Sprite> sprites;

	void add(Sprite);
	void remove(int);
	void update();
	void draw();
};