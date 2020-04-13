#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <iostream>
#include "AnimatedSprite.h"
#include <vector>
#include <string>
#include <fstream>
#include "Sprite.h"
#include "CollisionSystem.h"
#include "Controls.h"

using namespace std;

class Projectile : public AnimatedSprite{

public:
	Projectile();
    Projectile(string face, SDL_Point position, int type);
	
	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

	//void onEnemyCollision(Enemy* enemy);
	virtual void onCollision(DisplayObject* other);

	/* Durability and Distance */
	int durability = 1;
	int Distance = 100;
	int speed = 10;

	/* direction */
	string dir;

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;



private:
	

};

#endif