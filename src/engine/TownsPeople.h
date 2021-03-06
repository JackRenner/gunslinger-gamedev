#ifndef TOWNSPEOPLE_H
#define TOWNSPEOPLE_H

#include <iostream>
#include "AnimatedSprite.h"
#include <vector>
#include <string>
#include <fstream>
#include "Sprite.h"
#include "Controls.h"
#include "Player.h"
#include "./ui/TextBox.h"


using namespace std;

class TownsPeople : public AnimatedSprite{

public:
	TownsPeople(Player* sayu, string id, bool walking, string text);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);
    
    // void Creeper::onEssenceStrike(Weapon* w);
    virtual void onCollision(DisplayObject* other);
    virtual SDL_Point* getGlobalHitbox();

    void save();
    
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


    bool clean = false;


private:
    Player* sayu;
    
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

    int pauseCount = 0;
    int waitToDelete = 0;

    TextBox* townspeopleText;
    bool ableToBuy = true;
    int timeWaited = 0;

    bool walker = false;

	// int oldX=0, oldY=0;

	// int _jumpVel = -15;s

	//void initIFrames(int numFrames);

};

#endif