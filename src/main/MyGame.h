#ifndef MYGAME_H
#define MYGAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Game.h"
#include "../engine/ArrowGuy.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Player.h"
#include "../engine/Wolf.h"
#include "../engine/GangThug.h"
#include "../engine/Creeper.h"
#include "../engine/GangMarksman.h"
#include "../engine/Projectile.h"
#include "../engine/Scene.h"
#include "../engine/Sound.h"
#include "../engine/tweens/Tween.h"
#include "../engine/tweens/TweenJuggler.h"
#include "../engine/eventhandlers/CoinListener.h"
#include "../engine/TransitionStruct.h"
#include "../engine/SceneInfo.h"
#include "../engine/ui/TextBox.h"
#include "../engine/ui/HealthBar.h"
#include "MyObject.h"


using namespace std;

extern bool transLock;

class MyGame : public Game, public EventListener {

public:
	MyGame();
	virtual ~MyGame();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);

	void setScene(Scene* scene);

	bool checkInsideBox(SDL_Rect box, DisplayObject* entity);
	bool checkInsidePoint(SDL_Point point, DisplayObject* entity);

	void enforceCameraBounds();

	void handleEvent(Event* e);

	void transitionScene();

	void initTown();
	void initLake();

	void initEnemies(Scene* s);

	void playerShooting(int gun, string dir);
	void reloadGun(int gun);

	//map<Benemy*, GangThug*> thug_benemies;
	std::map<GangThug*,int> gang_thugs;
	std::map<ArrowGuy*,int> arrow_guys;

private:

	Scene* curScene = NULL;

	TextBox* test;

	// TOWN SCENES
	Scene* townScene;
	Scene* sheriffScene;
	Scene* storeScene;
	Scene* hotelScene;
	Scene* bankScene;
	Scene* postScene;
	Scene* cantinaScene;
	Scene* drugScene;

	// TOWN NPCS
	DisplayObject* storekeeper;

	Scene* lake1;
	Scene* lake2;
	Scene* lake3;
	Scene* lake4;
	Scene* lake5;
	Scene* lake6;
	Scene* lake7;
	Scene* lake8;
	Scene* lake9;

	Player* character;
	Projectile* bullet;
	
	HealthBar* playerHealth;

	Wolf* wolf1LakeStill1;
	Wolf* wolf2LakeStill1;
	
	GangThug* thug1LakeStill2;

	GangMarksman* mark1LakeStill3;

	ArrowGuy* arrow1LakeStill4;

	Creeper* creeper1LakeStill7;

	//AnimatedSprite* wolf;
	Benemy* benemy;
	Benemy* benemy2;
	Benemy* benemy3;

	Sprite* blackBox;
	Sprite* healthBackground;

	DisplayObjectContainer* foreground;
	Scene* cameraDemoScene;

	MyObject* object1;
	MyObject* object2;

	bool sceneFlip = false;
	bool keyToggle = true;

	TransitionStruct curTransition;

	int room_state = -1;

	Sound* music;

	TweenJuggler* juggler;

	vector<vector<TransitionStruct>> transitions;
	vector<SceneInfo> sceneInfo;
};

#endif
