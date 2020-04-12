#ifndef BENEMY_H
#define BENEMY_H

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

class Benemy : public AnimatedSprite{

public:
	Benemy(AnimatedSprite* sayu, int x, int y, int velocity, string weapon);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

    
    // void Wolf::onEssenceStrike(Weapon* w);
    // void Wolf::onCollision(DisplayObject* other);

    void save(ofstream &out);

    void fire();

    bool isTargetReached();

    bool distanceReached();
	
	/* Health and such */

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;
    int distance = 20;

    bool clean = false;
private:
    AnimatedSprite* sayu;
    
    int state = 0;

    int targX = 0;
    int targY = 0;


    SDL_Point initial;


    double vel = 2;
    double maxVel = 10;
    double maxRotVel = 20;

    int pauseCount = 0;


	// int oldX=0, oldY=0;

	// int _jumpVel = -15;s

	//void initIFrames(int numFrames);

};

#endif