#include "Benemy.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "DisplayObjectContainer.h"
#include <cstdlib>
#include <math.h>
#include <algorithm>

using namespace std;

//Here, "Sayu" is the player character
Benemy::Benemy(AnimatedSprite* sayu, int x, int y, int velocity, string weapon, string id) : AnimatedSprite(id){
	this->type = "Benemy";
	this->sayu = sayu;
	this->targX = x;
	this->source = weapon;
	this->addAnimation("resources/weapons/", weapon, 1, 1, true);
	this->play(weapon);

	this->targY = y;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
	this->vel = velocity;
	this->initial = sayu->position;
}

void Benemy::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	if (clean){
		delete this;
	}
	if(isTargetReached()){
		clean = true;
		AnimatedSprite::update(pressedKeys);
		this->removeThis();
	} else {
		fire();
	}
}
	

void Benemy::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}

void Benemy::onCollision(DisplayObject *other) {
	this->removeThis();
	this->clean = true;
	//delete this;
}

SDL_Point* Benemy::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(0, 0);
	this->MyGlobalHitbox[1] = temp->transformPoint(40, 0);
	this->MyGlobalHitbox[2] = temp->transformPoint(0, 40);
	this->MyGlobalHitbox[3] = temp->transformPoint(40, 40);
	return this->MyGlobalHitbox;
}

void Benemy::save(ofstream &out){
	//Sprite::save(out);
	//TODO: ADD THIS TO SAVE Wolf DATA
}



void Benemy::fire(){
	
    //std::cout << "CURRENT POSITION: " << this->position.x << " " << this->position.y <<"\n";
    //std::cout << "TARGET POSITION: " << this->targX << " " << this->targY << "\n";
	//increase velocity by accel
	this->vel = std::min(this->vel, this->maxVel);

	//use unit vector to determine percent that goes into x and y 
	theta = atan2(std::abs(this->targY - this->position.y),std::abs(this->targX - this->position.x));
	double xComp = this->vel*cos(theta);
	double yComp = this->vel*sin(theta);
	if(this->targX - this->position.x < 0) xComp *= -1;
	if(this->targY - this->position.y < 0) yComp *= -1;

    this->position.x += xComp;
    this->position.y += yComp;

	if (timer % 5 == 0) {
		this->targX += xComp;
		this->targY += yComp;
	}
	timer++;
}

bool Benemy::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}