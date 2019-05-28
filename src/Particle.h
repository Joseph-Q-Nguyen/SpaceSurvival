#pragma once

#include "ofMain.h"
#include "Sprite.h"

class ParticleForceField;

class Particle {
public:
	Particle();

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;
	void    integrate();
	void    draw();
	float   age();        // sec
	ofColor color;
	ofImage image; // draw Sprite in place of particle
	bool hasSprite = false;
	bool hasForce = false; // check if force has been applied
};


