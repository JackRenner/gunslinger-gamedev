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
#include "../engine/Sheriff.h"
#include "../engine/Wolf.h"
#include "../engine/GangThug.h"
#include "../engine/GangShot.h"
#include "../engine/Creeper.h"
#include "../engine/KnifeGuy.h"
#include "../engine/GangMarksman.h"
#include "../engine/GangMarksmanFree.h"
#include "../engine/ShotgunGuy.h"
#include "../engine/GangLeader.h"
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

	int testint = 0;
	int testint2 = 0;
	int testint3 = 0;

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
	std::map<GangMarksmanFree*,int> gang_marksmansfree;
	std::map<ArrowGuy*,int> arrow_guys;
	std::map<ShotgunGuy*,int> shotgun_boss;
	std::map<GangLeader*,int> final_bosses;

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
	TownsPeople* hotelMan;
	TownsPeople* bankMan;
	TownsPeople* postMan;
	TownsPeople* cantinaMan;
	TownsPeople* drugMan;

	Sheriff* sheriff1;
	Sheriff* sheriff2;

	Sprite* rolandDeschain;
	Sprite* floryan;
	bool rolandNotHere = true;
	TextBox* rolandText;
	TextBox* floryanText;


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
	TextBox* wasdText;
	TextBox* knifeText;
	TextBox* riverText;
	TextBox* dashText;
	TextBox* throwText;
	TextBox* whiskeyText;

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
	ArrowGuy* arrow2LakeStill9;
	ArrowGuy* arrow1LakeStill9;
	ArrowGuy* arrow3LakeStill9;
	ArrowGuy* arrow4LakeStill9;
	

	Creeper* creeper1LakeStill7;
	Creeper* creeper2LakeStill7;

	KnifeGuy* knifeguy1LakeStill8;

	//Text once revolver is unlocked
	TextBox* revolverText;

	// CANYON ENEMIES
	Wolf* wolf1Canyon3;
	Wolf* wolf2Canyon3;
	Wolf* wolf3Canyon3;

	// HIDEOUT ENEMIES
	ShotgunGuy* boss_1;
	KnifeGuy* knifeguy1hideout6;
	KnifeGuy* knifeguy2hideout6;
	KnifeGuy* knifeguy3hideout6;
	KnifeGuy* knifeguy4hideout6;

	Creeper* creeper1hideout6;
	Creeper* creeper2hideout6;

	GangThug* gangthug1hideout6;
	GangThug* gangthug2hideout6;


	GangMarksmanFree* gangmarksmanfree1hideout5;
	GangMarksmanFree* gangmarksmanfree2hideout5;

	KnifeGuy* knifeguy1hideout5;
	KnifeGuy* knifeguy2hideout5;

	GangShot* shot1hideout5;
	GangShot* shot2hideout5;

	KnifeGuy* knifeguy3hideout3;
	KnifeGuy* knifeguy2hideout3;
	KnifeGuy* knifeguy1hideout3;
	KnifeGuy* knifeguy4hideout3;

	GangMarksman* gangmarksmanfree1hideout3;
	GangMarksman* gangmarksmanfree2hideout3;

	GangThug* gangthug1hideout3;
	GangThug* gangthug2hideout3;
	GangThug* gangthug3hideout3;
	GangThug* gangthug4hideout3;

	Creeper* creeper1hideout3;
	Creeper* creeper2hideout3;

	GangShot* shot1hideout3;
	GangShot* shot2hideout3;

	KnifeGuy* knifeguy3hideout2;
	KnifeGuy* knifeguy2hideout2;
	KnifeGuy* knifeguy1hideout2;
	KnifeGuy* knifeguy4hideout2;

	GangThug* gangthug1hideout2;
	GangThug* gangthug2hideout2;
	GangThug* gangthug3hideout2;

	GangThug* gangthug1hideout1;
	GangThug* gangthug2hideout1;
	GangThug* gangthug3hideout1;
	
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


	int lakeEnemiesLeft = 12;

	// Final Town Battle
	GangLeader* final_boss;

	GangThug* angryTownspeople1;
	GangThug* angryTownspeople2;
	GangThug* angryTownspeople3;
	GangThug* angryTownspeople4;
	GangThug* angryTownspeople5;
	GangThug* angryTownspeople6;


	// END ENEMIES CREATION

	int iterate;

	int reloadTime = 300;

	Sprite* blackBox;
	Sprite* healthBackground;

	DisplayObjectContainer* foreground;
	Scene* cameraDemoScene;

	bool sceneFlip = false;
	bool keyToggle = true;

	TransitionStruct curTransition;

	int room_state = -1;

	Music* currentMusic;

	Music* lakeMusic;
	Music* townMusic;
	Music* badlandsMusic;
	Music* canyonMusic;
	Music* hideoutMusic;
	Music* bossMusic;
	Music* finaleMusic;

	Sound* knifeThrow;
	Sound* pistolShot;
	Sound* shotgunShot;
	Sound* rifleShot;

	TweenJuggler* juggler;

	vector<vector<TransitionStruct>> transitions;
	vector<SceneInfo> sceneInfo;

	int projectileIdNum = 0; //allows for unique id for each projectile

};

#endif
