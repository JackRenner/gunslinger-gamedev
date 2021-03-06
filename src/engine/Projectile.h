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
#include "Player.h"

class Player;

using namespace std;

class Projectile : public AnimatedSprite{

public:
	Projectile();
    Projectile(Player* player, string face, SDL_Point position, int type, string id);
	
	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

	//void onEnemyCollision(Enemy* enemy);
	virtual void onCollision(DisplayObject* other);
	virtual SDL_Point* getGlobalHitbox();

	/* Durability and Distance */
	int durability = 1;
	int Distance = 100;
	int speed = 10;

	/* direction */
	string dir;

	/* type of weapon */
	string gun;
	bool thrown = false;
	
	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;

	bool hitSomething = false; //stops bullet from moving once it's hit something


private:
	Player* player;
	bool clean = false;
};

#endif