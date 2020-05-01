#include "ShotgunGuy.h"
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
ShotgunGuy::ShotgunGuy(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "ShotgunGuy";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;

	shot_sound = new Sound("./resources/sfx/double_barrel_shot.wav");
	explosion = new Sound("./resources/sfx/DeathFlash.wav");
}

void ShotgunGuy::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);

	cout << "HEALTH LEFT: " << this->health << endl;
	cout << "LAST TWO ID: " << this->lastTwoId << endl;
	cout << "LAST ID: " << this->lastId << endl;
	
	//enemy is dead so clean it up
	if(this->health == 0){
		this->clean = true; //scene will clean it up
	}
	//do the actual cleaning if necessary
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
            this->play("ShotgunGuyLeft");
        } else {
            this->play("ShotgunGuyRight");
        }
    } else {
        if (this->position.y - sayu->position.y > 0) {
            this->play("ShotgunGuyUp");
        } else {
            this->play("ShotgunGuyDown");
        }
    }
		//everything else controlled by state machine
	//state 0 = one time state to kick things off
	//state 1 = more preparation
	//state 2 = at rest shooting at player
	//state 3 = moving towards player shooting at player
    //state 4 = reload and throw dynamite
    //state 5 = smoke bomb and relocate
	
	if(this->state == 0){
        cout << "init state" << endl;
		
	}
	else if(this->state == 1){
        cout << "state 1" << endl;
		
	}
	else if(this->state == 2){
        cout << "state 2" << endl;
        fire();
	}
	else if(this->state == 3){
        cout << "state 3" << endl;
        // state where he moves towards player
		charge();
		this->targX = this->sayu->position.x;
		this->targY = this->sayu->position.y;
	}
    else if(this->state == 4){
        cout << "state 4" << endl;
        // charge();
		// this->targX = this->sayu->position.x + rand() % 100 + 100; 
		// this->targY = this->sayu->position.y + rand() % 100 + 100;
        this->waitToReload ++;
    }
    else if(this->state == 5){
        smokeBomb();
    }

	//state transitions
	if(this->state == 0){
		this->state = 1;
	}
	else if(this->state == 1){
		// Prepare to move towards player (the boss won't stay in this state long)
        // it is here that we can change what his speed will be
        this->state = 2;
        this->vel = 0;
        this->maxVel = 5;
        this->acc = 0.5;
	}
	else if(this->state == 2){
        // this is the state where he doesn't move and just shoots
        // maybe figure out a way to get him to rotate around player??
        int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
        if (shots_fired == 2) {
            this->state = 4;
            shots_fired = 0;
        }
        else if (dist > 200) {
            this->state = 3;
            this->targX = this->sayu->position.x;
			this->targY = this->sayu->position.y;
            shots_fired = 0;
        }
	}
	else if(this->state == 3){
        int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
        // if (shots_fired == 2) {
        //     this->state = 4;
        //     shots_fired = 0;
        //     this->shoot = 0;
        // }
        if(dist < 200){
			this->state = 2;
			this->rotation = 0;
			this->rotVel = 0;
			this->targX = this->position.x;
			this->targY = this->position.y;
		}
	}
    else if(this->state == 4){
        if (this->waitToReload == 150) {
            this->dynamite = true;
        }
        if (this->waitToReload == 200) {        
            this->dynamite = false;  
            this->state = 2;
            this->shots_fired = 0;
            this->waitToReload = 0;
        }
    }
    else if(this->state == 5){
        this->state = 2;
    }

    if (this->waitToSmokeTimer == this->waitToSmoke) {
        this->state = 5;
        this->waitToSmoke = rand() % 200 + 200;
        this->waitToSmokeTimer = 0;
    }
    this->waitToSmokeTimer ++;
	
	this->save();
}

void ShotgunGuy::onCollision(DisplayObject* other){
	if (other->type == "Projectile" && other->id != lastId && other->id != lastTwoId && other->id != lastThreeId && other->id != lastFourId && other->id != lastFiveId && other->id != lastSixId) {
		Projectile *temp = (Projectile*)other;
		if (temp->gun == "revolver") {
			this->health -= 20;
			this->alpha -= 5;
			if(this->health < 0) this->health = 0;
		}else if (temp->gun == "knife" && temp->thrown) {
		} else if(temp->gun == "knife") {
			this->health -= 50;
			this->alpha -= 5;
			if(this->health < 0) this->health = 0;
			sayu->knife_throws = 0;
		} else if (temp->gun == "shotgun") {
			this->health -= 40;
			this->alpha -= 5;
			if(this->health < 0) this->health = 0;
		} else if (temp->gun == "rifle") {
			this->health -= 30;
			this->alpha -= 5;
			if(this->health < 0) this->health = 0;
		}
		if (lastTwoId != other->id && lastId != other->id && lastThreeId != other->id && lastFourId != other->id && lastFiveId != other->id && lastSixId != other->id) {
			lastSixId = lastFiveId;
			lastFiveId = lastFourId;
			lastFourId = lastThreeId;
			lastThreeId = lastTwoId;
			lastTwoId = lastId;
			lastId = other->id;
		}
	} else if(other->type == "Projectile"){
		if (lastTwoId != other->id && lastId != other->id && lastThreeId != other->id && lastFourId != other->id && lastFiveId != other->id && lastSixId != other->id) {
			lastSixId = lastFiveId;
			lastFiveId = lastFourId;
			lastFourId = lastThreeId;
			lastThreeId = lastTwoId;
			lastTwoId = lastId;
			lastId = other->id;
		}
	}else{
		Game::instance->ourCollisionSystem->resolveCollision(this, other , this->position.x - oldX, this->position.y-oldY, 0, 0);
		if (abs(this->position.x - other->position.x) > abs(this->position.y - other->position.y)) {
			if (this->position.x - other->position.x > 0) {
				this->targX = this->position.x - 100;
			} else {
				this->targX = this->position.x + 100;
			}
		} else {
			if (this->position.y - other->position.y > 0) {
				this->targY = this->position.y - 100;
			} else {
				this->targY = this->position.y + 100;
			}
		}
	}
}

SDL_Point* ShotgunGuy::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	return this->MyGlobalHitbox;
}

void ShotgunGuy::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}

void ShotgunGuy::save(){
	this->oldX = position.x;
	this->oldY = position.y;
    this->oldAlpha = alpha;
}

void ShotgunGuy::charge(){
	moveToTarget();
}

void ShotgunGuy::moveToTarget(){

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

bool ShotgunGuy::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

int ShotgunGuy::fire() {
	this->shoot += 1;
	return shoot;
}

void ShotgunGuy::smokeBomb() {
    this->play("smoke");
    this->waitToSmoke = 500;
    while (this->waitToSmokeTimer < this->waitToSmoke) {
        this->waitToSmokeTimer++;
    }
    this->waitToSmokeTimer = 0;
    this->waitToSmoke = rand() % 500 + 250;
    this->position.x = rand() % 1000;
    this->position.y = rand() % 700 + 300;
    //this->stop();
    //this->alpha = this->oldAlpha;
}

