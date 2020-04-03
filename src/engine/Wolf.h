#ifndef WOLF_H
#define WOLF_H

#include <iostream>
#include "AnimatedSprite.h"
#include <vector>
#include <string>
#include <fstream>
#include "Sprite.h"
#include "Projectile.h"
#include "Controls.h"
#include "Player.h"

using namespace std;

class Wolf : public AnimatedSprite{

public:
	Wolf(Player* sayu);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

    void onMeleeStrike();
    
    // void Wolf::onEssenceStrike(Weapon* w);
    // void Wolf::onCollision(DisplayObject* other);

    void save(ofstream &out);
    
    void charge();

    void prepareCharge();

    void setPatrolRange();

    void patrol();

    void moveToTarget();

    bool isTargetReached();
	
	/* Health and such */
	int health = 100;
	int maxHealth = 100;

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;

private:
    Player* sayu;
    bool clean = false;
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


	// int oldX=0, oldY=0;

	// int _jumpVel = -15;s

	//void initIFrames(int numFrames);

};

#endif