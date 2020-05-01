#include "Creeper.h"
#include "../main/MyGame.h"
#include <cstdlib>
#include <math.h>
#include <algorithm>

using namespace std;

//Here, "Sayu" is the player character
Creeper::Creeper(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "Creeper";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
	explosion = new Sound("./resources/sfx/DeathFlash.wav");
}

void Creeper::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);
	
	//std::cout << sayu->position.x << " " << sayu->position.y << "\n";

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
		std::cout << "setting state 1" << endl;
		patrol();
	}
	else if(this->state == 2){
        std::cout << "charging\n";
		// this->targX = sayu->x;
		// this->targY = sayu->y;
		charge();
	}
	else if(this->state == 3){
		std::cout << "explode" << endl;
		// blow up
		explode();
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
				this->play("CreeperLeft");
			} else {
				this->play("CreeperRight");
			}
		} else {
			if (this->position.y - sayu->position.y > 0) {
				this->play("CreeperUp");
			} else {
				this->play("CreeperDown");
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
				this->play("CreeperLeft");
			} else {
				this->play("CreeperRight");
			}
		} else {
			if (this->position.y - sayu->position.y > 0) {
				this->play("CreeperUp");
			} else {
				this->play("CreeperDown");
			}
		}
        int dist = std::max(std::abs(this->position.x-this->targX),std::abs(this->position.y-this->targY));
        if(dist < 100){
			this->state = 3;
			//this->rotation = 0;
			//this->rotVel = 0;
			this->targX = this->position.x;
			this->targY = this->position.y;
			Sound* explosion = new Sound("./resources/sfx/DeathFlash.wav");
			explosion->playSFX();
			this->play("Explode");
		}
	}
}

// void Creeper::onEssenceStrike(Weapon* w){

// 	if(this->shield <= 0) this->health -= w->damage;
// 	if(this->health < 0) this->health = 0;
// }

void Creeper::onCollision(DisplayObject* other){
	if (other->type == "Projectile") {
		Projectile *temp = (Projectile*)other;
		if (temp->gun == "knife" && temp->thrown) {
		}else{
			this->health = 0;
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
}

SDL_Point* Creeper::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	delete temp;
	return this->MyGlobalHitbox;
}

void Creeper::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
	//this->drawHitbox(position);
}

void Creeper::save(ofstream &out){
	//Sprite::save(out);
	//TODO: ADD THIS TO SAVE Creeper DATA
}

void Creeper::charge(){
	//this->rotation += this->rotVel;
	moveToTarget();
}

void Creeper::prepareCharge(){
	// this->rotVel = std::min(this->rotVel+this->rotAcc, this->maxRotVel);
	// this->rotation += this->rotVel;
}

void Creeper::setPatrolRange(){
	this->minPatX = this->position.x-120;
	this->maxPatX = this->position.x+120;
	this->minPatY = this->position.y-120;
	this->maxPatY = this->position.y+120;
}

void Creeper::patrol(){
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

void Creeper::moveToTarget(){
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

bool Creeper::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

void Creeper::explode(){
	explosion->playSFX();
	if (this->waitToDelete > 100)
		this->health = 0;
	this->waitToDelete ++;
}
