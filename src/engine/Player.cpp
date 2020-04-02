#include <iostream>
#include <cstdlib>
#include "Player.h"
#include "Projectile.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Controls.h"

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
	
	this->play("FaceUp");
	holding = 0;
}

//Called automatically by collision system when something collides with the player
//our job is to simply react to that collision.
// void Player::onCollision(DisplayObject* other){
// 	if(other->type == "Platform"){
// 		Game::instance->collisionSystem.resolveCollision(this, other, this->x - oldX, this->y - oldY);	
// 		_yVel = 0;
// 		_standing=true;
// 	}
// 	else if(other->type == "Enemy"){
// 		if(!this->iFrames){
// 			this->onEnemyCollision((Enemy*)other);
// 		}
// 	}

// }


void Player::update(set<SDL_Scancode> pressedKeys){
	if (this->poisonedTime > 0) {
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


	if (controls::holdW()) {
		this->position.y -= 8;
	}
	if (controls::holdS()) {
		this->position.y += 8;
	}
	if (controls::holdD()) {
		this->position.x += 8;
	}
	if (controls::holdA()) {
		this->position.x -= 8;
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

// do not include attacks from bosses yet
void Player::hitByProjectile(string gun){
	if (gun == "poison") {
		this->poisoned = true;
		this->poisonedTime = 10;
	} else if (gun == "bow") {
		this->health -= 10;
	} else if (gun == "revolver") {
		this->health -= 15;
	} else if (gun == "shotgun") {
		this->health -= 25;
	} else if (gun == "rifle") {
		this->health -= 20;
	}
}

// do not include attacks from bosses yet
void Player::hitByMelee(string enemy){
	if (enemy == "scorpion") {
		this->health -= 5;
	} else if (enemy == "wolf") {
		this->health -= 5;
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
