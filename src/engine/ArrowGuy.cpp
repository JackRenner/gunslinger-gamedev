#include "ArrowGuy.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Benemy.h"
#include "Sprite.h"
#include "DisplayObjectContainer.h"
#include "Controls.h"
#include <cstdlib>
#include <math.h>

using namespace std;

//Here, "Sayu" is the player character
ArrowGuy::ArrowGuy(Player* sayu) : AnimatedSprite("ArrowGuy"){
	this->type = "ArrowGuy";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
}

void ArrowGuy::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	
	//std::cout << sayu->position.x << " " << sayu->position.y << "\n";
	//enemy is dead so clean it up
	if(this->health == 0){
		this->clean = true; //scene will clean it up
	}
	//do the actual cleaning if necessary
	if(this->clean){
		this->removeThis();
		delete this;
	}

    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
    // if the difference in north/south is greater than east/west
    if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
        if (this->position.x - sayu->position.x > 0) {
            this->play("ArrowLeft");
        } else {
            this->play("ArrowRight");
        }
    } else {
        if (this->position.y - sayu->position.y > 0) {
            this->play("ArrowUp");
        } else {
            this->play("ArrowDown");
        }
    }
	//everything else controlled by state machine
	//state 0 = one time state to kick things off
	//state 1 = patrolling
	//state 2 = readying a charge
	//state 3 = charging
	//state 4 = post-charge movement
	//state 5 = stunned
	
	if(this->state == 0){
        //std::cout << "init\n";
		setPatrolRange();
	}
	else if(this->state == 1){
        //std::cout << "patrolling\n";
		patrol();
	}
	else if(this->state == 4){
        //std::cout << "post charging\n";		
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
	else if(this->state == 2){
		//if(abs(this->rotVel - this->maxRotVel) < 1){
			this->state = 3;
			this->targX = this->sayu->position.x;
			this->targY = this->sayu->position.y;
		//}
	}
	else if(this->state == 3){
		if(isTargetReached()){
			this->state = 4;
			this->rotation = 0;
			this->rotVel = 0;
			this->targX = this->position.x;
			this->targY = this->position.y - 100;
		}
	}
	else if(this->state == 4){
		if(isTargetReached()){
			this->state = 1;
			this->setPatrolRange();
		}
	}
}

void ArrowGuy::onMeleeStrike(){
	this->health -= 10;
	if(this->health < 0) this->health = 0;
}

// void ArrowGuy::onEssenceStrike(Weapon* w){

// 	if(this->shield <= 0) this->health -= w->damage;
// 	if(this->health < 0) this->health = 0;
// }

// void ArrowGuy::onCollision(DisplayObject* other){
// 	if(other->type == "Weapon"){
// 		if(controls::pressSpecial()) 
// 			onEssenceStrike((Weapon*)other);
// 	}
// 	else if(other->type == "Blast"){
// 		if(controls::pressAttack())
// 			onMeleeStrike();
// 	}
// }

void ArrowGuy::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
	//this->drawHitbox();
}

void ArrowGuy::save(ofstream &out){
	//Sprite::save(out);
	//TODO: ADD THIS TO SAVE ArrowGuy DATA
}


void ArrowGuy::setPatrolRange(){
	this->minPatX = this->position.x-120;
	this->maxPatX = this->position.x+120;
	this->minPatY = this->position.y-120;
	this->maxPatY = this->position.y+120;
}

void ArrowGuy::patrol(){
	//if close to target, set a new one
	
	if(isTargetReached() && pauseCount == 119){
		this->targX = std::rand()%(this->maxPatX-this->minPatX) + this->minPatX;
		this->targY = std::rand()%(this->maxPatY-this->minPatY) + this->minPatY;
		this->vel = 0;
		this->maxVel = 4;
		this->pauseCount = 0;
		
	}

	if(pauseCount < 119){
		pauseCount = (pauseCount+1) % 120;
	
	}
	else{
		moveToTarget();
	}
}

void ArrowGuy::moveToTarget(){

	//increase velocity by accel
	this->vel = std::min(this->vel+this->acc, this->maxVel);

	//use unit vector to determine percent that goes into x and y 
	double theta = atan2(std::abs(this->targY - this->position.y),std::abs(this->targX - this->position.x));
	double xComp = this->vel*cos(theta);
	double yComp = this->vel*sin(theta);
	if(this->targX - this->position.x < 0) xComp *= -1;
	if(this->targY - this->position.y < 0) yComp *= -1;

    //this->position.x += xComp;
    //this->position.y += yComp;
	fire();
}

bool ArrowGuy::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

int ArrowGuy::fire() {
	this->shoot += 1;
	return shoot;
}
