#pragma once
#include "ofMain.h"

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
	ofVec3f position, scale;
	float	rot;
	bool	bSelected;

	BaseObject();
	void setPosition(ofVec3f);
};