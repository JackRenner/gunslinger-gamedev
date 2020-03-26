#include <iostream>
#include <cstdlib>
#include "Player.h"
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
	AnimatedSprite::update(pressedKeys);
	controls::update(pressedKeys);
	if (controls::holdW()) {
		this->position.y -= 5;
	}
	if (controls::holdS()) {
		this->position.y += 5;
	}
	if (controls::holdD()) {
		this->position.x += 5;
	}
	if (controls::holdA()) {
		this->position.x -= 5;
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
