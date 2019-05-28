#pragma once

#include "ofMain.h"
#include "SpriteSystem.h"

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public Sprite {
public:
	SpriteSystem *sys;
	float rate; // per sec
	ofVec3f velocity;
	float lifespan; // millisec
	bool started;
	float lastSpawned;

	ofImage childImage;
	ofImage image;
	float childWidth;
	float childHeight;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	bool isEmitting;
	string emitSoundPath;
	bool hasEmitSound;
	ofSoundPlayer emitSound;
	ofColor childColor = ofColor::blue; // child color will determine its explosion color
	int childPoints = 0; //points of the sprites
	bool oneShot = false;

	Emitter(SpriteSystem *);
	void draw();
	void start();
	void stop();
	void setLifespan(float);
	void setVelocity(ofVec3f);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();
};