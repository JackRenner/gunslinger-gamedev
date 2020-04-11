#ifndef DISPLAYOBJECT_H
#define DISPLAYOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <set>
#include "AffineTransform.h"
#include "Camera.h"
#include <string>
#include <fstream>
#include "Controls.h"

#include "EventDispatcher.h"
#include "Event.h"

using namespace std;

extern bool transLock;

struct Hitbox{
	SDL_Point globalPosition;
	/*
	SDL_Point origin;
	SDL_Point upperLeft;
	SDL_Point upperRight;
	SDL_Point lowerLeft;
	SDL_Point lowerRight;
	int width;
	int height;
*/
	Hitbox(){//set default values to be 00
			SDL_Point globalPosition = {0,0};
/*
		 	origin = {0,0};
			upperLeft = {0,0};
			upperRight = {0,0};
			lowerLeft = {0,0};
			lowerRight = {0,0};
			width = 0;
			height = 0;
*/
	}
};


class DisplayObject{

public:
	string id = "DEFAULT_ID";
	string imgPath = "";
	int red = 0;
	int green = 0; 
	int blue = 0;
	string type = "DisplayObject";
	DisplayObject* parent = NULL;

	bool isRGB = false;

	DisplayObject();
	DisplayObject(string id, string path);
	DisplayObject(string id, int red, int green, int blue);
	virtual ~DisplayObject();
	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);
	void loadTexture(string filepath);
	void loadRGBTexture(int red, int green, int blue);
	void setTexture(SDL_Texture* t);

	void applyTransformations(AffineTransform &at);
	void reverseTransformations(AffineTransform &at);

	int getWidth();
	int getHeight();

	void setSourceRect(SDL_Rect* srcrect);

	bool visible = true;
	SDL_Point position = {0, 0};
	int width = 100;
	int height = 100;
	SDL_Point pivot = {0, 0};
	double scaleX = 1.0;
	double scaleY = 1.0;
	double rotation = 0.0; // in radians
	int alpha = 255;
	bool facingRight = true;
	SDL_Point transformedOrigin;
	SDL_Point transformedURight;
	SDL_Point transformedLRight;
	SDL_Point transformedLLeft;


	int oldMinX;
	int oldMaxX;
	int oldMinY;
	int oldMaxY;




	//This rect is to allow spritesheet support in AnimatedSprite.h
	SDL_Rect* srcrect = NULL;

	static Camera gameCamera;
	struct Hitbox myHitbox;

//	void saveHitbox(SDL_Point transformedOrigin, SDL_Point transformedURight,	SDL_Point transformedLRight, SDL_Point transformedLLeft, int width, int height);


//		AffineTransform myTransform;


	AffineTransform globalTransform = AffineTransform();
	AffineTransform passAffineTransform(AffineTransform& toPass);

	SDL_Point* MyGlobalHitbox;


	AffineTransform* getGlobalTransform();
	SDL_Point* getGlobalHitbox(); // Specified that its put into the DisplayObject	/
	//In C++, this can return an array of points.
	//These points will be the four corners.
	//AffineTransform = getGlobalTransform
	//Use Transformpoint to on 0,0
	//width,0,
	//o height,
	//width, height
	//THen free the affine Transform.




	void drawHitbox(SDL_Point globalPosition);
	bool hitboxDrawn = false;
	virtual void onCollision(DisplayObject * otherObject);
	virtual void savePosition();
	virtual void saveAllPositions();
	int oldX;
	int oldY;


	EventDispatcher* MyEventDispatcher; //Create an EventDispatcher for DOC when built.
	//Then we can call it for all

private:
	double distance(SDL_Point &p1, SDL_Point &p2);
	double calculateRotation(SDL_Point &origin, SDL_Point &p);

	SDL_Texture* texture = NULL;
	SDL_Surface* image = NULL;

	/* Texture currently being drawn. Equal to texture for normal DO */
	SDL_Texture* curTexture;

};

#endif
