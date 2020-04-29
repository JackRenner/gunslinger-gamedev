#ifndef SHERIFF_H
#define SHERIFF_H

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
#include "./ui/TextBox.h"

using namespace std;

class Sheriff : public AnimatedSprite{

public:
	Sheriff(Player* sayu, string id, vector<string> dialogue);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);
    
    virtual void onCollision(DisplayObject* other);
    virtual SDL_Point* getGlobalHitbox();

    void save();
    
    void charge();

    void prepareCharge();

    void setPatrolRange();

    void patrol();

    void moveToTarget();

    bool isTargetReached();

    int fire();

	int shoot = 0;
	/* Health and such */
	int health = 120;
	int maxHealth = 100;

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;

    int shots_fired = 0;
    bool clean = false;

    bool engagePlayer = false;


private:
    Player* sayu;
    Benemy* dirt;
    
    int state = 0;

    int targX = 0;
    int targY = 0;

    double vel = 4;
    double maxVel = 20;
    double acc = 2;
    double rotVel = 2;
    double rotAcc = 2;
    double maxRotVel = 20;

    int pauseCount = 0;

    string lastId;

    bool removed = false;

	TextBox* sheriffText;

};

#endif