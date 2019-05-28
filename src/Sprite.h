#pragma once

#include "BaseObject.h"

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	bool haveImage;
	float width, height;
	ofSoundPlayer sound;
	bool hasSound = false;
	ofColor color = ofColor::gray; // will be used for particle explosion
	int points = 0; // how many points the sprite will give when killed
	bool isPointSprite = false; // determines if the sprite is used for drawing points or not

	Sprite();
	void draw();
	void setLifeSpan(float);
	float age();
	void setImage(ofImage);
	void setSound(string);
	void playSound();
	void drawPoint();
};