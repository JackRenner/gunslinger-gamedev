#include <iostream>
#include <cstdlib>
#include "Player.h"
#include "Projectile.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Controls.h"
#include "CollisionSystem.h"
#include "Benemy.h"

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
	
	this->play("FaceUp");
	holding = 0;
}


void Player::update(set<SDL_Scancode> pressedKeys){
	if (this->poisonedTime > 0 && this->poisoned) {
		this->health -= 5;
		this->poisonedTime -= 1;
	}
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
			this->position.y -= 2;
		}
		if (controls::holdS()) {
			this->dir = "Down";
			this->play("FaceDown");
			this->position.y += 2;
		}
		if (controls::holdD()) {
			this->dir = "Right";
			this->play("FaceRight");
			this->position.x += 2;
		}
		if (controls::holdA()) {
			this->dir = "Left";
			this->play("FaceLeft");
			this->position.x -= 2;
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
		takeDamage(50);
	} else if (gun == "rifle") {
		takeDamage(25);
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

// do not include attacks from bosses yet
void Player::hitByMelee(string enemy){
	if (enemy == "creeper") {
		takeDamage(500);
	} else if (enemy == "wolf") {
		takeDamage(1);
	}
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
	if(this->health > 0){
		this->health -= damage;
		if (this->health < 0) {this->health = 0;}
		dispatchEvent(healthChangeEvent);
	}
}

// void Player::onEnemyCollision(Enemy* enemy){
// 	this->health -= enemy->damage;
// 	this->initIFrames(120);
// }

// void Player::initIFrames(int numFrames){
// 	this->iFrameCount = 0;
// 	this->numIFrames = numFrames;
// 	this->iFrames = true;
// }

void Player::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}

