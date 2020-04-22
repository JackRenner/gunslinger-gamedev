#include <iostream>
#include <cstdlib>
#include "Player.h"
#include "Projectile.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Controls.h"
#include "CollisionSystem.h"
#include "Benemy.h"
#include "Sound.h"

using namespace std;

Player::Player() : AnimatedSprite("Player"){

	this->type = "Player";
	
	// this->width = 416;
	// this->height = 454;
	// this->scaleX = 0.15;
	// this->scaleY = 0.15;
	
	this->addAnimation("resources/character/", "FaceUp", 1, 1, true);
	this->addAnimation("resources/character/", "FaceLeft", 1, 1, true);
	this->addAnimation("resources/character/", "FaceRight", 1, 1, true);
	this->addAnimation("resources/character/", "FaceDown", 1, 1, true);

	healthChangeEvent = new HealthEvent(HealthEvent::HEALTH_CHANGE_EVENT, this);

	selectFist = new WeaponSelectEvent(WeaponSelectEvent::SELECT_FIST_EVENT, this);
	selectKnife = new WeaponSelectEvent(WeaponSelectEvent::SELECT_KNIFE_EVENT, this);
	selectPistol = new WeaponSelectEvent(WeaponSelectEvent::SELECT_PISTOL_EVENT, this);
	selectShotgun = new WeaponSelectEvent(WeaponSelectEvent::SELECT_SHOTGUN_EVENT, this);
	selectRifle = new WeaponSelectEvent(WeaponSelectEvent::SELECT_RIFLE_EVENT, this);
	playerHeal = new WeaponSelectEvent(WeaponSelectEvent::PLAYER_HEAL, this);
	
	this->play("FaceUp");
	holding = 0;

	//add blood splatter effect if player is hit
	bloodSplatter = new AnimatedSprite("bs");
	bloodSplatter->addAnimation("./resources/weapons/", "blood_splatter", 14, 1, false);
	bloodSplatter->position.x = bloodSplatter->position.x - (0.5 * this->height);
	bloodSplatter->position.y = bloodSplatter->position.y - (0.25 * this->width);
	bloodSplatter->alpha = 0;
	bloodSplatter->play("blood_splatter");

	this->addChild(bloodSplatter);
}


void Player::update(set<SDL_Scancode> pressedKeys){

	if (controls::pressShift()) {
		if (this->dir == "Up") {
			this->position.y -= 50;
		}
		if (this->dir == "Down") {
			this->position.y += 50;
		}
		if (this->dir == "Right") {
			this->position.x += 50;
		}
		if (this->dir == "Left") {
			this->position.x -= 50;
		}
	}
	AnimatedSprite::update(pressedKeys);	
	controls::update(pressedKeys);

	if (!transLock) {
		if (controls::holdW()) {
			this->dir = "Up";
			this->play("FaceUp");
			//this->position.y -= 4;
			this->position.y -= 10;
		}
		if (controls::holdS()) {
			this->dir = "Down";
			this->play("FaceDown");
			//this->position.y += 4;
			this->position.y += 10;
		}
		if (controls::holdD()) {
			this->dir = "Right";
			this->play("FaceRight");
			//this->position.x += 4;
			this->position.x += 10;
		}
		if (controls::holdA()) {
			this->dir = "Left";
			this->play("FaceLeft");
			//this->position.x -= 4;
			this->position.x -= 10;
		}
		if (controls::holdUp()) {
			this->dir = "Up";
			this->play("FaceUp");
		}
		if (controls::holdDown()) {
			this->dir = "Down";
			this->play("FaceDown");
		}
		if (controls::holdRight()) {
			this->dir = "Right";
			this->play("FaceRight");
		}
		if (controls::holdLeft()) {
			this->dir = "Left";
			this->play("FaceLeft");
		}
	}

	//Allows wolves to nip
	this->wolfWaitToDamage++;
	if(this->wolfWaitToDamage > 10000){//arbitrary number so doesn't cause memory issues at all with too big int
		this->wolfWaitToDamage = 900;
	}
	this->knifeWaitToDamage++;
	if(this->knifeWaitToDamage > 10000){
		this->knifeWaitToDamage = 900;
	}

	// reset health upon death
	if (this->health == 0) {
		this->health = 500;
		dispatchEvent(healthChangeEvent);
	}

}

// do not include attacks from bosses yet
void Player::hitByProjectile(string gun){
	if (gun == "poison") {
		this->poisoned = true;
		this->poisonedTime = 10;
	} else if (gun == "arrow") {
		takeDamage(10);
	} else if (gun == "revolver") {
		takeDamage(15);
	} else if (gun == "shotgun") {
		takeDamage(60);
	} else if (gun == "rifle") {
		takeDamage(40);
	} else if (gun == "dynamite") {
		takeDamage(200);
		Sound* new_sound = new Sound();
		new_sound->playSFX();
	}
}

void Player::heal(string food){
	if (food == "whiskey" && foodNum > 0) {
		takeNoDamage(-20);
	}

}

// do not include attacks from bosses yet
void Player::hitByMelee(string enemy){
	std::cout << this->wolfWaitToDamage << endl;
	if (enemy == "creeper") {
		takeDamage(this->health);
	} else if (enemy == "wolf" && this->wolfWaitToDamage > 40) {
		takeDamage(15);
		this->wolfWaitToDamage = 0;
	}
	else if (enemy == "knife" && this->knifeWaitToDamage > 40) {
		takeDamage(100);
		this->knifeWaitToDamage = 0;
	}
}

//Called automatically by collision system when something collides with the player
//our job is to simply react to that collision.
void Player::onCollision(DisplayObject* other){
	if (other->type == "Benemy") {
		Benemy *temp = (Benemy*) other;
		hitByProjectile(temp->source);
	} else if (other->type == "Projectile") {
		Projectile *temp = (Projectile*) other;
		if (temp->gun == "knife" && temp->thrown) {
			// picking up knife
			this->knife_throws = 0;
		}
	} else if (other->type == "Wolf") {
		hitByMelee("wolf");
	} else if (other->type == "Creeper") {
		hitByMelee("creeper");
	} else if (other->type == "KnifeGuy") {
		hitByMelee("knife");
	}
	else if (other->type == "Obstacle") {
		Game::instance->ourCollisionSystem->resolveCollision(this, other, this->position.x - this->oldX, this->position.y - this->oldY, 0, 0);
	} else if (other->type == "River") {
		this->health = 0;
	}
}

SDL_Point* Player::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(25, 25);
	this->MyGlobalHitbox[1] = temp->transformPoint(75, 25);
	this->MyGlobalHitbox[2] = temp->transformPoint(25, 50);
	this->MyGlobalHitbox[3] = temp->transformPoint(75, 50);
	return this->MyGlobalHitbox;
}

void Player::healPlayer(string method){
	if (method == "bed") {
		this->health = 100;
	} else if (method == "whiskey") {
		this->health += 10;
	} else if (method == "food") {
		this->health += 20;
	}
}

void Player::takeDamage(int damage){
	this->bloodSplatter->alpha = 255;
	this->bloodSplatter->replay();
	if(this->health > 0){
		this->health -= damage;
		if (this->health < 0) {this->health = 0;}
		dispatchEvent(healthChangeEvent);
	}
	
}

void Player::takeNoDamage(int damage){
	//this->bloodSplatter->alpha = 255;
	//this->bloodSplatter->replay();
	if(this->health > 0){
		this->health -= damage;
		if (this->health < 0) {this->health = 0;}
		dispatchEvent(healthChangeEvent);
	}
	
}

void Player::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}



void Player::selectWeapon(int gun) {
	this->gun = gun;
	switch (gun) {
	case 0:
		this->dispatchEvent(selectFist);
		break;
	case 1:
		this->dispatchEvent(selectKnife);
		break;
	case 2:
		this->dispatchEvent(selectPistol);
		break;
	case 3:
		this->dispatchEvent(selectShotgun);
		break;
	case 4:
		this->dispatchEvent(selectRifle);
		break;
	case 5:
		this->dispatchEvent(playerHeal);
	}
}