#include "DisplayObject.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>

#define PI 3.14159265

DisplayObject::DisplayObject(){
	image = NULL;
	texture = NULL;
	curTexture = NULL;

//
 this->myHitbox =  Hitbox();

}

DisplayObject::DisplayObject(string id, string filepath){
	this->id = id;
	this->imgPath = filepath;

	loadTexture(filepath);

	this->myHitbox =  Hitbox();
}

DisplayObject::DisplayObject(string id, int red, int green, int blue){
	isRGB = true;
	this->id = id;

	this->red = red;
	this->blue = blue;
	this->green = green;

	this->loadRGBTexture(red, green, blue);
}

DisplayObject::~DisplayObject(){
	//TODO: Get this freeing working
	if(image != NULL) SDL_FreeSurface(image);
	if(texture != NULL) SDL_DestroyTexture(texture);
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

		transformedOrigin = origin;
		transformedURight = upperRight;
		transformedLRight = lowerRight;
		transformedLLeft = at.transformPoint(0, height);

		int w = (int)distance(origin, upperRight);
		int h = (int)distance(upperRight, lowerRight);

		//Lets go inserting!
		this->saveHitbox(transformedOrigin, transformedURight, transformedLRight, transformedLLeft, w, h);

		if(this->hitboxDrawn){
			drawHitbox();
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

void DisplayObject::applyTransformations(AffineTransform &at) {
	at.translate(position.x, position.y);
	at.rotate(rotation);
	at.scale(scaleX, scaleY);
	at.translate(-pivot.x, -pivot.y);
}

void DisplayObject::reverseTransformations(AffineTransform &at) {
	at.translate(pivot.x, pivot.y);
	at.scale(1.0/scaleX, 1.0/scaleY);
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


	void DisplayObject:: saveHitbox(SDL_Point transformedOrigin, SDL_Point transformedURight,	SDL_Point transformedLRight, SDL_Point transformedLLeft, int width, int height){
			this->myHitbox.origin.x = transformedOrigin.x;
			this->myHitbox.origin.y = transformedOrigin.x;
			this->myHitbox.upperRight.x = transformedURight.x;
			this->myHitbox.upperRight.y = transformedURight.x;
			this->myHitbox.lowerLeft.x = transformedLLeft.x;
			this->myHitbox.lowerLeft.y = transformedLLeft.y;
			this->myHitbox.lowerRight.x = transformedLRight.x;
			this->myHitbox.lowerRight.y = transformedLRight.y;
			this->myHitbox.width =  width;
			this->myHitbox.height = height;
	}


	Hitbox DisplayObject:: getHitbox(){
		return this->myHitbox;
	}
	void DisplayObject :: drawHitbox(){
		SDL_Rect temp;

		temp.x = this->myHitbox.origin.x;
		temp.y = this->myHitbox.origin.y;
		temp.w = this->myHitbox.width;
		temp.h = this->myHitbox.height;


		SDL_Rect origin;
		SDL_Rect upperRight;
		SDL_Rect lowerLeft;
		SDL_Rect lowerRight;

		origin.x = this->myHitbox.origin.x;
		origin.y = this->myHitbox.origin.y;
		origin.w = 25;
		origin.h = 25;



		upperRight.x = this->myHitbox.upperRight.x;
		upperRight.y = this->myHitbox.upperRight.y;
		upperRight.w = 20;
		upperRight.h = 20;

		lowerLeft.x = this->myHitbox.lowerLeft.x;
		lowerLeft.y = this->myHitbox.lowerLeft.y;
		lowerLeft.w = 10;
		lowerLeft.h = 10;


		lowerRight.x = this->myHitbox.lowerRight.x;
		lowerRight.y = this->myHitbox.lowerRight.y;
		lowerRight.w = 15;
		lowerRight.h = 15;



		SDL_RenderDrawRect(Game::renderer, &origin);
		SDL_RenderDrawRect(Game::renderer, &upperRight);
		SDL_RenderDrawRect(Game::renderer, &lowerLeft);
		SDL_RenderDrawRect(Game::renderer, &lowerRight);


		SDL_SetRenderDrawColor(Game::renderer, 136, 0, 0, 255);
		SDL_RenderDrawRect(Game::renderer, &temp);
		SDL_RenderPresent(Game::renderer);

//		cout<< "DrawHitboxPressed";
//		SDL_RenderCopyEx(Game::renderer, curTexture, srcrect, &dstrect, calculateRotation(origin, upperRight), &corner, flip);

	};
