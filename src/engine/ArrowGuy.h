#ifndef ARROWGUY_H
#define ARROWGUY_H

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

class ArrowGuy : public AnimatedSprite{

public:
	ArrowGuy(Player* sayu, string id);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

    void onMeleeStrike();
    
    // void ArrowGuy::onEssenceStrike(Weapon* w);
    virtual void onCollision(DisplayObject* other);

    virtual SDL_Point* getGlobalHitbox();

    void save();

    void setPatrolRange();

    void patrol();

    void moveToTarget();

    bool isTargetReached();

    int fire();


	int shoot = 0;
	/* Health and such */
	int health = 100;
	int maxHealth = 100;

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;

    bool clean = false;
    Sound* shot_sound;


private:
    Player* sayu;
    Benemy* dirt;

    int state = 0;

    int targX = 0;
    int targY = 0;

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

    bool removed = false;


	// int oldX=0, oldY=0;

	// int _jumpVel = -15;s

	//void initIFrames(int numFrames);

};

#endif