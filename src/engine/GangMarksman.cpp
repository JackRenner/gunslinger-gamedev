#include "GangMarksman.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Benemy.h"
#include "Sprite.h"
#include "DisplayObjectContainer.h"
#include "Controls.h"
#include <cstdlib>
#include <math.h>
#include "Game.h"
#include "Scene.h"
#include <algorithm>

using namespace std;

//Here, "Sayu" is the player character
GangMarksman::GangMarksman(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "GangMarksman";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;

	shot_sound = new Sound("./resources/sfx/rifle_shot.wav");
}

void GangMarksman::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);

	// remove from game tree
	if(this->clean){
		Scene *temp = (Scene*) this->parent;
		temp->enemiesLeft --;
		this->removed = true;
		this->removeThis();
		return;
	}

    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
    // if the difference in north/south is greater than east/west
    if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
        if (this->position.x - sayu->position.x > 0) {
            this->play("GangMarksmanLeft");
        } else {
            this->play("GangMarksmanRight");
        }
    } else {
        if (this->position.y - sayu->position.y > 0) {
            this->play("GangMarksmanUp");
        } else {
            this->play("GangMarksmanDown");
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
		setPatrolRange();
	}
	else if(this->state == 1){
		patrol();
	}
	else if(this->state == 4){
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
	this->save();
}

void GangMarksman::onCollision(DisplayObject* other){
	if (other->type == "Projectile" && other->id != lastId) {
		Projectile *temp = (Projectile*)other;
		if (temp->gun == "revolver") {
			this->health -= 20;
			this->alpha -= 40;
			if(this->health < 0) this->health = 0;
		} else if (temp->gun == "knife" && temp->thrown) {
		} else if(temp->gun == "knife") {
			this->health -= 50;
			this->alpha -= 100;
			if(this->health < 0) this->health = 0;
			sayu->knife_throws = 0;
		} else if (temp->gun == "shotgun") {
			this->health -= 40;
			this->alpha -= 80;
			if(this->health < 0) this->health = 0;
		} else if (temp->gun == "rifle") {
			this->health -= 30;
			this->alpha -= 60;
			if(this->health < 0) this->health = 0;
		}
		lastId = other->id;
	} else if(other->type == "Projectile"){
		lastId = other->id;
	} else if (other->type != "GangMarksman"){
		Game::instance->ourCollisionSystem->resolveCollision(this, other , this->position.x - this->oldX, this->position.y - this->oldY, 0, 0);
		if (abs(this->position.x - this->oldX) > abs(this->position.y - this->oldY)) {
			if (this->position.x - this->oldX > 0) {
				this->targX = this->oldX - 50;
			} else {
				this->targX = this->oldX + 50;
			}
		} else {
			if (this->position.y - this->oldY > 0) {
				this->targY = this->oldY - 50;
			} else {
				this->targY = this->oldY + 50;
			}
		}
	}
}

SDL_Point* GangMarksman::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	delete temp;
	return this->MyGlobalHitbox;
}

void GangMarksman::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
	//this->drawHitbox();
}

void GangMarksman::save(){
	this->oldX = position.x;
	this->oldY = position.y;
	//Sprite::save(out);
	//TODO: ADD THIS TO SAVE GangMarksman DATA
}


void GangMarksman::setPatrolRange(){
	this->minPatX = this->position.x-120;
	this->maxPatX = this->position.x+120;
	this->minPatY = this->position.y-120;
	this->maxPatY = this->position.y+120;
}

void GangMarksman::patrol(){
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
	fire();
}

void GangMarksman::moveToTarget(){

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
	fire();
}

bool GangMarksman::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

int GangMarksman::fire() {
	this->shoot += 1;
	return shoot;
}
