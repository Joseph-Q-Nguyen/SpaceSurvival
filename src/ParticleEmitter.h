#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "TransformObject.h"
#include "ParticleSystem.h"

typedef enum { DirectionalEmitter, RadialEmitter, SphereEmitter } EmitterType;

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class ParticleEmitter : public TransformObject {
public:
	ParticleEmitter();
	ParticleEmitter(ParticleSystem *s);
	//ParticleEmitter(const ParticleEmitter &p);
	~ParticleEmitter();
	void init();
	void draw();
	void start();
	void stop();
	void setLifespan(const float life)   { lifespan = life; }
	void setVelocity(const ofVec3f &vel) { velocity = vel; }
	void setRate(const float r) { rate = r; }
	void setParticleRadius(const float r) { particleRadius = r; }
	void setEmitterType(EmitterType t) { type = t; }
	void setGroupSize(int s) { groupSize = s; }
	void setOneShot(bool s) { oneShot = s; }
	void update();
	void spawn(float time);
	ParticleSystem *sys;
	float rate;         // per sec
	bool oneShot;
	bool fired;
	ofVec3f velocity;
	float lifespan;     // sec
	bool started;
	float lastSpawned;  // ms
	float particleRadius;
	float radius;
	bool visible;
	int groupSize;      // number of particles to spawn in a group
	bool createdSys;
	EmitterType type;
	ofColor particleColors;
	ofImage particleImage; // draw particle image in place of particle
	bool hasParticleImage = false;;
};
