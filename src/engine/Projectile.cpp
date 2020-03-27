#include <iostream>
#include <cstdlib>
#include "Projectile.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Controls.h"

using namespace std;


Projectile::Projectile() : AnimatedSprite("Projectile"){

	this->type = "Projectile";
	
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

Projectile::Projectile(string face, SDL_Point position) : AnimatedSprite("Projectile"){

	this->type = "Projectile";
	
	// this->width = 416;
	// this->height = 454;
	// this->scaleX = 0.15;
	// this->scaleY = 0.15;
    this->addAnimation("resources/character/", "bullet", 1, 1, true);

    this->dir = face;

    
	this->play("bullet");
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


void Projectile::update(set<SDL_Scancode> pressedKeys){
    if(this->dir == "right"){
        this->position.x -= 20;
    }
    if (this->dir == "left") {
        this->position.x += 20;
    }
    if(this->dir == "up") {
        this->position.y -= 20;
    }
    if(this->dir == "down") {
        this->position.y += 20;
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

void Projectile::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}
