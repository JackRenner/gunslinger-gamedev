#include "DisplayObject.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Controls.h"


#define PI 3.14159265

bool transLock = false;

DisplayObject::DisplayObject(){
	image = NULL;
	texture = NULL;
	curTexture = NULL;

 this->MyGlobalHitbox = new SDL_Point[4];

 EventDispatcher* myEventDispatcher = new EventDispatcher();
 this->MyEventDispatcher = myEventDispatcher;



}

DisplayObject::DisplayObject(string id, string filepath){
	this->id = id;
	this->imgPath = filepath;

	loadTexture(filepath);


	this->MyGlobalHitbox = new SDL_Point[4];

	EventDispatcher* myEventDispatcher = new EventDispatcher();
	this->MyEventDispatcher = myEventDispatcher;

}

DisplayObject::DisplayObject(string id, int red, int green, int blue){
	isRGB = true;
	this->id = id;

	this->red = red;
	this->blue = blue;
	this->green = green;

	this->loadRGBTexture(red, green, blue);

	this->MyGlobalHitbox = new SDL_Point[4];


	EventDispatcher* myEventDispatcher = new EventDispatcher();
	this->MyEventDispatcher = myEventDispatcher;

}

DisplayObject::~DisplayObject(){
	//TODO: Get this freeing working
	if(image != NULL) SDL_FreeSurface(image);
	if(texture != NULL) SDL_DestroyTexture(texture);
	if(MyGlobalHitbox!= NULL){
		free(MyGlobalHitbox);
	}
	free(MyEventDispatcher);
}

void DisplayObject::loadTexture(string filepath){
	image = IMG_Load(filepath.c_str());
	texture = SDL_CreateTextureFromSurface(Game::renderer, image);
	setTexture(texture);
}

void DisplayObject::loadRGBTexture(int red, int green, int blue){
	image = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
	SDL_FillRect(image, NULL, SDL_MapRGB(image->format, red, green, blue));
	texture = SDL_CreateTextureFromSurface(Game::renderer, image);
	SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
	setTexture(texture);
}

void DisplayObject::setTexture(SDL_Texture* t){
	this->curTexture = t;
}

void DisplayObject::update(set<SDL_Scancode> pressedKeys){
	SDL_Point* temp = this->getGlobalHitbox();
}

void DisplayObject::setSourceRect(SDL_Rect* srcrect){
	this->srcrect = srcrect;
}

void DisplayObject::draw(AffineTransform &at){
	applyTransformations(at);

	//at.translate(-gameCamera.x, -gameCamera.y);

	if(curTexture != NULL && visible) {


		SDL_Point origin = at.transformPoint(0, 0);
		SDL_Point upperRight = at.transformPoint(width, 0);
		SDL_Point lowerRight = at.transformPoint(width, height);
		SDL_Point corner = {0, 0};

		int w = (int)distance(origin, upperRight);
		int h = (int)distance(upperRight, lowerRight);
		if(this->hitboxDrawn){
			this->getGlobalHitbox();
			cout << this->MyGlobalHitbox[0].x << endl;
			cout << this->MyGlobalHitbox[0].y <<endl;
			cout << this->MyGlobalHitbox[3].x << endl;
			cout << this->MyGlobalHitbox[3].y <<endl;
//			this->drawHitbox( setGlobalTransform(globalTransform, this->pivot) );
		 }
		SDL_Rect dstrect = { origin.x, origin.y, w, h };


		SDL_RendererFlip flip;
		if (facingRight) {
			flip = SDL_FLIP_NONE;
		}
		else {
			flip = SDL_FLIP_HORIZONTAL;
		}

		SDL_SetTextureAlphaMod(curTexture, alpha);
		SDL_RenderCopyEx(Game::renderer, curTexture, srcrect, &dstrect, calculateRotation(origin, upperRight), &corner, flip);

	}

	//at.translate(gameCamera.x, gameCamera.y);

	reverseTransformations(at);
}

void DisplayObject::applyTransformations(AffineTransform& at) {
	at.translate(position.x, position.y);
	at.rotate(rotation);
	at.scale(scaleX, scaleY);
	at.translate(-pivot.x, -pivot.y);
}

void DisplayObject::reverseTransformations(AffineTransform& at) {
	at.translate(pivot.x, pivot.y);
	at.scale(1.0 / scaleX, 1.0 / scaleY);
	at.rotate(-rotation);
	at.translate(-position.x, -position.y);
}

int DisplayObject::getWidth() {
	return this->image->w;
}

int DisplayObject::getHeight() {
	return this->image->h;
}

double DisplayObject::distance(SDL_Point &p1, SDL_Point &p2) {
	return sqrt(((p2.y - p1.y)*(p2.y - p1.y)) + ((p2.x - p1.x)*(p2.x - p1.x)));
}

double DisplayObject::calculateRotation(SDL_Point &origin, SDL_Point &p) {
	double y = p.y - origin.y;
	double x = p.x - origin.x;
	return (atan2(y, x) * 180 / PI);
}

//Fixed
//Matrix multiplication is not communative. As a result it needs to be done in order.
		AffineTransform* DisplayObject ::  getGlobalTransform(){


			AffineTransform* finalGlobalTransform;

			if(this->parent != NULL){
				finalGlobalTransform = this->parent->getGlobalTransform();
				finalGlobalTransform->translate(this->parent->pivot.x, this->parent->pivot.y);
				applyTransformations(*finalGlobalTransform);
			}
			else{
					finalGlobalTransform = new AffineTransform();
							applyTransformations(*finalGlobalTransform);
				}
				return finalGlobalTransform;
		}


		SDL_Point* DisplayObject :: getGlobalHitbox(){
			//Creating an array of SDL_Points allows us to return the four corners of the hitbox.
			AffineTransform* temp = this->getGlobalTransform();
			this->MyGlobalHitbox[0] = temp->transformPoint(0,0);
			this->MyGlobalHitbox[1] = temp->transformPoint(this->width, 0);
			this->MyGlobalHitbox[2] = temp->transformPoint(0, this->height);
			this->MyGlobalHitbox[3] = temp->transformPoint(this->width, this->height);
			return this->MyGlobalHitbox;
		}


		void DisplayObject :: onCollision(DisplayObject * otherObject){
			cout << "DisplayObjects onCollision is being called." <<endl;
		    Game::instance->ourCollisionSystem->resolveCollision(this, otherObject , this->position.x - oldX, this->position.y-oldY, 0, 0);

		}//Use this to specify what happens on hit with another object

	void DisplayObject :: drawHitbox(SDL_Point globalPosition){
/*
		SDL_Rect origin;
		origin.x = this->myHitbox.origin.x;
		origin.y = this->myHitbox.origin.y;
		origin.w = this->myHitbox.width;
		origin.h = this->myHitbox.height;
		SDL_Rect upperLeft;
		SDL_Rect upperRight;
		SDL_Rect lowerLeft;
		SDL_Rect lowerRight;


		origin.x = this->myHitbox.origin.x;
		origin.y = this->myHitbox.origin.y;
		origin.w = 35;
		origin.h = 35;

		upperLeft.x = this->myHitbox.upperLeft.x;
		upperLeft.y = this->myHitbox.upperLeft.y;
		upperLeft.w = 25;
		upperLeft.h = 25;
		upperRight.x = this->myHitbox.upperRight.x;
		upperRight.y = this->myHitbox.upperRight.y;
		upperRight.w = 20;
		upperRight.h = 20;
		lowerLeft.x = this->myHitbox.lowerLeft.x;
		lowerLeft.y = this->myHitbox.lowerLeft.y;
		lowerLeft.w = 15;
		lowerLeft.h = 15;
		lowerRight.x = this->myHitbox.lowerRight.x;
		lowerRight.y = this->myHitbox.lowerRight.y;
		lowerRight.w = 10;
		lowerRight.h = 10;
		SDL_RenderDrawRect(Game::renderer, &origin);
		SDL_RenderDrawRect(Game::renderer, &upperRight);
		SDL_RenderDrawRect(Game::renderer, &lowerLeft);
		SDL_RenderDrawRect(Game::renderer, &lowerRight);
		SDL_RenderDrawRect(Game::renderer, &upperLeft);

		*/

		/*
		cout << "position x : ";
		cout << this->position.x << endl;
		cout << "position y: ";
		cout << this->position.y << endl;

		cout << "ID: ";
		cout << this->id <<endl;
		cout << "Global position x: ";
		cout << globalPosition.x <<endl;
		cout << "Global Position y: ";
		cout << globalPosition.y << endl;
*/

		SDL_Rect hitboxRect;
		hitboxRect.x = globalPosition.x;
		hitboxRect.y = globalPosition.y;
		hitboxRect.w = this -> width;
		hitboxRect.h = this -> height;

		SDL_RenderDrawRect(Game::renderer, &hitboxRect);
//		SDL_RenderDrawRect(Game::renderer, &temp);
		SDL_RenderPresent(Game::renderer);

//		cout<< "DrawHitboxPressed";
//		SDL_RenderCopyEx(Game::renderer, curTexture, srcrect, &dstrect, calculateRotation(origin, upperRight), &corner, flip);
	};

	void DisplayObject :: savePosition(){
		//cout << "DO savePosition called" <<endl;
		this->oldX = position.x;
	  this->oldY = position.y;
	};
	void DisplayObject:: saveAllPositions(){
		cout << "Display Object's Save all Positions Called";
		this-> savePosition();
	}
