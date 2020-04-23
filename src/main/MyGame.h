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
#include "../engine/TownsPeople.h"
#include "../engine/Wolf.h"
#include "../engine/GangThug.h"
#include "../engine/GangShot.h"
#include "../engine/Creeper.h"
#include "../engine/KnifeGuy.h"
#include "../engine/GangMarksman.h"
#include "../engine/ShotgunGuy.h"
#include "../engine/Projectile.h"
#include "../engine/Scene.h"
#include "../engine/Sound.h"
#include "../engine/tweens/Tween.h"
#include "../engine/tweens/TweenJuggler.h"
#include "../engine/eventhandlers/CoinListener.h"
#include "../engine/TransitionStruct.h"
#include "../engine/SceneInfo.h"
#include "../engine/Music.h"
#include "../engine/ui/TextBox.h"
#include "../engine/ui/HealthBar.h"
#include "../engine/ui/WeaponSelect.h"
#include "../engine/ui/AmmoCount.h"
//#include "MyObject.h"


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

	void checkTransition();
	void transitionScene();

	void initTown();
	void initLake();
	void initCanyon();
	void initBadlands();
	void initHideout();

	void initObstacles();

	void initTownsPeople(Scene* s);
	void initLakeEnemies(Scene* s);
	void initCanyonEnemies(Scene* s);
	void initHideoutEnemies(Scene* s);
	void initBadlandsEnemies(Scene* s);
	void initTownEnemies(Scene* s);

	void playerShooting(int gun, string dir);
	void reloadGun(int gun);
	void enemyShootingLoops();

	//map<Benemy*, GangThug*> thug_benemies;
	std::map<GangThug*,int> gang_thugs;
	std::map<GangShot*,int> gang_shot;
	std::map<GangMarksman*,int> gang_marksmans;
	std::map<ArrowGuy*,int> arrow_guys;
	std::map<ShotgunGuy*,int> shotgun_boss;

private:

	void addDOC(Scene* s, string type, int width, int height, int xpos, int ypos);

	Scene* curScene = NULL;

	//TextBox* test;

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
	TownsPeople* walkingTownee1;
	TownsPeople* storekeeper1;

	// LAKE STILL SCENES
	Scene* lake1;
	Scene* lake2;
	Scene* lake3;
	Scene* lake4;
	Scene* lake5;
	Scene* lake6;
	Scene* lake7;
	Scene* lake8;
	Scene* lake9;

	// ROCK CANYON SCENES
	Scene* canyon1;
	Scene* canyon2;
	Scene* canyon3;

	TextBox* openingText;

	// BADLANDS SCENES
	Scene* badlands1;
	Scene* badlands2;
	Scene* badlands3;
	Scene* badlands4;
	Scene* badlands5;
	Scene* badlands6;

	// HIDEOUT SCENES
	Scene* hideout1;
	Scene* hideout2;
	Scene* hideout3;
	Scene* hideout4;
	Scene* hideout5;
	Scene* hideout6;
	Scene* hideout7;
	Scene* hideout8;

	WeaponSelect* selection;
	AmmoCount* ammoCounter;
	Player* character;
	Projectile* bullet;
	
	HealthBar* playerHealth;

	// LAKE STILL ENEMIES
	Wolf* wolf1LakeStill1;
	Wolf* wolf2LakeStill1;
	
	GangThug* thug1LakeStill2;
	GangThug* thug2LakeStill2;

	GangShot* shot1LakeStill;
	
	GangMarksman* mark1LakeStill3;
	GangMarksman* mark2LakeStill3;

	ArrowGuy* arrow1LakeStill4;
	ArrowGuy* arrow2LakeStill4;

	Creeper* creeper1LakeStill7;
	Creeper* creeper2LakeStill7;

	KnifeGuy* knifeguy1LakeStill8;

	// CANYON ENEMIES
	Wolf* wolf1Canyon3;
	Wolf* wolf2Canyon3;
	Wolf* wolf3Canyon3;

	// HIDEOUT ENEMIES
	ShotgunGuy* boss_1;
	
	//BADLANDS ENEMIES
	Wolf* wolf1Badlands1;
	Wolf* wolf2Badlands1;
	Wolf* wolf3Badlands1;
	Wolf* wolf4Badlands1;
	Wolf* wolf5Badlands1;
	Wolf* wolf6Badlands1;

	GangThug* thug1Badlands2;
	GangThug* thug2Badlands2;
	GangThug* thug3Badlands2;
	GangThug* thug4Badlands2;
	GangMarksman* mark1Badlands2;
	GangMarksman* mark2Badlands2;

	Wolf* wolf1Badlands3;
	Wolf* wolf2Badlands3;
	GangMarksman* mark1Badlands3;
	GangMarksman* mark2Badlands3;
	Creeper* creeper1Badlands3;

	KnifeGuy* knifeguy1Badlands4;
	GangShot* shot1Badlands4;

	GangThug* thug1Badlands5;
	GangThug* thug2Badlands5;
	GangThug* thug3Badlands5;

	Creeper* creeper1Badlands6;
	Creeper* creeper2Badlands6;
	GangThug* thug1Badlands6;
	GangThug* thug2Badlands6;
	GangThug* thug3Badlands6;


	// END ENEMIES CREATION

	int iterate;

	Sprite* blackBox;
	Sprite* healthBackground;

	DisplayObjectContainer* foreground;
	Scene* cameraDemoScene;

	//MyObject* object1;
	//MyObject* object2;

	bool sceneFlip = false;
	bool keyToggle = true;

	TransitionStruct curTransition;

	int room_state = -1;

	Music* currentMusic;

	Music* lakeMusic;
	Music* townMusic;

	TweenJuggler* juggler;

	vector<vector<TransitionStruct>> transitions;
	vector<SceneInfo> sceneInfo;

	int projectileIdNum = 0; //allows for unique id for each projectile

};

#endif
