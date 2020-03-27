#include <iostream>
#include <cstdlib>
#include "Projectile.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Controls.h"

using namespace std;


Projectile::Projectile() : AnimatedSprite("Projectile"){

	this->type = "Projectile";

}

Projectile::Projectile(string face, SDL_Point position, int type) : AnimatedSprite("Projectile"){

	this->type = "Projectile";
	
	// this->width = 416;
	// this->height = 454;
	// this->scaleX = 0.15;
	// this->scaleY = 0.15;
	// no gun
	if (type == 0){
		this->addAnimation("resources/character/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 0;
		
	}
	//knife
	if(type == 1){
		this->addAnimation("resources/character/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 150;
		this->speed = 4;
		
	}
	//pistol
	if(type == 2){
		this->addAnimation("resources/character/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 150;
		this->speed = 8;
		 
	}
	//shotgun
	if(type == 3){
 		this->dir = face;
		if(this->dir == "left" || "right"){
			this->addAnimation("resources/character/", "shotgun", 1, 1, true);
			this->play("shotgun");
			//wont let shotgunright - up
		}
		else {
			this->addAnimation("resources/character/", "shotgun", 1, 1, true);
			this->play("shotgun");
			//should be shotgunup
		}
		this->Distance = 120;
		this->speed = 5;
	}
	//rifle
	if (type == 4){
		this->addAnimation("resources/character/", "bullet", 1, 1, true);
 		this->dir = face;
		this->play("bullet");
		this->Distance = 350;
		this->speed = 15;
		
	}
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
		this->removeThis();
	}
	
	

}

void Projectile::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}
