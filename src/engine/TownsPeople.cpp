#include "TownsPeople.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "DisplayObjectContainer.h"
#include "Controls.h"
#include "../main/MyGame.h"
#include <cstdlib>
#include <math.h>
#include <algorithm>

using namespace std;

//Here, "Sayu" is the player character
TownsPeople::TownsPeople(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "TownsPeople";
	this->sayu = sayu;
	//this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
}

void TownsPeople::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	
	if(this->health == 0){
		this->clean = true; //scene will clean it up
	}
	//do the actual cleaning if necessary
	if(this->clean){
		this->removeThis();
	}

	this->scaleX = 0.75;
	this->scaleY = 0.75;
	if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
		if (this->position.x - sayu->position.x > 0) {
			this->play("storekeeperLeft");
		} else {
			this->play("storekeeperRight");
		}
	} else {
		if (this->position.y - sayu->position.y > 0) {
			this->play("storekeeperUp");
		} else {
			this->play("storekeeperDown");
		}
	}


	//everything else controlled by state machine
	//state 0 = one time state to kick things off
	//state 1 = patrolling

	
	if(this->state == 0){
		setPatrolRange();
	}
	else if(this->state == 1){
		patrol();
    }

	//state transitions
	if(this->state == 0){
		this->state = 1;
		this->targX = std::rand()%(this->maxPatX-this->minPatX) + this->minPatX;
		this->targY = std::rand()%(this->maxPatY-this->minPatY) + this->minPatY;
		this->vel = 0;
		this->maxVel = 4;
	}
	else if(this->state == 1){

	}

}


void TownsPeople::onCollision(DisplayObject* other){
	if (other->type == "Player") {
		std::cout << "Show Text" << endl;
		}
	}
	// if(other->type == "Weapon"){
	// 	if(controls::pressSpecial()) 
	// 		onEssenceStrike((Weapon*)other);
	// }
	// else if(other->type == "Blast"){
	// 	if(controls::pressAttack())
	// 		onMeleeStrike();
	// }


SDL_Point* TownsPeople::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width*2, -this->height*2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width*2, -this->height*2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width*2, this->height*2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width*2, this->height*2);
	return this->MyGlobalHitbox;
}

void TownsPeople::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
	//this->drawHitbox(position);
}

void TownsPeople::save(ofstream &out){
	//Sprite::save(out);
	//TODO: ADD THIS TO SAVE TOWNSPEOPLE DATA
}



void TownsPeople::setPatrolRange(){
	this->minPatX = this->position.x-120;
	this->maxPatX = this->position.x+120;
	this->minPatY = this->position.y-120;
	this->maxPatY = this->position.y+120;
}

void TownsPeople::patrol(){
	//if close to target, set a new one
    //TODO CHECK THIS
	if(isTargetReached() && pauseCount == 119){
		this->targX = std::rand()%(this->maxPatX-this->minPatX) + this->minPatX;
		this->targY = std::rand()%(this->maxPatY-this->minPatY) + this->minPatY;
		this->vel = 0;
		this->maxVel = 4;
		this->pauseCount = 0;
	}

	if(pauseCount < 119){
		pauseCount = (pauseCount+1) % 120;
	}else{
		moveToTarget();
	}
}

void TownsPeople::moveToTarget(){

	//increase velocity by accel
	this->vel = std::min(this->vel+this->acc, this->maxVel);

	//use unit vector to determine percent that goes into x and y 
	double theta = atan2(std::abs(this->targY - this->position.y),std::abs(this->targX - this->position.x));
	double xComp = this->vel*cos(theta);
	double yComp = this->vel*sin(theta);
	if(this->targX - this->position.x < 0) xComp *= -1;
	if(this->targY - this->position.y < 0) yComp *= -1;

    this->position.x += xComp;
    this->position.y += yComp;
}

bool TownsPeople::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

