#include "Benemy.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "DisplayObjectContainer.h"
#include <cstdlib>
#include <math.h>

using namespace std;

//Here, "Sayu" is the player character
Benemy::Benemy(AnimatedSprite* sayu, int x, int y, int velocity, string weapon) : AnimatedSprite("Benemy"){
	this->type = "Benemy";
	this->sayu = sayu;
	this->targX = x;
	this->addAnimation("resources/weapons/", weapon, 1, 1, true);
	this->play(weapon);
	// this->width = 100;
	// this->height = 100;
	this->targY = y;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
	this->vel = velocity;
	this->initial = sayu->position;
}

void Benemy::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	fire();
	if(isTargetReached()){
		clean = true;
		AnimatedSprite::update(pressedKeys);
		this->removeThis();
	}
	// if(!distanceReached()){
	// 	this->removeThis();
	// }
	
}
	

void Benemy::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
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
	double theta = atan2(std::abs(this->targY - this->position.y),std::abs(this->targX - this->position.x));
	double xComp = this->vel*cos(theta);
	double yComp = this->vel*sin(theta);
	if(this->targX - this->position.x < 0) xComp *= -1;
	if(this->targY - this->position.y < 0) yComp *= -1;

    this->position.x += xComp;
    this->position.y += yComp;
}

bool Benemy::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

bool Benemy::distanceReached(){
	double distancex = (this->initial.x - this->position.x)*(this->initial.x - this->position.x);
	double distancey = (this->initial.y - this->position.y)*(this->initial.y - this->position.y);
	double distanceF = sqrt(distancex - distancey);
	if(this->distance <= distanceF) {
		return true;
	}
	return false;
}