#include "Sprite.h"

//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 50;
	height = 50;
}

void Sprite::setLifeSpan(float l) {
	lifespan = l;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

// Set an image if a path is valid
//
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
}


//  Render the sprite
//
void Sprite::draw() {	
	if (isPointSprite) {
		drawPoint();
	}
	else {
		ofSetColor(255, 255, 255, 255);

		// draw image about the center point and resize to width and height
		ofPushMatrix(); // save the matrix before rotation
		ofTranslate(position); // translate coordinate to position
		ofRotateRad(this->rot, 0, 0, 1); // rotate
		ofTranslate(-position); // change coordinate back to 0,0,0
		if (haveImage)
			image.draw(-width / 2.0 + position.x, -height / 2.0 + position.y, width, height);
		else {
			// in case no image is supplied, draw something.
			ofSetColor(255, 0, 0);
			ofDrawRectangle(-width / 2.0 + position.x, -height / 2.0 + position.y, width, height);
		}
		ofPopMatrix();
	}
}

void Sprite::drawPoint() {
	color = ofColor::white;
	ofDrawBitmapString("+" + std::to_string(points), position.x, position.y);
}

void Sprite::setSound(string path) {
	hasSound = sound.load(path);
}

// Play sound of sprite
void Sprite::playSound() {
	if (hasSound)
		sound.play();
}
