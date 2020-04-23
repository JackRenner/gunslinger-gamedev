#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include "../engine/Controls.h"
#include "../engine/Player.h"
#include "../engine/Benemy.h"
#include "MyGame.h"

using namespace std;

Camera DisplayObject::gameCamera;

MyGame::MyGame() : Game(gameCamera.viewportWidth, gameCamera.viewportHeight) {

	gameCamera = Camera();

	foreground = new DisplayObjectContainer();
	foreground->id = "foreground";

	townMusic = new Music("./resources/music/MoodyLoop.wav");
	lakeMusic = new Music("./resources/music/lakestill_music.wav");

    character = new Player();
	//this->removeImmediateChild(character);

	character->position = { 80, 600 };
	character->scaleX = 0.8;
	character->scaleY = 0.8;
	character->pivot = { character->width / 2, character->height / 2 };

	character->gun = 0;

	initTown();
	initLake();
	initCanyon();
	initBadlands();
	initHideout();

	//room_state = 17;

	//this->setScene(canyon1);
	room_state = 26;
	this->setScene(hideout1);
	this->addChild(foreground);
	
	juggler = TweenJuggler::getInstance();

	blackBox = new Sprite("blackbox",0,0,0);
	blackBox->alpha = 0;
	blackBox->width = 5000;
	blackBox->height = 5000;
	foreground->addChild(blackBox);
	
	selection = new WeaponSelect();
	selection->position = { character->position.x - (gameCamera.viewportWidth / 2) + 10, (character->position.y - gameCamera.viewportHeight / 2 + 10) };
	ammoCounter = new AmmoCount();
	ammoCounter->position = { -25, -105 };

	healthBackground = new Sprite("blackbox", 255, 0, 0);
	healthBackground->id = "healthbackground";
	healthBackground->position = { -50,-65 };
	healthBackground->width = 100;
	healthBackground->height = 20;
	
	playerHealth = new HealthBar(character, 0, 500);

	character->addEventListener(selection, WeaponSelectEvent::SELECT_FIST_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_KNIFE_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_PISTOL_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_SHOTGUN_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_RIFLE_EVENT);

	character->addEventListener(ammoCounter, WeaponSelectEvent::UPDATE_AMMO);

	foreground->addChild(character);
	foreground->addChild(selection);
	character->addChild(healthBackground);
	character->addChild(playerHealth);
	character->addChild(ammoCounter);
	foreground->addChild(blackBox);
}

MyGame::~MyGame() {

}


void MyGame::update(set<SDL_Scancode> pressedKeys) {
	controls::update(pressedKeys);

	this->saveAllPositions();

	// code to reset scene
	if (character->health <= 0) {
		curTransition = transitions[0][0];
		transitionScene();
	}
	// Demo for enemies

	// need to make these for loops that loop through for each type of enemy

	this->enemyShootingLoops();
	
	// for (std::map<Benemy*, GangThug*>::iterator it=thug_benemies.begin(); it!=thug_benemies.end(); ++it) {
	// 	if (it->first->clean != true) {
	// 		if (this->ourCollisionSystem->collidesWith(it->first, character)) {
	// 			character->takeDamage(1);
	// 		}
	// 	} else {
	// 		thug_benemies.erase(it->first);
	// 		break;
	// 	}
	// }

	if (!transLock) {
		// gun select
		if (controls::press1()) {
			character->selectWeapon(0);
		};
		if (controls::press2()) {
			character->selectWeapon(1);
		};
		if (controls::press3()) {
			character->selectWeapon(2);
		};
		if (controls::press4()) {
			character->selectWeapon(3);
		};
		if (controls::press5()) {
			character->selectWeapon(4);
		};
		// shooting
		if (controls::pressUp()) {
			//gunshot->playSFX();
			this->playerShooting(character->gun, "up");
		}
		if (controls::pressDown()) {
			//gunshot->playSFX();
			this->playerShooting(character->gun, "down");
		}
		if (controls::pressRight()) {
			//gunshot->playSFX();	
			this->playerShooting(character->gun, "left");
		}
		if (controls::pressLeft()) {
			//gunshot->playSFX();	
			this->playerShooting(character->gun, "right");
		}
		// there isn't any delay from reloading yet
		if (controls::pressR()) {
			this->reloadGun(character->gun);
		}
	}

	Game::update(pressedKeys);
	controls::update(pressedKeys);

	gameCamera.x = character->position.x - gameCamera.viewportWidth / 2;
	gameCamera.y = character->position.y - gameCamera.viewportHeight / 2;

	if (!transLock) {
		checkTransition();
	}

	this->ourCollisionSystem->update();
	enforceCameraBounds();
	selection->position = { gameCamera.x + 10, gameCamera.y + 10 };
}

void MyGame::draw(AffineTransform& at) {
	at.translate(-gameCamera.x + gameCamera.zoomPoint.x, -gameCamera.y + gameCamera.zoomPoint.y);
	at.scale(gameCamera.scale, gameCamera.scale);
	at.translate(-gameCamera.zoomPoint.x, -gameCamera.zoomPoint.y);

	Game::draw(at);

	at.translate(gameCamera.zoomPoint.x, gameCamera.zoomPoint.y);
	at.scale(1 / gameCamera.scale, 1 / gameCamera.scale);
	at.translate(gameCamera.x - gameCamera.zoomPoint.x, gameCamera.y - gameCamera.zoomPoint.y);
}

// sets the current scene and adds as child to game and unlinks the old scene from game (does not destroy it)
// we can tweak this to destroy the scene for memory reasons (or add a new method to destroy), but left it like this for now
void MyGame::setScene(Scene* scene) {
	if (curScene != NULL)
		this->unlinkImmediateChild(curScene->id);
	this->curScene = scene;
	if (curScene != NULL) {
		this->addChild(curScene);
		if (scene->id.substr(0,4) == "lake"){
			initLakeEnemies(scene);
			if(currentMusic != lakeMusic){
				cout << "Start lake music!" << endl;
				lakeMusic->play();
				currentMusic = lakeMusic;
			}
		}
		else if (scene->id.substr(0,4) == "cany")
			initCanyonEnemies(scene);
		//all town scenes end with Scene
		else if (scene->id.length() > 5 && scene->id.substr(scene->id.length() - 5, scene->id.length() - 1) == "Scene") {
			if (currentMusic != townMusic) {
				cout << "Start town music!" << endl;
				townMusic->play();
				currentMusic = townMusic;
			}
		}
		else if (scene->id == "hideout4") {
			this->character->lightingSystem(true);
		} else {
			this->character->lightingSystem(false);
		}
		
		initObstacles();
	}
}

// Enforce camera bounds for the current room state. Does not currently account for room rotations.
void MyGame::enforceCameraBounds() {
	Bound room = sceneInfo[room_state].bounds;

	AffineTransform boundCalc = AffineTransform();

	boundCalc.translate(gameCamera.zoomPoint.x, gameCamera.zoomPoint.y);
	boundCalc.scale(gameCamera.scale, gameCamera.scale);
	boundCalc.translate(-gameCamera.zoomPoint.x, -gameCamera.zoomPoint.y);

	SDL_Point upper_left = boundCalc.transformPoint(room.bounds.x, room.bounds.y);
	SDL_Point lower_right = boundCalc.transformPoint(room.bounds.x + room.bounds.w, room.bounds.y + room.bounds.h);

	// check right bound
	if (room.check_right) {
		if (gameCamera.x + gameCamera.viewportWidth > lower_right.x)
			gameCamera.x = lower_right.x - gameCamera.viewportWidth;
	}
	// check left bound
	if (room.check_left) {
		if (gameCamera.x < upper_left.x)
			gameCamera.x = upper_left.x;
	}
	// check upper bound
	if (room.check_up) {
		if (gameCamera.y < upper_left.y)
			gameCamera.y = upper_left.y;
	}
	// check lower bound
	if (room.check_down) {
		if (gameCamera.y + gameCamera.viewportHeight > lower_right.y)
			gameCamera.y = lower_right.y - gameCamera.viewportHeight;
	}
}

// checks if entire entity is inside area described by box (rudimentary until engine team implements it)
bool MyGame::checkInsideBox(SDL_Rect box, DisplayObject* entity) {
	return (entity->position.x - entity->pivot.x >= box.x &&
		entity->position.x - entity->pivot.x + entity->width <= box.x + box.w &&
		entity->position.y - entity->pivot.y >= box.y &&
		entity->position.y - entity->pivot.y + entity->height <= box.y + box.h);
}

bool MyGame::checkInsidePoint(SDL_Point point, DisplayObject* entity) {
	return (point.x > entity->position.x - entity->pivot.x &&
		point.x < entity->position.x - entity->pivot.x + entity->width &&
		point.y > entity->position.y - entity->pivot.y &&
		point.y < entity->position.y - entity->pivot.y + entity->height);
}

void MyGame::handleEvent(Event* e) {
	room_state = curTransition.newstate;
	character->position = curTransition.dest;

	this->unlinkImmediateChild("foreground");
	this->setScene(sceneInfo[room_state].scenePointer);
	this->addChild(foreground);
	
	transLock = false;

	e->getSource()->removeEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);

	Tween* fadeIn = new Tween(blackBox);
	fadeIn->animate(TweenableParams::ALPHA, 255, 0, 30, TweenTransitions::LINEAR);
	juggler->add(fadeIn);
}

void MyGame::transitionScene() {
	transLock = true;

	//stops blood animation if transitioning scene
	character->bloodSplatter->alpha = 0;

	//Center blackbox at character
	blackBox->position.x = character->position.x - blackBox->width / 2;
	blackBox->position.y = character->position.y - blackBox->height / 2;

	Tween* fadeOut = new Tween(blackBox);
	fadeOut->animate(TweenableParams::ALPHA, 0, 255, 30, TweenTransitions::LINEAR);
	juggler->add(fadeOut);
	fadeOut->addEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
}

void MyGame::initTown() {
	townScene = new Scene();
	townScene->loadScene("./resources/scene/townScene.txt");
	sheriffScene = new Scene();
	sheriffScene->loadScene("./resources/scene/sherrifsoffice.txt");
	storeScene = new Scene();
	storeScene->loadScene("./resources/scene/generalstore.txt");
	hotelScene = new Scene();
	hotelScene->loadScene("./resources/scene/grandhotel.txt");
	bankScene = new Scene();
	bankScene->loadScene("./resources/scene/bank.txt");
	postScene = new Scene();
	postScene->loadScene("./resources/scene/postoffice.txt");
	cantinaScene = new Scene();
	cantinaScene->loadScene("./resources/scene/cantina.txt");
	drugScene = new Scene();
	drugScene->loadScene("./resources/scene/drugstore.txt");

	// initialize town transition points, hardcoded for now
	vector<TransitionStruct> townPoints = {
	//special reset transition
	TransitionStruct(SDL_Point{-1,-1}, SDL_Point{535, 960}, 0),

	// transitions to buildings
	TransitionStruct(SDL_Point{ 192, 300 }, SDL_Point{ 535, 900 }, 1),
	TransitionStruct(SDL_Point{550, 288}, SDL_Point{ 535, 900 }, 2),
	TransitionStruct(SDL_Point{900, 300}, SDL_Point{ 535, 900 }, 3),
	TransitionStruct(SDL_Point{1268, 300}, SDL_Point{ 535, 900 }, 4),
	TransitionStruct(SDL_Point{1632, 272}, SDL_Point{ 535, 900 }, 5),
	TransitionStruct(SDL_Point{2040, 300}, SDL_Point{ 535, 900 }, 6),
	TransitionStruct(SDL_Point{2333, 292}, SDL_Point{ 535, 900 }, 7),

	// transition to lake still
	TransitionStruct(SDL_Point{636, 964}, SDL_Point{ 550, 80 }, 8),

	// transition to rock canyon
	TransitionStruct(SDL_Point{115, 590}, SDL_Point{80, 625}, 19),

	// transition to badlands
	TransitionStruct(SDL_Point{2497, 964}, SDL_Point{406, 110}, 20),

	// transition to hideout
	TransitionStruct(SDL_Point{2867, 620}, SDL_Point{80, 660}, 26)
	};
	transitions.push_back(townPoints);

	// initialize town interior transition points, hardcoded for now
	vector<TransitionStruct> sheriffPoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{ 192, 360 }, 0) };
	transitions.push_back(sheriffPoint);
	vector<TransitionStruct> storePoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{550, 360}, 0) };
	transitions.push_back(storePoint);
	vector<TransitionStruct> hotelPoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{900, 360}, 0) };
	transitions.push_back(hotelPoint);
	vector<TransitionStruct> bankPoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{1268, 360}, 0) };
	transitions.push_back(bankPoint);
	vector<TransitionStruct> postPoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{1632, 360}, 0) };
	transitions.push_back(postPoint);
	vector<TransitionStruct> cantinaPoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{2040, 360}, 0) };
	transitions.push_back(cantinaPoint);
	vector<TransitionStruct> drugPoint = { TransitionStruct(SDL_Point{ 535, 960 }, SDL_Point{2333, 360}, 0) };
	transitions.push_back(drugPoint);

	// initilizing scene info
	sceneInfo.push_back(SceneInfo(townScene, SDL_Rect{ 0, 0, 3000, 1080 })); // 0
	sceneInfo.push_back(SceneInfo(sheriffScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 1
	sceneInfo.push_back(SceneInfo(storeScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 2
	sceneInfo.push_back(SceneInfo(hotelScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 3
	sceneInfo.push_back(SceneInfo(bankScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 4
	sceneInfo.push_back(SceneInfo(postScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 5
	sceneInfo.push_back(SceneInfo(cantinaScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 6
	sceneInfo.push_back(SceneInfo(drugScene, SDL_Rect{ 0, 0, 1080, 1080 })); // 7

	// add friendly NPCs to Town
	storekeeper = new DisplayObject("storekeeper", "./resources/friendlies/storekeeper.png");
	storeScene->addChild(storekeeper);
	storekeeper->position.x = 500;
	storekeeper->position.y = 500;
}

void MyGame::initLake() {
	lake1 = new Scene();
	lake1->loadScene("./resources/scene/lake1.txt");
	lake2 = new Scene();
	lake2->loadScene("./resources/scene/lake2.txt");
	lake3 = new Scene();
	lake3->loadScene("./resources/scene/lake3.txt");
	lake4 = new Scene();
	lake4->loadScene("./resources/scene/lake4.txt");
	lake5 = new Scene();
	lake5->loadScene("./resources/scene/lake5.txt");
	lake6 = new Scene();
	lake6->loadScene("./resources/scene/lake6.txt");
	lake7 = new Scene();
	lake7->loadScene("./resources/scene/lake7.txt");	
	lake8 = new Scene();
	lake8->loadScene("./resources/scene/lake8.txt");
	lake9 = new Scene();
	lake9->loadScene("./resources/scene/lake9.txt");

	// initialize lake transitions
	vector<TransitionStruct> lake1Points = {
	//transition back to town
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{720, 964}, 0, TransitionDetection::AXIS, Cardinal::NORTH),

	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 9, TransitionDetection::AXIS, Cardinal::EAST),
	TransitionStruct(SDL_Point{ 0, 595 }, SDL_Point{ 550, 80 }, 11, TransitionDetection::AXIS, Cardinal::SOUTH) };
	transitions.push_back(lake1Points);

	vector<TransitionStruct> lake2Points = {
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 10, TransitionDetection::AXIS, Cardinal::EAST),
	//TransitionStruct(SDL_Point{ 0, 595}, SDL_Point{ 550, 80 }, 12, TransitionDetection::AXIS, Cardinal::SOUTH),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 8, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake2Points);

	vector<TransitionStruct> lake3Points = {
	TransitionStruct(SDL_Point{ 0, 595 }, SDL_Point{ 550, 80 }, 13, TransitionDetection::AXIS, Cardinal::SOUTH),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 9, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake3Points);

	vector<TransitionStruct> lake4Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 8, TransitionDetection::AXIS, Cardinal::NORTH),
	//TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 12, TransitionDetection::AXIS, Cardinal::EAST),
	TransitionStruct(SDL_Point{ 0, 595 }, SDL_Point{ 550, 80 }, 14, TransitionDetection::AXIS, Cardinal::SOUTH) };
	transitions.push_back(lake4Points);

	vector<TransitionStruct> lake5Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 9, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 13, TransitionDetection::AXIS, Cardinal::EAST),
	TransitionStruct(SDL_Point{ 0, 595 }, SDL_Point{ 550, 80 }, 15, TransitionDetection::AXIS, Cardinal::SOUTH),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 11, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake5Points);

	vector<TransitionStruct> lake6Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 10, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 0, 595}, SDL_Point{ 550, 80 }, 16, TransitionDetection::AXIS, Cardinal::SOUTH),
	//TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 12, TransitionDetection::AXIS, Cardinal::WEST) 
	};
	transitions.push_back(lake6Points);

	vector<TransitionStruct> lake7Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 11, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 15, TransitionDetection::AXIS, Cardinal::EAST) };
	transitions.push_back(lake7Points);

	vector<TransitionStruct> lake8Points = {
	// TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 12, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 16, TransitionDetection::AXIS, Cardinal::EAST),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 14, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake8Points);

	vector<TransitionStruct> lake9Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 13, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 15, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake9Points);

	// initialize lake scene info
	sceneInfo.push_back(SceneInfo(lake1, SDL_Rect{ 0, 0, 1100, 610 })); // 8
	sceneInfo.push_back(SceneInfo(lake2, SDL_Rect{ 0, 0, 1100, 610 })); // 9
	sceneInfo.push_back(SceneInfo(lake3, SDL_Rect{ 0, 0, 1100, 610 })); // 10
	sceneInfo.push_back(SceneInfo(lake4, SDL_Rect{ 0, 0, 1100, 610 })); // 11
	sceneInfo.push_back(SceneInfo(lake5, SDL_Rect{ 0, 0, 1100, 610 })); // 12
	sceneInfo.push_back(SceneInfo(lake6, SDL_Rect{ 0, 0, 1100, 610 })); // 13
	sceneInfo.push_back(SceneInfo(lake7, SDL_Rect{ 0, 0, 1100, 610 })); // 14
	sceneInfo.push_back(SceneInfo(lake8, SDL_Rect{ 0, 0, 1100, 610 })); // 15
	sceneInfo.push_back(SceneInfo(lake9, SDL_Rect{ 0, 0, 1100, 610 })); // 16

}

void MyGame::initLakeEnemies(Scene* s) {
	if (s->id == "lake1" && !s->enemiesAdded) {
		// wolves
		wolf1LakeStill1 = new Wolf((Player*)character, "Wolf1");	// Adding wolf sprites
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		lake1->addChild(wolf1LakeStill1);
		wolf1LakeStill1->position = { 500, 500 };
		//wolf1LakeStill1->pivot = { wolf1LakeStill1->width / 2, wolf1LakeStill1->height / 2 };
		wolf1LakeStill1->scaleX = 0.75;
		wolf1LakeStill1->scaleY = 0.75;
		//wolf1LakeStill1->width = 90;
		wolf1LakeStill1->play("WolfLeft");

		wolf2LakeStill1 = new Wolf((Player*)character, "Wolf2");	// Adding wolf sprites
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		lake1->addChild(wolf2LakeStill1);
		wolf2LakeStill1->position = { 700, 500 };
		//wolf1LakeStill1->pivot = { wolf1LakeStill1->width / 2, wolf1LakeStill1->height / 2 };
		wolf2LakeStill1->scaleX = 0.75;
		wolf2LakeStill1->scaleY = 0.75;
		//wolf1LakeStill1->width = 90;
		wolf2LakeStill1->play("WolfLeft");
		s->enemiesAdded = true;
	}
	if (s->id == "lake2" && !s->enemiesAdded) {

		thug1LakeStill2 = new GangThug((Player*)character, "GangThug1");	
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		lake2->addChild(thug1LakeStill2);
		thug1LakeStill2->position = { 700, 500 };
		thug1LakeStill2->play("GangThugLeft");
		gang_thugs[thug1LakeStill2] = 1;

		thug2LakeStill2 = new GangThug((Player*)character, "GangThug2");	
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		lake2->addChild(thug2LakeStill2);
		thug2LakeStill2->position = { 700, 300 };
		thug2LakeStill2->play("GangThugLeft");
		gang_thugs[thug2LakeStill2] = 1;
		
		s->enemiesAdded = true;

	}
	if (s->id == "lake3" && !s->enemiesAdded) {
		mark1LakeStill3 = new GangMarksman((Player*)character, "GangMarksman1");	
		mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		lake3->addChild(mark1LakeStill3);
		mark1LakeStill3->position = { 500, 400 };
		mark1LakeStill3->scaleX = 0.75;
		mark1LakeStill3->scaleY = 0.75;
		mark1LakeStill3->play("GangMarksmanLeft");
		gang_marksmans[mark1LakeStill3] = 1;

		mark2LakeStill3 = new GangMarksman((Player*)character, "GangMarksman2");	
		mark2LakeStill3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		mark2LakeStill3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		mark2LakeStill3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		mark2LakeStill3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		lake3->addChild(mark2LakeStill3);
		mark2LakeStill3->position = { 300, 400 };
		mark2LakeStill3->scaleX = 0.75;
		mark2LakeStill3->scaleY = 0.75;
		mark2LakeStill3->play("GangMarksmanLeft");
		gang_marksmans[mark2LakeStill3] = 1;

		s->enemiesAdded = true;
	}
	if (s->id == "lake4" && !s->enemiesAdded) {
		arrow1LakeStill4 = new ArrowGuy((Player*)character, "Arrow1");
		arrow1LakeStill4->addAnimation("resources/enemies/", "Arrow", 1, 1, true);
		lake4->addChild(arrow1LakeStill4);
		arrow1LakeStill4->position = { 900, 150 };
		arrow1LakeStill4->pivot = { arrow1LakeStill4->width / 2, arrow1LakeStill4->height / 2 };
		arrow1LakeStill4->scaleX = 0.5;
		arrow1LakeStill4->scaleY = 0.5;
		arrow1LakeStill4->height = 400;
		arrow1LakeStill4->width = 250;
		arrow1LakeStill4->play("Arrow");
		arrow_guys[arrow1LakeStill4] = 1;

		arrow2LakeStill4 = new ArrowGuy((Player*)character, "Arrow2");
		arrow2LakeStill4->addAnimation("resources/enemies/", "Arrow", 1, 1, true);
		lake4->addChild(arrow2LakeStill4);
		arrow2LakeStill4->position = { 300, 300 };
		arrow2LakeStill4->pivot = { arrow2LakeStill4->width / 2, arrow2LakeStill4->height / 2 };
		arrow2LakeStill4->scaleX = 0.5;
		arrow2LakeStill4->scaleY = 0.5;
		arrow2LakeStill4->height = 400;
		arrow2LakeStill4->width = 250;
		arrow2LakeStill4->play("Arrow");
		arrow_guys[arrow2LakeStill4] = 1;

		s->enemiesAdded = true;
	}
	if (s->id == "lake6" && !s->enemiesAdded) {
		shot1LakeStill = new GangShot((Player*)character, "GangShot1");	
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		lake6->addChild(shot1LakeStill);
		shot1LakeStill->position = { 700, 300 };
		shot1LakeStill->play("GangShotLeft");
		gang_shot[shot1LakeStill] = 1;
		s->enemiesAdded = true;
	}
	if (s->id == "lake7" && !s->enemiesAdded) {
		creeper1LakeStill7 = new Creeper(character, "Creeper1");
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		lake7->addChild(creeper1LakeStill7);
		creeper1LakeStill7->position = { 300, 700 };
		creeper1LakeStill7->play("CreeperUp");

		creeper2LakeStill7 = new Creeper(character, "Creeper2");
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		lake7->addChild(creeper2LakeStill7);
		creeper2LakeStill7->position = { 700, 500 };
		creeper2LakeStill7->play("CreeperUp");
		
		s->enemiesAdded = true;

	}

	if (s->id == "lake8" && !s->enemiesAdded) {
		knifeguy1LakeStill8 = new KnifeGuy(character, "KnifeGuy1");
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		//creeper1LakeStill7->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		lake8->addChild(knifeguy1LakeStill8);
		knifeguy1LakeStill8->position = { 300, 300 };
		knifeguy1LakeStill8->play("KnifeGuyLeft");
		
		s->enemiesAdded = true;

	}
}

void MyGame::initCanyon() {
	canyon1 = new Scene();
	canyon1->loadScene("./resources/scene/canyon1.txt");
	canyon2 = new Scene();
	canyon2->loadScene("./resources/scene/canyon2.txt");
	canyon3 = new Scene();
	canyon3->loadScene("./resources/scene/canyon3.txt");

	// initialize canyon transitions
	vector<TransitionStruct> canyon1Points = {
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1000, 690 }, 18, TransitionDetection::AXIS, Cardinal::WEST)
	};
	transitions.push_back(canyon1Points);

	vector<TransitionStruct> canyon2Points = {
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1000, 690 }, 19, TransitionDetection::AXIS, Cardinal::WEST),
	TransitionStruct(SDL_Point{ 1065, 0 }, SDL_Point{ 80, 690 }, 17, TransitionDetection::AXIS, Cardinal::EAST)
	};
	transitions.push_back(canyon2Points);

	vector<TransitionStruct> canyon3Points = {
	TransitionStruct(SDL_Point{ 15, 625 }, SDL_Point{ 255, 590 }, 0),
	TransitionStruct(SDL_Point{ 1065, 0 }, SDL_Point{ 80, 690 }, 18, TransitionDetection::AXIS, Cardinal::EAST)
	};
	transitions.push_back(canyon3Points);

	// initialize canyon scene info
	sceneInfo.push_back(SceneInfo(canyon1, SDL_Rect{ 0, 0, 1080, 1080 })); // 17
	sceneInfo.push_back(SceneInfo(canyon2, SDL_Rect{ 0, 0, 1080, 1080 })); // 18
	sceneInfo.push_back(SceneInfo(canyon3, SDL_Rect{ 0, 0, 1080, 1080 })); // 19
}

void MyGame::initCanyonEnemies(Scene* s) {
	if (s->id == "canyon1" && !s->enemiesAdded) {
		openingText = new TextBox(SDL_Point{ 1500, 500 }, 400, 100, 20, 20, 20, 255);

		string openingTextText = "The man in black fled across the desert, and the gunslinger followed. \n -Stephen King, The Gunslinger";
		openingText->addTextLine("./resources/fonts/west.otf", openingTextText, 24, SDL_Color{ 255, 255, 255 });
		canyon1->addChild(openingText);
		openingText->position = { 500, 300 };
		if (!openingText->textLock)
			openingText->initBox();
		if (!openingText->textLock)
			openingText->closeBox();
		s->enemiesAdded = true;
	}
	if (s->id == "canyon2" && !s->enemiesAdded) {
		s->enemiesAdded = true;
	}
	if (s->id == "canyon3" && !s->enemiesAdded) {
		wolf1Canyon3 = new Wolf((Player*)character, "Wolf4");	// Adding wolf sprites
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		canyon3->addChild(wolf1Canyon3);
		wolf1Canyon3->position = { 200, 600 };
		wolf1Canyon3->scaleX = 0.75;
		wolf1Canyon3->scaleY = 0.75;
		wolf1Canyon3->play("WolfRight");

		wolf2Canyon3 = new Wolf((Player*)character, "Wolf5");	// Adding wolf sprites
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		canyon3->addChild(wolf2Canyon3);
		wolf2Canyon3->position = { 200, 700 };
		wolf2Canyon3->scaleX = 0.75;
		wolf2Canyon3->scaleY = 0.75;
		wolf2Canyon3->play("WolfRight");

		wolf3Canyon3 = new Wolf((Player*)character, "Wolf6");	// Adding wolf sprites
		wolf3Canyon3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf3Canyon3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf3Canyon3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf3Canyon3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		canyon3->addChild(wolf3Canyon3);
		wolf3Canyon3->position = { 200, 800 };
		wolf3Canyon3->scaleX = 0.75;
		wolf3Canyon3->scaleY = 0.75;
		wolf3Canyon3->play("WolfRight");
		s->enemiesAdded = true;
	}	
}

void MyGame::initBadlands() {
	badlands1 = new Scene();
	badlands1->loadScene("./resources/scene/badlands1.txt");
	badlands2 = new Scene();
	badlands2->loadScene("./resources/scene/badlands2.txt");
	badlands3 = new Scene();
	badlands3->loadScene("./resources/scene/badlands3.txt");
	badlands4 = new Scene();
	badlands4->loadScene("./resources/scene/badlands4.txt");
	badlands5 = new Scene();
	badlands5->loadScene("./resources/scene/badlands5.txt");
	badlands6 = new Scene();
	badlands6->loadScene("./resources/scene/badlands6.txt");

	vector<TransitionStruct> badlands1Points = {
		//transition back to town
		TransitionStruct(SDL_Point{406, 30}, SDL_Point{2330, 964}, 0),

		TransitionStruct(SDL_Point{510, 720}, SDL_Point{406, 110}, 21)
	};
	transitions.push_back(badlands1Points);

	vector<TransitionStruct> badlands2Points = {
	TransitionStruct(SDL_Point{932, 663}, SDL_Point{95, 680}, 22),
	TransitionStruct(SDL_Point{406, 40}, SDL_Point{450, 720}, 20)
	};
	transitions.push_back(badlands2Points);

	vector<TransitionStruct> badlands3Points = {
	TransitionStruct(SDL_Point{918, 85}, SDL_Point{38, 305}, 23),
	TransitionStruct(SDL_Point{15, 680}, SDL_Point{852, 663}, 21)
	};
	transitions.push_back(badlands3Points);

	vector<TransitionStruct> badlands4Points = {
	TransitionStruct(SDL_Point{358, 50}, SDL_Point{556, 117}, 24),
	TransitionStruct(SDL_Point{38, 373}, SDL_Point{918, 165}, 22)
	};
	transitions.push_back(badlands4Points);

	vector<TransitionStruct> badlands5Points = {
	TransitionStruct(SDL_Point{38, 47}, SDL_Point{198, 318}, 25),
	TransitionStruct(SDL_Point{556, 173}, SDL_Point{358, 100}, 23)
	};
	transitions.push_back(badlands5Points);

	vector<TransitionStruct> badlands6Points = {
	TransitionStruct(SDL_Point{198, 373}, SDL_Point{38, 100}, 24)
	};
	transitions.push_back(badlands6Points);

	// initialize badlands scene info
	sceneInfo.push_back(SceneInfo(badlands1, SDL_Rect{ 0, 0, 960, 768 })); // 20
	sceneInfo.push_back(SceneInfo(badlands2, SDL_Rect{ 0, 0, 960, 768 })); // 21
	sceneInfo.push_back(SceneInfo(badlands3, SDL_Rect{ 0, 0, 960, 768 })); // 22
	sceneInfo.push_back(SceneInfo(badlands4, SDL_Rect{ 0, 0, 400, 400 })); // 23
	sceneInfo.push_back(SceneInfo(badlands5, SDL_Rect{ 0, 0, 600, 200 })); // 24
	sceneInfo.push_back(SceneInfo(badlands6, SDL_Rect{ 0, 0, 400, 400 })); // 25
}

void MyGame::initHideout() {
	hideout1 = new Scene();
	hideout1->loadScene("./resources/scene/hideout1.txt");
	hideout2 = new Scene();
	hideout2->loadScene("./resources/scene/hideout2.txt");
	hideout3 = new Scene();
	hideout3->loadScene("./resources/scene/hideout3.txt");
	hideout4 = new Scene();
	hideout4->id = "hideout4";
	hideout4->loadScene("./resources/scene/hideout4.txt");
	hideout5 = new Scene();
	hideout5->loadScene("./resources/scene/hideout5.txt");
	hideout6 = new Scene();
	hideout6->loadScene("./resources/scene/hideout6.txt");
	hideout7 = new Scene();
	hideout7->loadScene("./resources/scene/hideout7.txt");
	hideout8 = new Scene();
	hideout8->loadScene("./resources/scene/hideout8.txt");

	// initialize transition points
	vector<TransitionStruct> hideout1Points = {
		// transition back to town
	TransitionStruct(SDL_Point{15, 0}, SDL_Point{2787, 620}, 0, TransitionDetection::AXIS, Cardinal::WEST),

	TransitionStruct(SDL_Point{1551, 656}, SDL_Point{657, 830}, 27)
	};
	transitions.push_back(hideout1Points);

	vector<TransitionStruct> hideout2Points = {
	TransitionStruct(SDL_Point{1892, 654}, SDL_Point{80, 650}, 28),
	TransitionStruct(SDL_Point{657, 992}, SDL_Point{1551, 720}, 26)
	};
	transitions.push_back(hideout2Points);

	vector<TransitionStruct> hideout3Points = {
	TransitionStruct(SDL_Point{1902, 578}, SDL_Point{80, 485}, 29),
	TransitionStruct(SDL_Point{15, 0}, SDL_Point{1812, 654}, 27, TransitionDetection::AXIS, Cardinal::WEST)
	};
	transitions.push_back(hideout3Points);

	vector<TransitionStruct> hideout4Points = {
	TransitionStruct(SDL_Point{15, 485}, SDL_Point{1822, 578}, 28),
	TransitionStruct(SDL_Point{683, 15}, SDL_Point{800, 920}, 30),
	TransitionStruct(SDL_Point{1065, 415}, SDL_Point{135, 470}, 31),
	TransitionStruct(SDL_Point{507, 1085}, SDL_Point{290, 165}, 32)
	};
	transitions.push_back(hideout4Points);

	vector<TransitionStruct> hideout5Points = {
	TransitionStruct(SDL_Point{800, 1000}, SDL_Point{683, 90}, 29)
	};
	transitions.push_back(hideout5Points);

	vector<TransitionStruct> hideout6Points = {
	TransitionStruct(SDL_Point{40, 470}, SDL_Point{985, 415}, 29)
	};
	transitions.push_back(hideout6Points);

	vector<TransitionStruct> hideout7Points = {
	TransitionStruct(SDL_Point{993, 82}, SDL_Point{598, 900}, 33),
	TransitionStruct(SDL_Point{290, 82}, SDL_Point{507, 1005}, 29)
	};
	transitions.push_back(hideout7Points);

	vector<TransitionStruct> hideout8Points = {
	TransitionStruct(SDL_Point{598, 1000}, SDL_Point{993, 160}, 32)
	};
	transitions.push_back(hideout8Points);

	// initialize hideout scene info
	sceneInfo.push_back(SceneInfo(hideout1, SDL_Rect{ 0, 0, 1920, 1080 })); // 26
	sceneInfo.push_back(SceneInfo(hideout2, SDL_Rect{ 0, 0, 1920, 1080 })); // 27
	sceneInfo.push_back(SceneInfo(hideout3, SDL_Rect{ 0, 0, 1920, 1080 })); // 28
	sceneInfo.push_back(SceneInfo(hideout4, SDL_Rect{ 0, 0, 1080, 1080 })); // 29
	sceneInfo.push_back(SceneInfo(hideout5, SDL_Rect{ 0, 0, 1080, 1080 })); // 30
	sceneInfo.push_back(SceneInfo(hideout6, SDL_Rect{ 0, 0, 1920, 1080 })); // 31
	sceneInfo.push_back(SceneInfo(hideout7, SDL_Rect{ 0, 0, 1080, 720 }));  // 32
	sceneInfo.push_back(SceneInfo(hideout8, SDL_Rect{ 0, 0, 1080, 1080 })); // 33
}

void MyGame::enemyShootingLoops() {
	// GANG THUG LOOP
	for (std::map<GangThug*, int>::iterator it=gang_thugs.begin(); it!=gang_thugs.end(); ++it) {
		if (it->first->health == 0) {
			it->first->clean = true;
			gang_thugs.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			benemy = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "revolver");
			benemy->distance = 20;
			this->addChild(benemy);
			benemy->position = {it->first->position.x, it->first->position.y };
			benemy->pivot = { benemy->width / 2, benemy->height / 2 };
			benemy->scaleX = 1;
			benemy->scaleY = 1;
			if (it->first->shots_fired == 5) {
				it->first->shoot -= 120;
				it->first->shots_fired = 0;
			} else{
				it->first->shoot -= 40;
				it->first->shots_fired ++;
			}
		}
	}
	for (std::map<GangShot*, int>::iterator it=gang_shot.begin(); it!=gang_shot.end(); ++it) {
		if (it->first->health == 0) {
			it->first->clean = true;
			gang_shot.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			benemya = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "shotgun");
			benemya->distance = 20;
			this->addChild(benemya);
			benemya->position = {it->first->position.x, it->first->position.y };
			benemya->pivot = { benemya->width / 2, benemya->height / 2 };
			benemya->scaleX = 1;
			benemya->scaleY = 1;
			if (it->first->shots_fired == 1) {
				it->first->shoot -= 300;
				it->first->shots_fired = 0;
			} else{
				it->first->shoot -= 100;
				it->first->shots_fired ++;
			}
		}
	}
	for (std::map<GangMarksman*, int>::iterator it=gang_marksmans.begin(); it!=gang_marksmans.end(); ++it) {
		if (it->first->health == 0) {
			it->first->clean = true;
			gang_marksmans.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			benemy2 = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 5, "rifle");
			benemy2->distance = 20;
			this->addChild(benemy2);
			benemy2->position = {it->first->position.x, it->first->position.y };
			benemy2->pivot = { benemy2->width / 2, benemy2->height / 2 };
			benemy2->scaleX = 1;
			benemy2->scaleY = 1;
			it->first->shoot -= 40;
		}
	}
	for (std::map<ArrowGuy*, int>::iterator it=arrow_guys.begin(); it!=arrow_guys.end(); ++it) {
		if (it->first->health == 0) {
			it->first->clean = true;
			arrow_guys.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			benemy3 = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 5, "arrow");
			benemy3->distance = 20;
			this->addChild(benemy3);
			benemy3->position = {it->first->position.x, it->first->position.y };
			benemy3->pivot = { benemy3->width / 2, benemy3->height / 2 };
			benemy3->scaleX = 1;
			benemy3->scaleY = 1;
			it->first->shoot -= 80;
		}
	}
}

void MyGame::playerShooting(int gun, string dir){
	if (gun == 1 && character->knife_throws > 0) {
	} else if (gun == 1) {
		bullet = new Projectile(dir,this->position, gun);
		//foreground->addChild(bullet);
		this->addChild(bullet);
		bullet->speed += 5;
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->knife_throws ++;
		character->dispatchEvent(character->updateAmmo);
	} else if (character->gun == 2 && character->revolver_shots > 5) {
	} else if (character->gun == 2) {
		bullet = new Projectile(dir,this->position, character->gun);
		bullet->id = "bullet" + projectileIdNum;
		projectileIdNum++;
		//foreground->addChild(bullet);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->revolver_shots ++;
		character->dispatchEvent(character->updateAmmo);
	} else if (character->gun == 3 && character->shotgun_shots > 1) {
	} else if (character->gun == 3) {
		bullet = new Projectile(dir,character->position, gun);
		bullet->id = "bullet" + projectileIdNum;
		projectileIdNum++;
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->shotgun_shots ++;
		character->dispatchEvent(character->updateAmmo);
	} else if (character->gun == 4 && character->rifle_shots > 4) {
	} else if (character->gun == 4) {
		bullet = new Projectile(dir,character->position, gun);
		bullet->id = "bullet" + projectileIdNum;
		projectileIdNum++;
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->rifle_shots ++;
		character->dispatchEvent(character->updateAmmo);
	}
}
void MyGame::reloadGun(int gun) {
	if (gun == 2) {
		character->revolver_shots = 0;
		character->dispatchEvent(character->updateAmmo);
	} else if (gun == 3) {
		character->shotgun_shots = 0;
		character->dispatchEvent(character->updateAmmo);
	} else if (gun == 4) {
		character->rifle_shots = 0;
		character->dispatchEvent(character->updateAmmo);
	}
}

void MyGame::checkTransition() {
	for (int i = 0; i < transitions[room_state].size(); i++) {
		TransitionStruct cur = transitions[room_state][i];
		if (cur.detection == TransitionDetection::POINT) {
			if (checkInsidePoint(cur.point, character)) {
				curTransition = cur;
				transitionScene();
				break;
			}
		}
		else if (cur.detection == TransitionDetection::AXIS) {
			if (cur.direction == Cardinal::NORTH) {
				if (character->position.y - character->pivot.y <= cur.point.y) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
			else if (cur.direction == Cardinal::EAST) {
				if (character->position.x - character->pivot.x + character->width >= cur.point.x) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
			else if (cur.direction == Cardinal::SOUTH) {
				if (character->position.y - character->pivot.y + character->height >= cur.point.y) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
			else if (cur.direction == Cardinal::WEST) {
				if (character->position.x - character->pivot.x <= cur.point.x) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
		}
	}
}

void MyGame::initObstacles() {
	Scene* s = sceneInfo[room_state].scenePointer;
	if (s->obstaclesAdded)
		return;

	string obs = "Obstacle";
	string riv = "River";
	string cac = "Cactus";

	SDL_Rect rect = sceneInfo[room_state].bounds.bounds;
	// initialize outer walls
	DisplayObjectContainer* tmpUp = new DisplayObjectContainer();
	DisplayObjectContainer* tmpDown = new DisplayObjectContainer();
	DisplayObjectContainer* tmpLeft = new DisplayObjectContainer();
	DisplayObjectContainer* tmpRight = new DisplayObjectContainer();

	tmpUp->type = obs;
	tmpDown->type = obs;
	tmpLeft->type = obs;
	tmpRight->type = obs;

	tmpUp->width = rect.w;
	tmpUp->height = 20;
	tmpUp->position = { 0, -20 };

	tmpDown->width = rect.w;
	tmpDown->height = 20;
	tmpDown->position = { 0, rect.h };

	tmpLeft->width = 20;
	tmpLeft->height = rect.h;
	tmpLeft->position = { -20, 0 };

	tmpRight->width = 20;
	tmpRight->height = rect.h;
	tmpRight->position = { rect.w, 0 };

	s->addChild(tmpUp);
	s->addChild(tmpDown);
	s->addChild(tmpLeft);
	s->addChild(tmpRight);

	if (s == lake2){
		// river
		addDOC(s, riv, 60, 110, 500, 0);
		addDOC(s, riv, 60, 350, 500, 250);
		addDOC(s, riv, 60, 300, 450, 350);
		addDOC(s, riv, 60, 250, 400, 400);
	}

	if (s == townScene) {
		// well
		addDOC(s, obs, 80, 85, 1375, 570);
		// skybox
		addDOC(s, obs, 3000, 300, 0, 0);
	}

	if (s == sheriffScene) {
		// wall
		addDOC(s, obs, 1100, 80, 0, 400);

		// benches
		addDOC(s, obs, 20, 130, 145, 570);
		addDOC(s, obs, 20, 130, 145, 830);
		addDOC(s, obs, 140, 30, 810, 635);
	}

	if (s == storeScene){
		// wall
		addDOC(s, obs, 1100, 100, 0, 400);

		// table
		addDOC(s, obs, 130, 140, 478, 500);
	}

	if (s == hotelScene){
		// wall
		addDOC(s, obs, 1100, 100, 0, 400);

		// table
		addDOC(s, obs, 130, 30, 478, 605);
	}

	if (s == bankScene){
		// wall
		addDOC(s, obs, 1100, 200, 0, 320);
	}

	if (s == postScene){
		// wall
		addDOC(s, obs, 570, 140, 0, 560);
		// top wall
		addDOC(s, obs, 1100, 120, 0, 0);

		// counter
		addDOC(s, obs, 60, 650, 500, 0);
	}

	if (s == cantinaScene){
		// top wall
		addDOC(s, obs, 1100, 100, 0, 0);

		// tables
		addDOC(s, obs, 45, 160, 250, 240);
		addDOC(s, obs, 45, 160, 500, 240);

		// bar
		addDOC(s, obs, 300, 130, 775, 100);
	}

	if (s == drugScene){
		// top wall
		addDOC(s, obs, 1100, 100, 0, 0);

		//shelves
		addDOC(s, obs, 525, 75, 0, 210);
		addDOC(s, obs, 525, 75, 0, 410);
		addDOC(s, obs, 525, 75, 0, 610);

		// counters
		addDOC(s, obs, 20, 180, 910, 200);
		addDOC(s, obs, 20, 180, 910, 470);
	}

	if (s == canyon1) {
		// skybox
		addDOC(s, obs, 1080, 300, 0, 0);

		// rocks
		addDOC(s, obs, 468, 124, 32, 335);
		addDOC(s, obs, 176, 216, 584, 339);
		addDOC(s, obs, 130, 88, 842, 439);
		addDOC(s, obs, 108, 142, 874, 839);
		addDOC(s, obs, 280, 126, 538, 881);
		addDOC(s, obs, 168, 112, 258, 907);
		addDOC(s, obs, 168, 100, 20, 833);
	}

	if (s == canyon2) {
		// skybox
		addDOC(s, obs, 1080, 300, 0, 0);

		// river boxes - really rough
		addDOC(s, riv, 110, 186, 440, 301);
		addDOC(s, riv, 92, 339, 440, 722);
		addDOC(s, riv, 59, 35, 423, 694);

		// bridge boxes - really rough
		addDOC(s, obs, 113, 20, 437, 510);
		addDOC(s, obs, 97, 20, 432, 662);
	}

	if (s == canyon3) {
		// skybox
		addDOC(s, obs, 1080, 300, 0, 0);

		// rocks
		addDOC(s, obs, 170, 115, 25, 343);
		addDOC(s, obs, 181, 214, 576, 338);
		addDOC(s, obs, 110, 146, 868, 837);
		addDOC(s, obs, 174, 116, 249, 905);
		addDOC(s, obs, 178, 106, 14, 825);
	}

	if (s == hideout1) {
		// skybox
		addDOC(s, obs, 1920, 245, 0, 0);

		// rocks and trees
		addDOC(s, obs, 143, 249, 94, 155);
		addDOC(s, obs, 212, 90, 978, 312);
		addDOC(s, obs, 127, 113, 927, 185);
		addDOC(s, obs, 296, 132, 1242, 832);
		addDOC(s, obs, 120, 108, 1247, 715);

		// building areas
		addDOC(s, obs, 188, 357, 1722, 374);
		addDOC(s, obs, 254, 385, 1458, 247);


		// river boxes - pretty rough
		addDOC(s, riv, 90, 45, 1632, 918);
		addDOC(s, riv, 88, 29, 851, 285);
		addDOC(s, riv, 84, 44, 881, 321);

		addDOC(s, riv, 158, 139, 704, 935);
		addDOC(s, riv, 124, 280, 688, 918);
		addDOC(s, riv, 110, 23, 700, 641);
		addDOC(s, riv, 110, 28, 710, 617);
		addDOC(s, riv, 110, 26, 722, 594);
		addDOC(s, riv, 110, 26, 736, 571);
		addDOC(s, riv, 98, 26, 766, 540);
		addDOC(s, riv, 70, 35, 799, 502);
		addDOC(s, riv, 70, 35, 810, 477);


		// bridge boxes - pretty rough
		addDOC(s, obs, 71, 28, 827, 455);
		addDOC(s, obs, 26, 19, 873, 481);
		addDOC(s, obs, 43, 28, 878, 342);
		addDOC(s, obs, 47, 26, 910, 354);
		addDOC(s, obs, 38, 26, 937, 375);
	}


	if (s == hideout2) {
		// wall
		addDOC(s, obs, 1920, 275, 0, 0);

		// piano
		addDOC(s, obs, 207, 198, 101, 782);

		// tables
		addDOC(s, obs, 135, 135, 701, 596);
		addDOC(s, obs, 135, 135, 887, 820);
		addDOC(s, obs, 135, 135, 1185, 605);
		addDOC(s, obs, 135, 135, 1390, 820);

		// bar/shelves
		addDOC(s, obs, 791, 280, 1037, 201);
	}

	if (s == hideout3) {
		// wall
		addDOC(s, obs, 1920, 350, 0, 0);

		// shelves
		addDOC(s, obs, 571, 236, 100, 165);
		addDOC(s, obs, 680, 82, 1161, 307);

		// tables
		addDOC(s, obs, 273, 163, 351, 478);
		addDOC(s, obs, 273, 163, 765, 810);
		addDOC(s, obs, 273, 163, 1158, 490);
		addDOC(s, obs, 273, 163, 1525, 810);

		// food bar
		addDOC(s, obs, 501, 94, 51, 909);
	}

	if (s == hideout4) {
		// walls
		addDOC(s, obs, 448, 540, 0, 540);
		addDOC(s, obs, 449, 434, 0, 0);
		addDOC(s, obs, 170, 356, 451, 0);
		addDOC(s, obs, 339, 358, 741, 0);
		addDOC(s, obs, 520, 599, 560, 481);
	}

	if (s == hideout5) {
		// cell wall
		addDOC(s, obs, 854, 136, 0, 387);

		// misc objects
		addDOC(s, obs, 153, 275, 887, 457);
		addDOC(s, obs, 141, 191, 908, 765);
		addDOC(s, obs, 141, 229, 49, 758);
		addDOC(s, obs, 205, 297, 369, 544);
	}

	if (s == hideout6) {
		// wall
		addDOC(s, obs, 1920, 356, 0, 0);

		// dressers
		addDOC(s, obs, 160, 75, 757, 347);
		addDOC(s, obs, 321, 75, 1599, 345);

		// beds
		addDOC(s, obs, 89, 198, 955, 377);
		addDOC(s, obs, 89, 198, 1115, 377);
		addDOC(s, obs, 89, 198, 1298, 377);
		addDOC(s, obs, 89, 198, 943, 834);
		addDOC(s, obs, 89, 198, 1113, 834);
		addDOC(s, obs, 89, 198, 1301, 834);

		// bathroom walls
		addDOC(s, obs, 294, 125, 0, 655);
		addDOC(s, obs, 151, 101, 411, 655);
		addDOC(s, obs, 90, 326, 470, 754);
	}

	if (s == hideout7) {
		// wall
		addDOC(s, obs, 1080, 100, 0, 0);
		// lower wall? - is this supposed to be a wall?
		addDOC(s, obs, 450, 423, 0, 297);

		// drawers
		addDOC(s, obs, 430, 58, 498, 77);

		// TV
		addDOC(s, obs, 146, 144, 838, 313);

		// chest table
		addDOC(s, obs, 94, 90, 576, 423);
	}

	if (s == hideout8) {
		// wall
		addDOC(s, obs, 1080, 224, 0, 0);

		// bookcases
		addDOC(s, obs, 270, 148, 0, 448);
		addDOC(s, obs, 280, 146, 234, 805);
		addDOC(s, obs, 226, 179, 689, 596);

		// iron maidens
		addDOC(s, obs, 120, 162, 129, 215);
		addDOC(s, obs, 120, 162, 823, 215);

		// table
		addDOC(s, obs, 122, 94, 475, 283);
	}

	if (s == badlands1) {
		// cacti
		addDOC(s, cac, 30, 30, 526, 72);
		addDOC(s, cac, 30, 30, 197, 214);
		addDOC(s, cac, 30, 30, 744, 237);
		addDOC(s, cac, 30, 30, 278, 353);
		addDOC(s, cac, 30, 30, 554, 356);
		addDOC(s, cac, 30, 30, 208, 521);
		addDOC(s, cac, 30, 30, 636, 582);

	}

	if (s == badlands2) {
		// cacti
		addDOC(s, cac, 30, 30, 526, 76);
		addDOC(s, cac, 30, 30, 450, 192);
		addDOC(s, cac, 30, 30, 343, 292);
		addDOC(s, cac, 30, 30, 279, 431);
		addDOC(s, cac, 30, 30, 524, 73);
		addDOC(s, cac, 30, 30, 769, 460);
		addDOC(s, cac, 30, 30, 636, 507);
		addDOC(s, cac, 30, 30, 636, 583);

	}

	if (s == badlands3) {
		// cacti
		addDOC(s, cac, 30, 30, 524, 73);
		addDOC(s, cac, 30, 30, 707, 176);
		addDOC(s, cac, 30, 30, 343, 293);
		addDOC(s, cac, 30, 30, 343, 293);
		addDOC(s, cac, 30, 30, 706, 379);
		addDOC(s, cac, 30, 30, 561, 389);
		addDOC(s, cac, 30, 30, 279, 432);
		addDOC(s, cac, 30, 30, 500, 452);
		addDOC(s, cac, 30, 30, 770, 460);
		addDOC(s, cac, 30, 30, 636, 510);

	}

	if (s == badlands4) {
		// tables
		addDOC(s, obs, 70, 50, 0, 20);
		addDOC(s, obs, 80, 80, 160, 134);
		addDOC(s, obs, 70, 50, 320, 300);

		//wall
		addDOC(s, obs, 400, 70, 0, -40);
		addDOC(s, obs, 400, 70, 0, 360);

	}

	if (s == badlands5) {
		//walls
		addDOC(s, obs, 600, 70, 0, -40);
		addDOC(s, obs, 600, 70, 0, 160);

	}

	if (s == badlands6) {
		//walls
		addDOC(s, obs, 400, 70, 0, -40);
		addDOC(s, obs, 400, 70, 0, 360);

	}

	s->obstaclesAdded = true;
}

void MyGame::addDOC(Scene* s, string type, int width, int height, int xpos, int ypos) {
	DisplayObjectContainer* tmp = new DisplayObjectContainer;
	tmp->type = type;
	tmp->width = width;
	tmp->height = height;
	tmp->position = SDL_Point{ xpos, ypos };
	s->addChild(tmp);
}