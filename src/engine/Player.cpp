#include <iostream>
#include <cstdlib>
#include "Player.h"
#include "AnimatedSprite.h"
#include "Sprite.h"

using namespace std;


Player::Player() : AnimatedSprite("Player"){

	this->type = "Player";
	
	// this->width = 416;
	// this->height = 454;
	// this->scaleX = 0.15;
	// this->scaleY = 0.15;

	this->addAnimation("resources/character/", "Up", 3, 1, true);
	this->addAnimation("resources/character/", "Left", 3, 1, true);
	this->addAnimation("resources/character/", "Right", 3, 1, true);
	this->addAnimation("resources/character/", "Down", 3, 1, true);

	this->play("Up");
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
	if (pressedKeys.find(SDL_SCANCODE_W) != pressedKeys.end()) {
		this->play("Up");
	}
	if (pressedKeys.find(SDL_SCANCODE_S) != pressedKeys.end()) {
		this->play("Down");
	}
	if (pressedKeys.find(SDL_SCANCODE_D) != pressedKeys.end()) {
		this->play("Right");
	}
	if (pressedKeys.find(SDL_SCANCODE_A) != pressedKeys.end()) {
		this->play("Left");
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
