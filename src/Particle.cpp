#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = .1;
	damping = .99;
	mass = 1;
	color = ofColor::aquamarine;
}

void Particle::draw() {
	//ofSetColor(color);
	if (hasSprite) {
		ofSetColor(ofColor::white);
		image.draw(position, radius * 2, radius * 2);
	}
	else {
		ofSetColor(ofMap(age(), 0, lifespan, color.r, 10), ofMap(age(), 0, lifespan, color.g, 10), ofMap(age(), 0, lifespan, color.b, 10));
		ofDrawSphere(position, radius);
	}
}

// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

	// interval for this step
	//
	float dt = 1.0 / ofGetFrameRate();

	// update position based on velocity
	//
	position += (velocity * dt);

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	
	if (!hasForce) {
		accel += (forces * (1.0 / mass));
		hasForce = true;
	}
	velocity += accel * dt;

	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on particle (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}


