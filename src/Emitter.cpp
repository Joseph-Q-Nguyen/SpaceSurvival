#include "Emitter.h"

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = -1;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
	hasEmitSound = false;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + position.x, -image.getHeight() / 2.0 + position.y);
		}
		else {
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2 + position.x, -height / 2 + position.y, width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (started) {
		float time = ofGetElapsedTimeMillis();
		if (isEmitting && (time - lastSpawned) > (1000.0 / rate)) { // if the sprite is the first one, emit immediately otherwise wait until correct time
			// spawn a new sprite
			Sprite sprite;
			sprite.velocity = velocity;
			sprite.lifespan = lifespan;
			sprite.setPosition(position);
			sprite.birthtime = time;
			sprite.width = childWidth; // set width and height sprite
			sprite.height = childHeight;
			sprite.color = childColor;
			sprite.points = childPoints;
			sprite.isPointSprite = isPointSprite;
			//if (isPointSprite)
				//cout << "POINT\n";
			if (haveChildImage)
				sprite.setImage(childImage);
			if (hasEmitSound) {
				sprite.setSound(emitSoundPath);
				emitSound.play();
				sprite.playSound();
			}
			sys->add(sprite);
			lastSpawned = time;
		}
	}
	if (oneShot) {
		isEmitting = false;	
	}
	sys->update(); // if emitter is not on, still update the system to move the sprites
}

// Start/Stop the emitter.
//
void Emitter::start() {
	if (!started) {
		started = true;
		lastSpawned = ofGetElapsedTimeMillis();
	}
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
}

void Emitter::setRate(float r) {
	rate = r;
}