
#include "LaserEmitter.h"

LaserEmitter::LaserEmitter():Emitter(new SpriteSystem()) {
	isEmitting = false;
	childWidth = 20;
	childHeight = 80;
	haveChildImage = childImage.load("images//mybeam.png");
	emitSoundPath = "sounds/gunsound.mp3";
	hasEmitSound = true;
	emitSound.load(emitSoundPath);
}