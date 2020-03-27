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

void Projectile::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	controls::update(pressedKeys);
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

void Projectile::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}
