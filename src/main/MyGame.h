#ifndef MYGAME_H
#define MYGAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Game.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Scene.h"
#include "../engine/Sound.h"
#include "../engine/Tween.h"
#include "../engine/TweenJuggler.h"


using namespace std;

struct Bound {
	SDL_Rect bounds;
	bool check_up = true;
	bool check_down = true;
	bool check_left = true;
	bool check_right = true;

	Bound(SDL_Rect bounds, bool up, bool down, bool left, bool right) {
		this->bounds = bounds;
		this->check_up = up;
		this->check_down = down;
		this->check_left = left;
		this->check_right = right;
	};
};

class MyGame : public Game {

public:
	MyGame();
	virtual ~MyGame();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);

	void setScene(Scene* scene);

	bool checkInside(SDL_Rect box, DisplayObject* entity);

	void addCameraBound(SDL_Rect bounds, bool up, bool down, bool left, bool right);
	void enforceCameraBounds();

private:

	Scene* curScene = NULL;

	Scene* cameraDemoScene;

	AnimatedSprite* character;
	Sprite* coin;

	bool sceneFlip = false;
	bool keyToggle = true;
	bool coinPickedUp = false;

	vector<Bound> boundaries;
	int room_state = -1;

	SDL_Point zoomPoint;

	Sound* gunshot;
	Sound* music;
};

#endif