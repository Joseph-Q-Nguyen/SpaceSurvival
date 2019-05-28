#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "SpaceShip.h"
#include "Emitter.h"
#include "LaserEmitter.h"
#include "Sprite.h"
#include "BaseObject.h"
#include "SpriteSystem.h"
#include "ParticleEmitter.h"


//typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

class ofApp : public ofBaseApp {
public:
	// Game window constants
	static const int windowWidth = 1334; 
	static const int windowHeight = 750;

	SpaceShip *spaceShip; // control a spaceship
	glm::vec3 leftVelocity; // individual velocities to control the spaceship
	glm::vec3 rightVelocity;
	glm::vec3 upVelocity;
	glm::vec3 downVelocity;
	bool invulnerable = false;

	glm::vec3 lastMouse;

	LaserEmitter *laserEmitter; // invisible emitter that will emit lasers, I could have also just made the spaceship the emitter.

	ofImage backGround;
	ofImage backGroundWrap; // second background that creates a wrapping affect

	// booleans 
	bool gameStart; // used to put game in idle mode
	bool hasBackGround;
	bool selected; // used to select spaceship with mouse
	bool hidePanel;
	bool followMouse = false;

	float gameStartTime;
	bool startedTime = false;

	int score = 0;
	
	ofSoundPlayer explosion;
	bool hasExplosiveSound;

	ofImage whiteEnemyImg;
	ofImage greenEnemyImg;
	ofImage redEnemyImg;
	ofImage purpleEnemyImg;
	ofImage ufoImg;

	bool hasWhiteEnemyImg;
	bool hasGreenEnemyImg;
	bool hasRedEnemyImg;
	bool hasPurpleEnemyImg;
	bool hasUfoImg;

	int health;
	bool gameOver;

	float backGroundY;
	float backGroundWrapY;

	ParticleEmitter *particleEm;
	//vector<ParticleEmitter> particleEmitters;

	ParticleEmitter *splittingEm;

	ofTrueTypeFont myFont;
	bool hasFont = false;

	// Displays current wave
	int waveCount = 1;
	bool onGoingWave = false;
	float waveStartTime;
	float waveEndTime;
	bool waveDisplay = false;
	bool waveEndDisplay = false;

	int waveDuration = 10; // in seconds, will increase by 1 second every wave

	float enemyVelocity = 200;
	float enemyRate = 1;

	ParticleEmitter *exhaust; // used for spaceship exhaust

	Emitter *pawns; // enemies that go in a straight line
	Emitter *swervers; // enemies that swerve side to side
	Emitter *splitters; // enemies that split
	Emitter *seekers; // enemies that lock to player
	Emitter *trackingEnemies; // ufo's that aim for player, spawns during 5th wave

	Emitter *pointEmitter; // will be used to display points after sprite dies

	void setup();
	void update();
	void draw();

	bool onScreen(glm::vec3 point);
	bool simpleDetection(glm::vec3, float, float, glm::vec3, float, float);
	void updateCosPath(Emitter *);
	void updateSinPath(Emitter *);
	void smoothMove(SpaceShip *, glm::vec3 destinationPoint, int factor);
	void resetGame();
	void rotateEnemy(Emitter *);
	bool noEnemies();
	void checkLaserCollision(Emitter *, Sprite *);
	void checkPlayerCollision(Emitter *);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};

