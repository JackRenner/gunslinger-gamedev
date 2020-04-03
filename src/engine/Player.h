#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "AnimatedSprite.h"
#include <vector>
#include <string>
#include <fstream>
#include "Sprite.h"
#include "Projectile.h"
#include "Controls.h"
#include "events/EventDispatcher.h"
#include "ui/HealthEvent.h"

using namespace std;

extern bool transLock;

class HealthEvent;

class Player : public AnimatedSprite, public EventDispatcher{

public:
	Player();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

	//void onEnemyCollision(Enemy* enemy);
	//virtual void onCollision(DisplayObject* other);

	/* Health and such */
	int health = 100;
	int maxHealth = 100;
	bool poisoned = false;
	int poisonedTime = 0;

	int gun = 0;

	/* shooting */
	int knife_throws = 0;
	int revolver_shots = 0;
	int shotgun_shots = 0;
	int rifle_shots = 0;

	void hitByProjectile(string gun);
	void hitByMelee(string enemy);

	void healPlayer(string method);

	void takeDamage(int damage);

	/* direction */
	string dir;
	int holding = 0;

	//iFrames
	bool iFrames = false;
	int iFrameCount = 0;
	int numIFrames = 0;

	AnimatedSprite* bullet;

	HealthEvent * healthChangeEvent;


	/* Current Enemy player is engaging with*/
	//Enemy* curEnemy = NULL;

private:
	// int oldX=0, oldY=0;

	// int _jumpVel = -15;s

	//void initIFrames(int numFrames);

};

#endif