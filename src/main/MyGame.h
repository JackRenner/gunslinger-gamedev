#ifndef MYGAME_H
#define MYGAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Game.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Player.h"
#include "../engine/Projectile.h"
#include "../engine/Scene.h"
#include "../engine/Sound.h"
#include "../engine/tweens/Tween.h"
#include "../engine/tweens/TweenJuggler.h"
#include "../engine/eventhandlers/CoinListener.h"
#include "../engine/TransitionStruct.h"
#include "../engine/SceneInfo.h"

using namespace std;

class MyGame : public Game, public EventListener {

public:
	MyGame();
	virtual ~MyGame();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);

	void setScene(Scene* scene);

	bool checkInsideBox(SDL_Rect box, DisplayObject* entity);
	bool checkInsidePoint(SDL_Point point, DisplayObject* entity);
	int gun = 0;

	void enforceCameraBounds();

	void handleEvent(Event* e);

	void transitionScene();

	void initTown();
	void initLake();

	void playerShooting(int gun, string dir);
	void reloadGun(int gun);

private:

	Scene* curScene = NULL;

	Scene* townScene;
	Scene* sheriffScene;
	Scene* storeScene;
	Scene* hotelScene;
	Scene* bankScene;
	Scene* postScene;
	Scene* cantinaScene;
	Scene* drugScene;

	Scene* lake1;
	Scene* lake2;
	Scene* lake3;
	Scene* lake4;
	Scene* lake5;
	Scene* lake6;
	Scene* lake7;
	Scene* lake8;
	Scene* lake9;

	AnimatedSprite* character;
	AnimatedSprite* bullet;
	
	/* shooting */
	int knife_throws = 0;
	int revolver_shots = 0;
	int shotgun_shots = 0;
	int rifle_shots = 0;
	Sprite* blackBox;

	DisplayObjectContainer* foreground;

	bool sceneFlip = false;
	bool keyToggle = true;
	bool transLock = false;

	TransitionStruct curTransition;

	int room_state = -1;

	Sound* music;

	TweenJuggler* juggler;

	vector<vector<TransitionStruct>> transitions;
	vector<SceneInfo> sceneInfo;
};

#endif