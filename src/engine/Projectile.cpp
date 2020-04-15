#include <iostream>
#include <cstdlib>
#include "Projectile.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Controls.h"
#include "Game.h"

using namespace std;


Projectile::Projectile() : AnimatedSprite("Projectile"){

	this->type = "Projectile";

}

Projectile::Projectile(string face, SDL_Point position, int type) : AnimatedSprite("Projectile"){

	this->type = "Projectile";

	// no gun
	if (type == 0){
		this->addAnimation("resources/weapons/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 0;
	}
	//knife
	if(type == 1){
		this->addAnimation("resources/weapons/", "knife", 1, 1, true);
		this->width = 30;
		this->height = 30;
 		this->dir = face;
		this->play("knife");
		this->Distance = 150;
		this->speed = 4;
		this->gun = "knife";
	}
	//pistol
	if(type == 2){
		this->addAnimation("./resources/weapons/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 300;
		this->speed = 8;
		this->gun = "revolver";

	}
	//shotgun
	if(type == 3){
 		this->dir = face;
		if(this->dir == "left" || "right"){
			this->addAnimation("./resources/weapons/", "shotgun", 1, 1, true);
			this->play("shotgun");
			//wont let shotgunright - up
		}
		else {
			this->addAnimation("./resources/weapons/", "shotgun", 1, 1, true);
			this->play("shotgun");
			//should be shotgunup
		}
		this->Distance = 120;
		this->speed = 10;
		this->gun = "shotgun";
	}
	//rifle
	if (type == 4){
		this->addAnimation("./resources/weapons/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 350;
		this->speed = 15;
		this->gun = "rifle";
		
	}
}

void Projectile::onCollision(DisplayObject* other) {
	if (this->thrown && other->type=="Player") 
		this->removeThis();
	else if (other->type!="Player")
		this->removeThis();
}

SDL_Point* Projectile::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(0,0);
	this->MyGlobalHitbox[1] = temp->transformPoint(10, 0);
	this->MyGlobalHitbox[2] = temp->transformPoint(0, 10);
	this->MyGlobalHitbox[3] = temp->transformPoint(10, 10);
	return this->MyGlobalHitbox;
}

void Projectile::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	controls::update(pressedKeys);

    if(this->dir == "right"){
        this->position.x -= this->speed;
		this->durability +=this->speed;
    }
    if (this->dir == "left") {
        this->position.x += this->speed;
		this->durability +=this->speed;
    }
    if(this->dir == "up") {
        this->position.y -= this->speed;
		this->durability +=this->speed;
    }
    if(this->dir == "down") {
        this->position.y += this->speed;
		this->durability +=this->speed;
    }
	if(this->durability > this->Distance) {
		if(this->Distance == 150) {
			this->thrown = true;
			this->speed = 0;
			this->position = this->position;
		}else{
			this->removeThis();
		}
	}
	//Game::instance->ourCollisionSystem->collidesWith()
}

void Projectile::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}
