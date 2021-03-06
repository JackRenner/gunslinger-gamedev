#include "KnifeGuy.h"
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
KnifeGuy::KnifeGuy(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "KnifeGuy";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
}

void KnifeGuy::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	
	//enemy is dead so clean it up
	if(this->health == 0){
		this->clean = true; //scene will clean it up
	}
	//do the actual cleaning if necessary
	if(this->clean){
		Scene *temp = (Scene*) this->parent;
		temp->enemiesLeft --;
		this->removeThis();
		return;
	}


	//everything else controlled by state machine
	//state 0 = one time state to kick things off
	//state 1 = patrolling
	//state 2 = engaged
	//state 3 = not in distance
	//state 4 = hiding and reloading
	
	if(this->state == 0){
		setPatrolRange();
	}
	else if(this->state == 1){
		patrol();
	}
	else if(this->state == 2){
		// this->targX = sayu->x;
		// this->targY = sayu->y;
		charge();
	}
	else if(this->state == 3){
		// strike with knife
		slash();
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
		    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
		// if the difference in north/south is greater than east/west
		if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
			if (this->position.x - sayu->position.x > 0) {
				this->play("KnifeGuyLeft");
			} else {
				this->play("KnifeGuyRight");
			}
		} else {
			if (this->position.y - sayu->position.y > 0) {
				this->play("KnifeGuyUp");
			} else {
				this->play("KnifeGuyDown");
			}
		}
		//if player is close, start to prepare charge
		int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
		if(dist<400){
			this->state = 2;
			this->vel = 0;
			this->maxVel = 4;
			this->acc = 0.5;
			this->targX = this->sayu->position.x;
			this->targY = this->sayu->position.y;
			// this->rotVel = 0;
			// this->rotAcc = 0.4;
			// this->maxRotVel = 20;
		}
	}
	else if(this->state == 2){
		    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
		// if the difference in north/south is greater than east/west
		if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
			if (this->position.x - sayu->position.x > 0) {
				this->play("KnifeGuyLeft");
			} else {
				this->play("KnifeGuyRight");
			}
		} else {
			if (this->position.y - sayu->position.y > 0) {
				this->play("KnifeGuyUp");
			} else {
				this->play("KnifeGuyDown");
			}
		}
        int dist = std::max(std::abs(this->position.x-this->targX),std::abs(this->position.y-this->targY));
        if(dist < 20){
			this->state = 3;
			//this->rotation = 0;
			//this->rotVel = 0;
			this->targX = this->position.x;
			this->targY = this->position.y;
			// Sound* new_sound = new Sound();
			// new_sound->playSFX();
			// this->play("Explode");
		}
	}
	this->save();
}

// void Creeper::onEssenceStrike(Weapon* w){

// 	if(this->shield <= 0) this->health -= w->damage;
// 	if(this->health < 0) this->health = 0;
// }

void KnifeGuy::onCollision(DisplayObject* other){
	if (other->type == "Projectile" && other->id != lastId) {
		Projectile *temp = (Projectile*)other;
		if (temp->gun == "revolver") {
			this->health -= 40;
			this->alpha -= 40;
			if(this->health < 0) this->health = 0;
		}else if (temp->gun == "knife" && temp->thrown) {
		} else if(temp->gun == "knife") {
			this->health -= 80;
			this->alpha -= 100;
			if(this->health < 0) this->health = 0;
			sayu->knife_throws = 0;
		} else if (temp->gun == "shotgun") {
			this->health -= 100;
			this->alpha -= 80;
			if(this->health < 0) this->health = 0;
		} else if (temp->gun == "rifle") {
			this->health -= 50;
			this->alpha -= 60;
			if(this->health < 0) this->health = 0;
		}
		lastId = other->id;
	} else if(other->type == "Projectile"){
		lastId = other->id;
	} else if (other->type != "KnifeGuy"){
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
	// } else{
	// 	Game::instance->ourCollisionSystem->resolveCollision(this, other , this->position.x - oldX, this->position.y-oldY, 0, 0);
	// 	this->targX = oldX + rand() % 200 - 100;
	// 	this->targX = oldY + rand() % 200 - 100;
	// 	this->state = 1;
	// }
}

SDL_Point* KnifeGuy::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	delete temp;
	return this->MyGlobalHitbox;
}

void KnifeGuy::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
	//this->drawHitbox(position);
}

void KnifeGuy::save(){
	this->oldX = position.x;
	this->oldY = position.y;
	//Sprite::save(out);
	//TODO: ADD THIS TO SAVE KNIFEGUY DATA
}

void KnifeGuy::charge(){
	//this->rotation += this->rotVel;
	moveToTarget();
}

void KnifeGuy::prepareCharge(){
	// this->rotVel = std::min(this->rotVel+this->rotAcc, this->maxRotVel);
	// this->rotation += this->rotVel;
}

void KnifeGuy::setPatrolRange(){
	this->minPatX = this->position.x-120;
	this->maxPatX = this->position.x+120;
	this->minPatY = this->position.y-120;
	this->maxPatY = this->position.y+120;
}

void KnifeGuy::patrol(){
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

void KnifeGuy::moveToTarget(){
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
	//moveToTarget();
}

bool KnifeGuy::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

void KnifeGuy::slash(){
	if (this->waitToMove > 100) {
		this->state = 1;
		this->waitToMove = 0;
	}
	this->waitToMove ++;
}
