#ifndef SHOTGUNGUY_H
#define SHOTGUNGUY_H

#include <iostream>
#include "AnimatedSprite.h"
#include <vector>
#include <string>
#include <fstream>
#include "Sprite.h"
#include "Projectile.h"
#include "Controls.h"
#include "Player.h"
#include "Benemy.h"

using namespace std;

class ShotgunGuy : public AnimatedSprite{

public:
	ShotgunGuy(Player* sayu, string id);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

    void onMeleeStrike();
    
    virtual void onCollision(DisplayObject* other);
    virtual SDL_Point* getGlobalHitbox();

    void save();
    
    void charge();

    void moveToTarget();

    bool isTargetReached();

    void reload();

    int fire();

    void smokeBomb();

	int shoot = 0;
	/* Health and such */
	int health = 500;

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;

    int shots_fired = 0;
    bool clean = false;
    int state = 0;
    bool dynamite = false;

    Sound* shot_sound;
    Sound* explosion;


private:
    Player* sayu;
    Benemy* dirt;
    

    double targX = 0;
    double targY = 0;

    int maxPatX = 0;
    int maxPatY = 0;
    int minPatX = 0;
    int minPatY = 0;

    double vel = 4;
    double maxVel = 20;
    double acc = 2;
    double rotVel = 2;
    double rotAcc = 2;
    double maxRotVel = 20;

    int pauseCount = 0;

    string lastId;
    string lastTwoId;
    string lastThreeId;
    string lastFourId;
    string lastFiveId;
    string lastSixId;

    int waitToReload = 0;
    
    int waitToSmokeTimer = 0;
    int waitToSmoke = rand() % 500 + 250;

	int oldAlpha = 0;

    bool removed = false;


};

#endif