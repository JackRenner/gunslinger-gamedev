#include "GangLeader.h"
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
GangLeader::GangLeader(Player* sayu, string id) : AnimatedSprite(id){
	this->type = "GangLeader";
	this->sayu = sayu;
	this->width = 80; this->height = 100;
	this->pivot.x = this->width/2;
	this->pivot.y = this->height/2;

	leaderText = new TextBox(SDL_Point{ 1500, 500 }, 300, 100, 220, 220, 220, 100);
	vector<string> dialogue = {"", "Your knifes will no longer work on me, gunslinger!", "", "Don't you know where you're from? You were one of us!!!", "", "And you act like we are scum, when you are the worst of us."};
    for (string line: dialogue) {
        leaderText->addTextLine("./resources/fonts/west.otf", line, 24, SDL_Color{ 73, 43, 196 });
	    this->addChild(leaderText);
    }
	leaderText->position = { -100, -250 };
	leaderText->background->alpha = 0;
	leaderText->initBox();
}

void GangLeader::update(set<SDL_Scancode> pressedKeys){
	AnimatedSprite::update(pressedKeys);

    cout << "HEALTH: " << this->health << endl;
	
	//do the actual cleaning if necessary
	if(this->clean){
		Scene *temp = (Scene*) this->parent;
		temp->enemiesLeft --;
		this->removed = true;
		this->removeThis();
	}


    // ENSURE ENEMIES FACE THE CORRECT DIRECTION //
    // if the difference in north/south is greater than east/west
    if (abs(this->position.x - sayu->position.x) > abs(this->position.y - sayu->position.y)) {
        if (this->position.x - sayu->position.x > 0) {
            this->play("GangLeaderLeft");
        } else {
            this->play("GangLeaderRight");
        }
    } else {
        if (this->position.y - sayu->position.y > 0) {
            this->play("GangLeaderUp");
        } else {
            this->play("GangLeaderDown");
        }
    }
		//everything else controlled by state machine
	//state 0 = one time state to kick things off
	//state 1 = more preparation
	//state 2 = at rest shooting at player
	//state 3 = moving towards player shooting at player
    //state 4 = reload and throw knives
    //state 5 = going back to hide in well
	
	if(this->state == 0){
		
	}
	else if(this->state == 1){
		
	}
	else if(this->state == 2){
        // hopefully will shoot twice
        fire();
	}
	else if(this->state == 3){
        // state where he moves towards player
		charge();
		this->targX = this->sayu->position.x;
		this->targY = this->sayu->position.y;
        fire();
	}
    else if(this->state == 4){
        this->targX = this->sayu->position.x;
		this->targY = this->sayu->position.y;
        charge();
        this->waitToReload ++;
    } else if(this->state == 5 && !this->invincible){
        // go to well!
        this->targX = 1421;
        this->targY = 696;
        charge();
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
        if (shots_fired == 6) {
            this->state = 5;
            shots_fired = 0;
            this->shoot = 0;
        }
        else if (dist > 200) {
            this->state = 3;
            this->targX = this->sayu->position.x;
			this->targY = this->sayu->position.y;
            //shots_fired = 0;
        }
	}
	else if(this->state == 3){
        int dist = std::max(std::abs(this->position.x-this->sayu->position.x),std::abs(this->position.y-this->sayu->position.y));
        if (shots_fired == 6) {
            this->state = 4;
            shots_fired = 0;
            this->shoot = 0;
        }
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
            this->knives = true;
        }
        if (this->waitToReload == 200) {        
            this->knives = false;  
            this->state = 2;
            this->shots_fired = 0;
            this->waitToReload = 0;
        }
    }
    else if(this->state == 5){
        if (this->invincible) {
			if (this->timeInWell == 0 && !leaderText->textLock && leaderText->nextLine != leaderText->maxLine) {
				leaderText->background->alpha = 100;
				leaderText->drawNextLine();
			}
            fire();
            if (this->timeInWell == 500) {
                this->timeInWell = 0;
                this->state = 2;
                this->position = {1400, 800};
                this->shots_fired = 0;
                this->invincible = false;
				this->knife_invincible = true;
                this->alpha = this->oldAlpha;
				if (leaderText->nextLine != leaderText->maxLine) {
					leaderText->drawNextLine();
					leaderText->background->alpha = 0;
				}
            } else {
            	this->timeInWell++;
			}
        }
    }

    // what this does is after random intervals flips the dual wield mechanism
    if (this->waitToDualTimer == this->waitToDual) {
        this->waitToDual = rand() % 200 + 200;
        this->waitToDualTimer = 0;
        this->dualWield = !this->dualWield;
    }
    this->waitToDualTimer ++;

	// this will make him go back to well at 200 health intervals
	if (this->health % 200 == 0) {
		this->health --;
		this->state=5;
	}
	
	this->save();
}

void GangLeader::onCollision(DisplayObject* other){
	if (!this->invincible && other->type == "Projectile" && other->id != lastId && other->id != lastTwoId && other->id != lastThreeId && other->id != lastFourId && other->id != lastFiveId && other->id != lastSixId) {
		Projectile *temp = (Projectile*)other;
		if (temp->gun == "revolver") {
			this->health -= 20;
			this->alpha -= 5;
			if(this->health < 0) this->health = 0;
		}else if (temp->gun == "knife" && temp->thrown) {
		} else if(temp->gun == "knife" && !this->knife_invincible) {
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
	} else if(other->type == "Well"){
        this->state = 5;
        this->alpha = 0;
        this->invincible = true;
    }
    else{
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

SDL_Point* GangLeader::getGlobalHitbox(){
	//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
	AffineTransform* temp = this->getGlobalTransform();
	this->MyGlobalHitbox[0] = temp->transformPoint(-this->width/2, -this->height/2);
	this->MyGlobalHitbox[1] = temp->transformPoint(this->width/2, -this->height/2);
	this->MyGlobalHitbox[2] = temp->transformPoint(-this->width/2, this->height/2);
	this->MyGlobalHitbox[3] = temp->transformPoint(this->width/2, this->height/2);
	return this->MyGlobalHitbox;
}

void GangLeader::draw(AffineTransform &at){
	AnimatedSprite::draw(at);
}

void GangLeader::save(){
	this->oldX = position.x;
	this->oldY = position.y;
    if (alpha != 0)
        this->oldAlpha = alpha;
}

void GangLeader::charge(){
	moveToTarget();
}

void GangLeader::moveToTarget(){

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

bool GangLeader::isTargetReached(){
	return std::abs(this->position.x-this->targX) <= 6 && std::abs(this->position.y-this->targY) <= 6;
}

int GangLeader::fire() {
	this->shoot += 1;
	return shoot;
}


