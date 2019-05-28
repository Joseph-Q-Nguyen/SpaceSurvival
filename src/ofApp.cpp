#include "ofApp.h"

// Used base code given by Kevin Smith
// Laser sound from https://www.freesoundeffects.com/free-sounds/gun-10081/
// Update: 3/10/19 I have created and used my own sprites

bool ofApp::simpleDetection(glm::vec3 p1, float w1, float h1, glm::vec3 p2, float w2, float h2) {
	float contactW = w1 / 2 + w2 / 2;
	float contactH = h1 / 2 + h2 / 2;
	float dx = abs(p1.x - p2.x);
	float dy = abs(p1.y - p2.y);
	if (dx < contactW && dy < contactH)
		return true;
	return false;
}

void ofApp::updateCosPath(Emitter *e) {
	// for each sprite, update its velocity as a function of cos(age)
	for (int i = 0; i < e->sys->sprites.size(); i++) {
		e->sys->sprites[i].velocity = glm::vec3(300 * cos(e->sys->sprites[i].age() / 125), e->sys->sprites[i].velocity.y, 0);
		e->sys->sprites[i].rot = -cos(e->sys->sprites[i].age() / 125) / 2; // set rotation to be as a function of cos, I played aournd with it and turns out the negative value works!
	}
}

void ofApp::updateSinPath(Emitter *e) {
	// for each sprite, update its velocity as a function of sin(age)
	for (int i = 0; i < e->sys->sprites.size(); i++) {
		e->sys->sprites[i].velocity = glm::vec3(400 * sin(e->sys->sprites[i].age() / 125), e->sys->sprites[i].velocity.y, 0);
		e->sys->sprites[i].rot = -sin(e->sys->sprites[i].age() / 125) / 2;
	}
}

void ofApp::smoothMove(SpaceShip *ship, glm::vec3 destinationPoint, int factor) {
	if (destinationPoint.x + ship->width / 2.0 > windowWidth)
		destinationPoint.x = windowWidth - (ship->width / 2.0);
	if (destinationPoint.x - ship->width / 2.0 < 0)
		destinationPoint.x = ship->width / 2.0;
	if (destinationPoint.y - ship->height / 2.0 < 0)
		destinationPoint.y = ship->height / 2.0;
	if (destinationPoint.y + ship->height / 2.0 > windowHeight)
		destinationPoint.y = windowHeight - (ship->height / 2.0);

	float distX = destinationPoint.x - ship->position.x;
	float distY = destinationPoint.y - ship->position.y;
	
	if (ship->position.x != destinationPoint.x)
		ship->position.x += distX / factor;
	if (ship->position.y != destinationPoint.y)
		ship->position.y += distY / factor;
}


// 4th Wave of enemies will target player
void ofApp::rotateEnemy(Emitter *em) {
	for (int i = 0; i < em->sys->sprites.size(); i++) {
		ofVec3f turn = spaceShip->position - em->sys->sprites[i].position;

		float angle = em->sys->sprites[i].velocity.angleRad(turn);
		if (isnan(angle)) // for some reason, the angle sometimes is nan
			angle = 0;
		ofVec3f cross = em->sys->sprites[i].velocity.getCrossed(turn);
		if (cross.z < 0)
			angle = -angle;

		em->sys->sprites[i].velocity = em->sys->sprites[i].velocity.length() * turn.getNormalized();
		em->sys->sprites[i].rot += angle;
	}
}

void ofApp::resetGame() {
	health = 10;
	score = 0;
	gameOver = false;
	startedTime = false;
	gameStartTime = 0;
	backGroundY = 0;
	backGroundWrapY = -windowHeight;
	waveDuration = 10;
	enemyVelocity = 200;
	enemyRate = 1;
	waveCount = 1;
	waveStartTime = 0;
	waveEndTime = 0;
	waveDisplay = false;
	waveEndDisplay = false;
	onGoingWave = false;

	pawns->childWidth = 50;
	pawns->childHeight = 50;
	pawns->childColor = ofColor::white;
	if (hasWhiteEnemyImg)
		pawns->setChildImage(whiteEnemyImg);
	pawns->drawable = false;
	pawns->childPoints = 10;

	swervers->childWidth = 50;
	swervers->childHeight = 50;
	swervers->childColor = ofColor::green;
	if (hasGreenEnemyImg)
		swervers->setChildImage(greenEnemyImg);
	swervers->drawable = false;
	swervers->childPoints = 20;

	splitters->childWidth = 50;
	splitters->childHeight = 50;
	splitters->childColor = ofColor::red;
	if (hasRedEnemyImg)
		splitters->setChildImage(redEnemyImg);
	splitters->drawable = false;
	splitters->childPoints = 30;

	seekers->childWidth = 50;
	seekers->childHeight = 50;
	seekers->childColor = ofColor::purple;
	if (hasPurpleEnemyImg)
		seekers->setChildImage(purpleEnemyImg);
	seekers->drawable = false;
	seekers->childPoints = 40;

	trackingEnemies->setPosition(glm::vec3(ofRandom(windowWidth), -50, 0));
	trackingEnemies->setVelocity(glm::vec3(0, 100, 0));
	trackingEnemies->setRate(.25);
	trackingEnemies->setLifespan(5000);
	trackingEnemies->childWidth = 50;
	trackingEnemies->childHeight = 50;
	trackingEnemies->drawable = false;
	if (hasUfoImg)
		trackingEnemies->setChildImage(ufoImg);
	trackingEnemies->childPoints = 50;

	pawns->isEmitting = false;
	swervers->isEmitting = false;
	splitters->isEmitting = false;
	seekers->isEmitting = false;
	trackingEnemies->isEmitting = false;

	pawns->sys->sprites.clear();
	swervers->sys->sprites.clear();
	splitters->sys->sprites.clear();
	seekers->sys->sprites.clear();
	trackingEnemies->sys->sprites.clear();
	splittingEm->sys->particles.clear();

}

// returns true if no enemies are present
bool ofApp::noEnemies() {
	return pawns->sys->sprites.size() == 0 &&
		swervers->sys->sprites.size() == 0 &&
		splitters->sys->sprites.size() == 0 &&
		seekers->sys->sprites.size() == 0 &&
		trackingEnemies->sys->sprites.size() == 0 &&
		splittingEm->sys->particles.size() == 0;
}

void ofApp::checkLaserCollision(Emitter *e, Sprite *laserPtr) {
	for (int j = 0; j < e->sys->sprites.size(); j++) {
		Sprite *sPtr = &(e->sys->sprites[j]);
		if (simpleDetection(sPtr->position, sPtr->width, sPtr->height, laserPtr->position, laserPtr->width, laserPtr->height)) {
			sPtr->lifespan = 0;
			laserPtr->lifespan = 0;
			score += sPtr->points;
			pointEmitter->setPosition(sPtr->position);
			pointEmitter->childPoints = sPtr->points;
			pointEmitter->isEmitting = true;
			if (hasExplosiveSound)
				explosion.play();
			particleEm->setPosition(sPtr->position);
			particleEm->particleColors = sPtr->color;
			particleEm->sys->reset();
			particleEm->start();

			if (sPtr->color == ofColor::red) { // splitting enemies are red
				splittingEm->setPosition(sPtr->position);
				splittingEm->sys->reset();
				splittingEm->start();
			}
		}
	}
}

void ofApp::checkPlayerCollision(Emitter *e) {
	for (int i = 0; i < e->sys->sprites.size(); i++) {
		Sprite s = e->sys->sprites[i];
		if (simpleDetection(s.position, s.width, s.height, spaceShip->position, spaceShip->width, spaceShip->height)) {
			e->sys->sprites[i].lifespan = 0;
			if (hasExplosiveSound)
				explosion.play();
			health--;
			particleEm->setPosition(s.position);
			particleEm->particleColors = s.color;
			particleEm->sys->reset();
			particleEm->start();
		}
	}
}


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	
	gameStart = false; // set booleans
	selected = false;
	hidePanel = true;

	splittingEm = new ParticleEmitter();
	splittingEm->setGroupSize(2);
	splittingEm->setLifespan(5);
	splittingEm->setOneShot(true);
	splittingEm->setParticleRadius(12.5);
	splittingEm->sys->addForce(new ImpulseRadialForce(10000));
	splittingEm->sys->addForce(new GravityForce(glm::vec3(0, 20000, 0)));
	splittingEm->sys->addForce(new TurbulenceForce(glm::vec3(-100, -100, 0), glm::vec3(100, 100, 0)));
	splittingEm->setEmitterType(RadialEmitter);

	if (whiteEnemyImg.load("images//enemy1.png"))
		hasWhiteEnemyImg = true;
	if (greenEnemyImg.load("images//enemy2.png"))
		hasGreenEnemyImg = true;
	if (redEnemyImg.load("images//enemy3.png")) {
		hasRedEnemyImg = true;
		splittingEm->particleImage = redEnemyImg;
		splittingEm->hasParticleImage = true;
	}
	if (purpleEnemyImg.load("images//enemy4.png"))
		hasPurpleEnemyImg = true;
	if (ufoImg.load("images//ufo.png"))
		hasUfoImg = true;

	if (backGround.load("images//joseph_space_bg2.jpg") && backGroundWrap.load("images//joseph_space_bg2.jpg")) { // set background 
		hasBackGround = true;
	}
	else
		ofSetBackgroundColor(ofColor::black);

	if (explosion.load("sounds/canon.wav"))
		hasExplosiveSound = true;

	if (myFont.load("fonts//SFAtarianSystemBold.ttf", 25))
		hasFont = true;

	spaceShip = new SpaceShip(80, 160); // create spaceship with width 80 and height 160
	spaceShip->position = glm::vec3(windowWidth / 2, windowHeight - 100, 0); // set its position at center x and near bottom

	laserEmitter = new LaserEmitter();	// create a laser emitter that follows spaceship
	laserEmitter->setPosition(spaceShip->position);
	laserEmitter->setLifespan(1000);    // convert to milliseconds 
	laserEmitter->setVelocity(ofVec3f(0, -1000, 0));
	laserEmitter->drawable = false;
	laserEmitter->start();
	laserEmitter->isEmitting = false;

	trackingEnemies = new Emitter(new SpriteSystem()); // create an emitter for ufo enemy
	trackingEnemies->start();

	particleEm = new ParticleEmitter();
	particleEm->setGroupSize(100);
	particleEm->setLifespan(1);
	particleEm->setOneShot(true);
	particleEm->setParticleRadius(2.5);
	particleEm->particleColors = ofColor::yellow;
	particleEm->sys->addForce(new ImpulseRadialForce(10000));
	particleEm->sys->addForce(new GravityForce(glm::vec3(0, 10000, 0)));
	particleEm->sys->addForce(new TurbulenceForce(glm::vec3(-100, -100, 0), glm::vec3(100, 100, 0)));
	particleEm->setEmitterType(RadialEmitter);

	pawns = new Emitter(new SpriteSystem());
	swervers = new Emitter(new SpriteSystem());
	splitters = new Emitter(new SpriteSystem());
	seekers = new Emitter(new SpriteSystem());

	pawns->start();
	swervers->start();
	splitters->start();
	seekers->start();

	pointEmitter = new Emitter(new SpriteSystem());
	pointEmitter->setPosition(glm::vec3(windowWidth / 2, windowHeight / 2, 0));
	pointEmitter->setVelocity(glm::vec3(0, 0, 0));
	pointEmitter->setRate(1000);
	pointEmitter->setLifespan(1000);
	pointEmitter->childHeight = 50;
	pointEmitter->childWidth = 50;
	pointEmitter->drawable = false;
	pointEmitter->oneShot = true;
	pointEmitter->isPointSprite = true;
	pointEmitter->isEmitting = false;
	pointEmitter->start();

	exhaust = new ParticleEmitter(new ParticleSystem());
	exhaust->setPosition(spaceShip->position);
	exhaust->setVelocity(glm::vec3(0, 300, 0));
	exhaust->setParticleRadius(3);
	exhaust->sys->addForce(new TurbulenceForce(glm::vec3(-500, -500, 0), glm::vec3(500, 500, 0)));
	exhaust->setLifespan(1);
	exhaust->setRate(100);
	exhaust->particleColors = ofColor::red;
	exhaust->start();

	resetGame();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (gameStart && !gameOver) { 
		enemyVelocity = 200 + (10 * waveCount); // every round, velocity of enemies get faster
		enemyRate = 1 + (0.5 * waveCount); // every round, rate increases
		trackingEnemies->setRate(((waveCount - 5) * 0.1) + 0.25); // every round, more ufos spawn

		// randomize exhaust emmiter to give illusion of multiple emitters
		exhaust->setPosition(glm::vec3(spaceShip->position.x + ofRandom(-1, 1) * 10, spaceShip->position.y + spaceShip->height / 2, 0));
		exhaust->update(); // change exhaust to be at position on spaceship

		pointEmitter->update();

		if (!startedTime) {
			gameStartTime = ofGetElapsedTimef();
			startedTime = true;
			waveStartTime = gameStartTime;
			waveDisplay = true;
		}

		if (followMouse) {
			smoothMove(spaceShip, lastMouse, 4);
		}
		else { // check if the spaceship is at any of the boundaries and set its corresponding velocity to zero
			if (spaceShip->position.x + (spaceShip->width / 2.0) > windowWidth)
				rightVelocity = glm::vec3(0, 0, 0);
			if (spaceShip->position.x < spaceShip->width / 2.0)
				leftVelocity = glm::vec3(0, 0, 0);
			if (spaceShip->position.y < (spaceShip->height / 2.0))
				upVelocity = glm::vec3(0, 0, 0);
			if (spaceShip->position.y + (spaceShip->height / 2.0) > windowHeight)
				downVelocity = glm::vec3(0, 0, 0);

			spaceShip->velocity = leftVelocity + rightVelocity + upVelocity + downVelocity; // add velocities up to translate
			spaceShip->update();
		}

		laserEmitter->setRate(waveCount); // laser rate depends on wave number
		laserEmitter->setPosition(glm::vec3(spaceShip->position.x, spaceShip->position.y - spaceShip->height / 2, 0));
		laserEmitter->update();

		// collision checking for spaceship
		checkPlayerCollision(pawns);
		checkPlayerCollision(swervers);
		checkPlayerCollision(splitters);
		checkPlayerCollision(seekers);

		checkPlayerCollision(trackingEnemies);


		for (int i = 0; i < splittingEm->sys->particles.size(); i++) {
			Particle p = splittingEm->sys->particles[i];
			if (simpleDetection(p.position, p.radius, p.radius, spaceShip->position, spaceShip->width, spaceShip->height)) {
				splittingEm->sys->particles[i].lifespan = 0;
				if (hasExplosiveSound)
					explosion.play();
				health--;
				particleEm->setPosition(p.position);
				particleEm->particleColors = ofColor::red;
				particleEm->sys->reset();
				particleEm->start();
			}
		}

		// collision checking for lasers
		for (int i = 0; i < laserEmitter->sys->sprites.size(); i++) {
			Sprite *laserPtr = &(laserEmitter->sys->sprites[i]);
			
			checkLaserCollision(pawns, laserPtr);
			checkLaserCollision(swervers, laserPtr);
			checkLaserCollision(splitters, laserPtr);
			checkLaserCollision(seekers, laserPtr);
			checkLaserCollision(trackingEnemies, laserPtr);
			
			for (int j = 0; j < splittingEm->sys->particles.size(); j++) {
				Particle p = splittingEm->sys->particles[j];
				if (simpleDetection(p.position, p.radius, p.radius, laserPtr->position, laserPtr->width, laserPtr->height)) {
					splittingEm->sys->particles[j].lifespan = 0;
					laserEmitter->sys->sprites[i].lifespan = 0;
					if (hasExplosiveSound)
						explosion.play();
					score += 60; // splitting enemies will be worth 60 points
					pointEmitter->setPosition(p.position);
					pointEmitter->childPoints = 60;
					pointEmitter->isEmitting = true;
					particleEm->setPosition(p.position);
					particleEm->particleColors = ofColor::red;
					particleEm->sys->reset();
					particleEm->start();
				}
			}
		}
	
		backGroundY += 1; // these statements will move the background
		backGroundWrapY += 1;
		if (backGroundY >= windowHeight)
			backGroundY = -windowHeight;
		if (backGroundWrapY >= windowHeight)
			backGroundWrapY = -windowHeight;
		
		if (health <= 0 && !invulnerable) {
			gameOver = true;
		}

		if (!onGoingWave && waveDisplay && ofGetElapsedTimef() - waveStartTime > 2) {// displaying wave should take 2 seconds
			waveDisplay = false;
			onGoingWave = true;
		}
		if (onGoingWave && !waveDisplay && ofGetElapsedTimef() - waveStartTime > waveDuration) {// if wave time completes that stop the wave
			onGoingWave = false;
		}
		if (!onGoingWave && noEnemies() && !waveDisplay && !waveEndDisplay) {
			waveEndDisplay = true;
			waveEndTime = ofGetElapsedTimef();
		}
		if (!onGoingWave && waveEndDisplay && ofGetElapsedTimef() - waveEndTime > 3) {
			waveEndDisplay = false;
			waveCount++;
			waveDisplay = true;
			waveDuration++;
			waveStartTime = ofGetElapsedTimef();
			health < 10 ? health = 10 : health += 5; // when survive round, get health back if less than max health otherwise give 2
		}

		if (onGoingWave) {
			pawns->setPosition(glm::vec3(ofRandom(windowWidth), -50, 0));
			pawns->setVelocity(glm::vec3(0, enemyVelocity, 0));
			pawns->setRate(enemyRate);
			pawns->setLifespan(((windowHeight / pawns->velocity.y) + 1) * 1000); // set lifespan depending on its down speed and window width, the sprite should be removed if out of screen

			swervers->setPosition(glm::vec3(ofRandom(windowWidth), -50, 0));
			swervers->setVelocity(glm::vec3(0, enemyVelocity, 0));
			swervers->setRate(enemyRate);
			swervers->setLifespan(((windowHeight / swervers->velocity.y) + 1) * 1000); // set lifespan depending on its down speed and window width, the sprite should be removed if out of screen

			splitters->setPosition(glm::vec3(ofRandom(windowWidth), -50, 0));
			splitters->setVelocity(glm::vec3(0, enemyVelocity, 0));
			splitters->setRate(enemyRate);
			splitters->setLifespan(((windowHeight / splitters->velocity.y) + 1) * 1000); // set lifespan depending on its down speed and window width, the sprite should be removed if out of screen
		
			seekers->setPosition(glm::vec3(ofRandom(windowWidth), -50, 0));
			seekers->setVelocity(glm::vec3(0, enemyVelocity, 0));
			seekers->setRate(enemyRate);
			seekers->setLifespan(((windowHeight / seekers->velocity.y) + 1) * 1000); // set lifespan depending on its down speed and window width, the sprite should be removed if out of screen

			if (waveCount % 5 == 1) { // pawns
				pawns->isEmitting = true;
			}
			else if (waveCount % 5 == 2) { // swervers 
				swervers->isEmitting = true;
			}
			else if (waveCount % 5 == 3) { // splitters
				splitters->isEmitting = true;
			}
			else if (waveCount % 5 == 4) { // seekers
				seekers->isEmitting = true;
			}
			else { // swarm wave, all enemy types appear
				pawns->setRate(enemyRate / 4);
				swervers->setRate(enemyRate / 4);
				splitters->setRate(enemyRate / 4);
				seekers->setRate(enemyRate / 4);

				pawns->isEmitting = true;
				swervers->isEmitting = true;
				splitters->isEmitting = true;
				seekers->isEmitting = true;
			}
			if (waveCount > 5) {
				trackingEnemies->setPosition(glm::vec3(ofRandom(windowWidth), -50, 0));
				trackingEnemies->setVelocity(spaceShip->position - trackingEnemies->position); // velocity is set to follow player
				trackingEnemies->isEmitting = true;
			}
		}

		if (!onGoingWave) {
			pawns->isEmitting = false;
			swervers->isEmitting = false;
			splitters->isEmitting = false;
			seekers->isEmitting = false;
			trackingEnemies->isEmitting = false;
		}

		pawns->update();
		swervers->update();
		updateCosPath(swervers);
		splitters->update();
		seekers->update();
		rotateEnemy(seekers);

		particleEm->update();
		splittingEm->update();
		trackingEnemies->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (hasBackGround) {
		ofSetColor(ofColor::white);
		backGround.draw(0, backGroundY, windowWidth, windowHeight);
		ofSetColor(ofColor::white);
		backGroundWrap.draw(0, backGroundWrapY, windowWidth, windowHeight);
	}
	if (!gameStart) { // idle mode check
		if (hasFont)
			myFont.drawString("Space Surivival\nHow long can you survive?\nBy Joseph Nguyen\nPRESS SPACE TO START\n\nControls: \nArrows Keys = Move\nSpace = Shoot\nM = Toggle Mouse Follow", (windowWidth / 2) - 100, windowHeight / 2 - 100);
		else
			ofDrawBitmapString("Space Surivival\nHow long can you survive?\nBy Joseph Nguyen\nPRESS SPACE TO START\n\nControls: \nArrows Keys = Move\nSpace = Shoot\nM = Toggle Mouse Follow", (windowWidth / 2) - 100, windowHeight / 2 - 100);
	}
	else if (!gameOver) {
		laserEmitter->draw();
		exhaust->draw();
		spaceShip->draw();
		pawns->draw();
		swervers->draw();
		splitters->draw();
		seekers->draw();
		trackingEnemies->draw();
		pointEmitter->draw();

		if (waveDisplay) {
			string display;
			display += "Wave " + std::to_string(waveCount) + " Start";
			if (hasFont)
				myFont.drawString(display, (windowWidth / 2) - 50, windowHeight / 2);
			else
				ofDrawBitmapString(display, (windowWidth / 2) - 50, windowHeight / 2);
		}
		if (waveEndDisplay) {
			string display;
			display += "Wave " + std::to_string(waveCount) + " Completed";
			if (hasFont)
				myFont.drawString(display, (windowWidth / 2) - 50, windowHeight / 2);
			else
				ofDrawBitmapString(display, (windowWidth / 2) - 50, windowHeight / 2);
		}		
		ofSetColor(ofColor::white);
		string scoreMes;
		scoreMes += "SCORE: " + std::to_string(score);
		if (hasFont)
			myFont.drawString(scoreMes, (windowWidth / 2) - 50, 100);
		else
			ofDrawBitmapString(scoreMes, (windowWidth / 2) - 50, 100);
		string str;
		str += "Health: " + std::to_string(health);
		if (hasFont)
			myFont.drawString(str, (windowWidth / 2) - 50, 150);
		else
			ofDrawBitmapString(str, (windowWidth / 2) - 50, 150);

		if (hasFont)
			myFont.drawString("Wave: " + std::to_string(waveCount), (windowWidth / 2) - 50, 200);
		else
			ofDrawBitmapString("Wave: " + std::to_string(waveCount), (windowWidth / 2) - 50, 200);

		particleEm->draw();
		splittingEm->draw();
	}
	else {
		if (hasFont)
			myFont.drawString("GAME OVER\n\n\nPress R to Restart", (windowWidth / 2) - 50, windowHeight / 2);
		else
			ofDrawBitmapString("GAME OVER\n\n\nPress R to Restart", (windowWidth / 2) - 50, windowHeight / 2);
		string str;
		str += "\nFinal Score: " + std::to_string(score) + "\nFinal Wave: " + std::to_string(waveCount);
		if (hasFont)
			myFont.drawString(str, (windowWidth / 2) - 50, (windowHeight / 2));
		else
			ofDrawBitmapString(str, (windowWidth / 2) - 50, (windowHeight / 2));
	}
	// draw screen data
		//
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, 15);
}

// helper method checks if a point is on window screen
bool ofApp::onScreen(glm::vec3 point) {
	return point.x > 0 && point.x < windowWidth && point.y > 0 && point.y < windowHeight;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) { // uses wasd or arrow keys to move, the corresponding key changes the velocity
	switch (key) {
	case OF_KEY_RIGHT:
	case 'd':
		rightVelocity = glm::vec3(20, 0, 0);
		break;
	case OF_KEY_LEFT:
	case 'a':
		leftVelocity = glm::vec3(-20, 0, 0);
		break;
	case OF_KEY_UP:
	case 'w':
		upVelocity = glm::vec3(0, -20, 0);
		break;
	case OF_KEY_DOWN:
	case 's':
		downVelocity = glm::vec3(0, 20, 0);
		break;
	case ' ':
		if (!gameStart) { // if in idle mode, then start game
			gameStart = true;
		}
		else if (gameStart) {
			laserEmitter->isEmitting = true;	// otherwise shoot laser
		}
		break;
	case 'r':
		if (gameOver) // if game over reset game
			resetGame();
		break;
	case 'h':
		hidePanel = !hidePanel; // hidden button for controling laser
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'm':
		followMouse = !followMouse; // press m to have toggle follow mouse
		break;
	case 'i':
		invulnerable = !invulnerable; // hidden cheat that allows player to be immune to damage
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) { // if any f the corresponding keys are released, set the velocity to zero
	switch (key) {
	case OF_KEY_RIGHT:
	case 'd':
		rightVelocity = glm::vec3(0, 0, 0);
		break;
	case OF_KEY_LEFT:
	case 'a':
		leftVelocity = glm::vec3(0, 0, 0);
		break;
	case OF_KEY_UP:
	case 'w':
		upVelocity = glm::vec3(0, 0, 0);
		break;
	case OF_KEY_DOWN:
	case 's':
		downVelocity = glm::vec3(0, 0, 0);
		break;
	case ' ':
		laserEmitter->isEmitting = false;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	lastMouse = glm::vec3(x, y, 0);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) { // move the spaceship with mouse if it is selected
	glm::vec3 translate = glm::vec3(x, y, 0) - lastMouse;
	if (selected && onScreen(spaceShip->position + translate))
		spaceShip->position += translate;
	lastMouse = glm::vec3(x, y, 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) { // check if mouse is inside of spaceship's hitbox
	if (x > spaceShip->position.x - spaceShip->width / 2
		&& x < spaceShip->position.x + spaceShip->width / 2
		&& y > spaceShip->position.y - spaceShip->height / 2
		&& y < spaceShip->position.y + spaceShip->height / 2)
		selected = true;
	else
		selected = false;
	lastMouse = glm::vec3(x, y, 0);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	selected = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

