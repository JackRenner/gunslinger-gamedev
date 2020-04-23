#include "GangThug.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Benemy.h"
#include "Sprite.h"
#include "DisplayObjectContainer.h"
#include "Controls.h"
#include "../main/MyGame.h"
#include <cstdlib>
#include <math.h>
#include <algorithm>

using namespace std;

//Here, "Sayu" is the player character
GangThug::GangThug(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "GangThug";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
}

void GangThug::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	
	//std::cout << sayu->position.x << " " << sayu->position.y << "\n";

	//do the actual cleaning if necessary
	if(this->clean){
		std::cout << "removing" << endl;
		//MyGame::gang_thugs.erase(this);
		this->removeThis();
		std::cout << "wait what" << endl;
		//delete this;
	}
	//enemy is dead so clean it up
	// if(this->health == 0){
	// 	//this->clean = true; //scene will clean it up
	// 	AnimatedSprite::update(pressedKeys);
	// }

    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
    // if the difference in north/south is greater than east/west
    if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
        if (this->position.x - sayu->position.x > 0) {
            this->play("GangThugLeft");
        } else {
            this->play("GangThugRight");
        }
    } else {
        if (this->position.y - sayu->position.y > 0) {
            this->play("GangThugUp");
        } else {
            this->play("GangThugDown");
        }
    }
	//everything else controlled by state machine
	//state 0 = one time state to kick things off
	//state 1 = patrolling
	//state 2 = engaged
	//state 3 = not in distance
	
	if(this->state == 0){
		setPatrolRange();
	}
	else if(this->state == 1){
		patrol();
	}
	else if(this->state == 2){
		cout << "fire state" << endl;
		fire();
	}
	else if(this->state == 3){
		cout << "charge state" << endl;
		charge();
		this->targX = this->sayu->position.x;
		this->targY = this->sayu->position.y;
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
		//if player is close, start to prepare charge
		int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
		if(dist<300){
			this->state = 2;
			this->vel = 0;
			this->maxVel = 4;
			this->acc = 0.5;
			this->rotVel = 0;
			this->rotAcc = 0.4;
			this->maxRotVel = 20;
		}
	}
	else if(this->state == 2){
        int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
        if (dist > 200) {
            this->state = 3;
            this->targX = this->sayu->position.x + (rand() % 400 + 100) - 300;
			this->targY = this->sayu->position.y + (rand() % 400 + 100) - 300;
        }
		// //if(abs(this->rotVel - this->maxRotVel) < 1){
		// 	this->state = 3;
		// 	this->targX = this->sayu->position.x;
		// 	this->targY = this->sayu->position.y;
		// //}
	}
	else if(this->state == 3){
        int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
        if(dist < 200){
			this->state = 2;
			this->rotation = 0;
			this->rotVel = 0;
			this->targX = this->position.x;
			this->targY = this->position.y;
		}
		std::cout << dist << endl;
	}
	this->save();
}

void GangThug::onMeleeStrike(){
	this->health -= 10;
	if(this->health < 0) this->health = 0;
}

// void GangThug::onEssenceStrike(Weapon* w){

// 	if(this->shield <= 0) this->health -= w->damage;
// 	if(this->health < 0) this->health = 0;
// }

void GangThug::onCollision(DisplayObject* other){
	if (other->type == "Projectile" && other->id != lastId) {
		Projectile *temp = (Projectile*)other;
		if (temp->gun == "revolver") {
			this->health -= 20;
			this->alpha -= 40;
			if(this->health < 0) this->health = 0;
		}else if (temp->gun == "knife" && temp->thrown) {
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
	} else if(other->type == "Projectile") {
		lastId = other->id;	
	} else if(other->type == "River") {
		Game::instance->ourCollisionSystem->resolveCollision(this, other , this->position.x - oldX, this->position.y-oldY, 0, 0);
		this->targX = oldX;
		this->targY = oldY - 10;
	} else {
		Game::instance->ourCollisionSystem->resolveCollision(this, other , this->position.x - oldX, this->position.y-oldY, 0, 0);
		this->targX = oldX + rand() % 200 - 100;
		this->targY = oldY + rand() % 200 - 100;
	}
	// if(other->type == "Weapon"){
	// 	if(controls::pressSpecial()) 
	// 		onEssenceStrike((Weapon*)other);
	// }
	// else if(other->type == "Blast"){
	// 	if(controls::pressAttack())
	// 		onMeleeStrike();
	// }
}

SDL_Point* GangThug::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	return this->MyGlobalHitbox;
}

void GangThug::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
	//this->drawHitbox(position);
}

void GangThug::save(){
	this->oldX = position.x;
	this->oldY = position.y;	//TODO: ADD THIS TO SAVE GangThug DATA
}

void GangThug::charge(){
	//this->rotation += this->rotVel;
	moveToTarget();
}

void GangThug::prepareCharge(){
	// this->rotVel = std::min(this->rotVel+this->rotAcc, this->maxRotVel);
	// this->rotation += this->rotVel;
}

void GangThug::setPatrolRange(){
	this->minPatX = this->position.x-120;
	this->maxPatX = this->position.x+120;
	this->minPatY = this->position.y-120;
	this->maxPatY = this->position.y+120;
}

void GangThug::patrol(){
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

void GangThug::moveToTarget(){

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

bool GangThug::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

int GangThug::fire() {
    // shoot six shots at a time with pauses between
	this->shoot += 1;
	return shoot;
}

