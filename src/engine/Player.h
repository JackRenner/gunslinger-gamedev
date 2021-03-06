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
#include "ui/WeaponSelectEvent.h"
#include "Game.h"
#include "Sound.h"

using namespace std;

extern bool transLock;

class HealthEvent;
class WeaponSelectEvent;

class Player : public AnimatedSprite, public EventDispatcher{

public:
	Player();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

	//void onEnemyCollision(Enemy* enemy);
	virtual void onCollision(DisplayObject* other);
	virtual SDL_Point* getGlobalHitbox();
	void save();

	/* Health and such */
	int health = 500;
	int maxHealth = 100;
	bool poisoned = false;
	int poisonedTime = 0;
	int rollcount=0;
	int rollpause=0;

	int gun = 0;

	/* shooting */
	int knife_throws = 0;
	int revolver_shots = 0;
	int shotgun_shots = 0;
	int rifle_shots = 0;
	int foodNum = 5;

	void hitByProjectile(string gun);

	void heal(string food);

	void hitByMelee(string enemy);

	void healPlayer(string method);

	void takeDamage(int damage);

	void takeNoDamage(int damage);

	void selectWeapon(int gun);

	void areaAccess(int unlock);

	void lightingSystem(bool on); //toggles on and off the torch light lighting system

	/* direction */
	string dir;
	int holding = 0;
	string lastPressed;

	//purchasing counter
	int timeToBuy = 0;

	//AnimatedSprite* bullet;
	AnimatedSprite* bloodSplatter;
	AnimatedSprite* lightingEffect;

	HealthEvent * healthChangeEvent;

	WeaponSelectEvent* selectFist;
	WeaponSelectEvent* selectKnife;
	WeaponSelectEvent* selectPistol;
	WeaponSelectEvent* selectShotgun;
	WeaponSelectEvent* selectRifle;
	WeaponSelectEvent* playerHeal;
	WeaponSelectEvent* playerHeal2;

	WeaponSelectEvent* newlevel;

	WeaponSelectEvent* updateAmmo;

	WeaponSelectEvent* unlockPistol;
	WeaponSelectEvent* unlockRifle;
	WeaponSelectEvent* unlockShotgun;

	bool canyonComplete = false;

	bool lakeUnlocked = false;
	bool lakeComplete = false;
	
	bool badlandsUnlocked = false;
	bool badlandsComplete = false;
	
	bool hideoutUnlocked = false;
	bool hideoutComplete = false;

	bool finalBattleUnlocked = false;
	bool finalBossDefeated = false;

	bool killTheTown = false;
	bool killedTheTown = false;

	bool leftTown = false;

	Sound* grunt;
	Sound* drink;

	// bit of a strange hack, need this so Benemy can have access to it
	Sound* explosion;

	int wolfWaitToDamage = 0;

private:
	// int oldX=0, oldY=0;

	// int _jumpVel = -15;s

	int knifeWaitToDamage = 0;

	bool ableToBuy = false;

	//void initIFrames(int numFrames);

};

#endif