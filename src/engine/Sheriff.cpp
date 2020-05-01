#include "Sheriff.h"
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
Sheriff::Sheriff(Player* sayu, string id, vector<string> dialogue, bool hurtable) : AnimatedSprite(id){
	this->type = "Sheriff";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;
	this->hurtable = hurtable;

    sheriffText = new TextBox(SDL_Point{ 1500, 500 }, 300, 200, 20, 20, 20, 255);

    for (string line: dialogue) {
        sheriffText->addTextLine("./resources/fonts/west.otf", line, 24, SDL_Color{ 255, 255, 255 });
	    this->addChild(sheriffText);
    }
	sheriffText->position = { -100, -250 };
	sheriffText->initBox();
    sayu->lakeUnlocked = true;
}

void Sheriff::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);

	//enemy is dead so clean it up
	if(this->health == 0){
		this->clean = true; //scene will clean it up
	}
	//do the actual cleaning if necessary
	if(this->clean){
		sayu->killTheTown = true;
		Scene *temp = (Scene*) this->parent;
		this->removeThis();
		return;
	}
	
    if (!this->lastLine && !sayu->badlandsUnlocked && sayu->lakeComplete && !sayu->badlandsComplete && !sayu->hideoutComplete && !sheriffText->textLock) {
        sheriffText->drawNextLine();
        sayu->badlandsUnlocked = true;
    } else if (!this->lastLine && !sayu->hideoutUnlocked && sayu->lakeComplete && sayu->badlandsComplete && !sayu->hideoutComplete && !sheriffText->textLock) {
        sheriffText->drawNextLine();
        sayu->hideoutUnlocked = true;
    } else if (!this->lastLine && !sayu->finalBattleUnlocked && sayu->lakeComplete && sayu->badlandsComplete && sayu->hideoutComplete && !sayu->finalBossDefeated && !sheriffText->textLock) {
        sheriffText->drawNextLine();
		sayu->finalBattleUnlocked = true;
    }

    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
    // if the difference in north/south is greater than east/west
    if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
        if (this->position.x - sayu->position.x > 0) {
            this->play("SheriffLeft");
        } else {
            this->play("SheriffRight");
        }
    } else {
        if (this->position.y - sayu->position.y > 0) {
            this->play("SheriffUp");
        } else {
            this->play("SheriffDown");
        }
    }
	//everything else controlled by state machine
	//state 0 = sheriff will always be still

	// STATES 2 AND 3 WILL NOT BE USED UNTIL THE FINAL BATTLE
    //state 2 = engaged
	//state 3 = not in distance
	
	if(this->state == 0){
		// literally do nothing
	}
	else if(this->state == 2){
		fire();
	}
	else if(this->state == 3){
		charge();
		this->targX = this->sayu->position.x + (rand() % 400 + 100) - 300;
		this->targY = this->sayu->position.y + (rand() % 400 + 100) - 300;
	}

	//state transitions
	if(this->state == 0){

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
	}
	this->save();
}


void Sheriff::onCollision(DisplayObject* other){
	if (other->type == "Projectile" && other->id != lastId && this->hurtable) {
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
	} else if(other->type == "Projectile" && this->hurtable) {
		lastId = other->id;	
	} else if (other->type == "Obstacle" || other->type == "Cactus") {
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

SDL_Point* Sheriff::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	delete temp;
	return this->MyGlobalHitbox;
}

void Sheriff::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}

void Sheriff::save(){
	this->oldX = position.x;
	this->oldY = position.y;	//TODO: ADD THIS TO SAVE Sheriff DATA
}

void Sheriff::charge(){
	moveToTarget();
}

void Sheriff::moveToTarget(){

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

bool Sheriff::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

int Sheriff::fire() {
    // shoot six shots at a time with pauses between
	this->shoot += 1;
	return shoot;
}

