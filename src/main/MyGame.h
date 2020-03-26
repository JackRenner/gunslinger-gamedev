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
#include "../engine/tweens/Tween.h"
#include "../engine/tweens/TweenJuggler.h"
#include "../engine/eventhandlers/CoinListener.h"
#include "../engine/TransitionPoint.h"
#include "../engine/SceneInfo.h"

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

class MyGame : public Game, public EventListener {

public:
	MyGame();
	virtual ~MyGame();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);

	void setScene(Scene* scene);

	bool checkInsideBox(SDL_Rect box, DisplayObject* entity);
	bool checkInsidePoint(SDL_Point point, DisplayObject* entity);

	void addCameraBound(SDL_Rect bounds, bool up, bool down, bool left, bool right);
	void enforceCameraBounds();

	void handleEvent(Event* e);

	void transitionScene(TransitionPoint tp);

private:

	Scene* curScene = NULL;

	Scene* townScene;
	Scene* bankScene;

	AnimatedSprite* character;
	Sprite* coin;

	DisplayObjectContainer* foreground;

	bool sceneFlip = false;
	bool keyToggle = true;

	vector<Bound> boundaries;
	int room_state = -1;

	Sound* music;

	TweenJuggler* juggler;

	vector<vector<TransitionPoint>> transitionPoints;
	vector<SceneInfo> sceneInfo;
};

#endif