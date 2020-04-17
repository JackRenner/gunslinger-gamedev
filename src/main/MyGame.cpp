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

    character = new Player();
	//this->removeImmediateChild(character);

	character->position = { 1500, 500 };
	character->scaleX = 0.8;
	character->scaleY = 0.8;
	character->pivot = { character->width / 2, character->height / 2 };

	character->gun = 0;

	initTown();
	initLake();

	room_state = 0;

	this->setScene(townScene);
	this->addChild(foreground);
	
	juggler = TweenJuggler::getInstance();

	blackBox = new Sprite("blackbox",0,0,0);
	blackBox->alpha = 0;
	blackBox->width = 5000;
	blackBox->height = 5000;
	foreground->addChild(blackBox);

	// test = new TextBox(SDL_Point{ 1500, 500 }, 300, 200);

	// string testText = "This is test text. This is test text. This is test text. This is test text. This is test text. This is test text. This is test text. This is test text.";
	// test->addTextLine("./resources/fonts/arial.ttf", testText, 18, SDL_Color{ 255, 255, 255 });
	// string testText2 = "This is other text. This is other text. This is other text. This is other text. This is other text. This is other text. This is other text. This is other text.";
	// test->addTextLine("./resources/fonts/arial.ttf", testText2, 18, SDL_Color{ 255, 50, 50 });
	// string testText3 = "Deus volt";
	// test->addTextLine("./resources/fonts/arial.ttf", testText3, 18, SDL_Color{ 50, 50, 255 });
	// string testText4 = "Lorem ipsum.";
	// test->addTextLine("./resources/fonts/arial.ttf", testText4, 18, SDL_Color{ 50, 255, 50 });

	// foreground->addChild(test);
	
	selection = new WeaponSelect();
	selection->position = { character->position.x - (gameCamera.viewportWidth / 2) + 10, (character->position.y - gameCamera.viewportHeight / 2 + 10) };

	healthBackground = new Sprite("blackbox", 255, 0, 0);
	healthBackground->id = "healthbackground";
	healthBackground->position = { -50,-65 };
	healthBackground->width = 100;
	healthBackground->height = 20;
	playerHealth = new HealthBar(character, 0, 100);

	character->addEventListener(selection, WeaponSelectEvent::SELECT_FIST_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_KNIFE_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_PISTOL_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_SHOTGUN_EVENT);
	character->addEventListener(selection, WeaponSelectEvent::SELECT_RIFLE_EVENT);

	foreground->addChild(character);
	character->addChild(healthBackground);
	character->addChild(playerHealth);
	foreground->addChild(selection);
	//foreground->addChild(test);
	foreground->addChild(blackBox);
}

MyGame::~MyGame() {

}


void MyGame::update(set<SDL_Scancode> pressedKeys) {
	controls::update(pressedKeys);

	this->saveAllPositions();

	if (character->health == 0) {
		// add in code to reset to town
		character->health = 100;
	}
	// Demo for enemies

	// need to make these for loops that loop through for each type of enemy

	// GANG THUG LOOP
	for (std::map<GangThug*, int>::iterator it=gang_thugs.begin(); it!=gang_thugs.end(); ++it) {
		if (it->first->health == 0) {
			it->first->clean = true;
			gang_thugs.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			benemy = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "bullet");
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
		
	if(mark1LakeStill3->shoot > 0) {
		benemy2 = new Benemy((AnimatedSprite*)mark1LakeStill3, character->position.x, character->position.y, 5, "bullet");
		benemy2->distance = 20;
		this->addChild(benemy2);
		benemy2->position = {mark1LakeStill3->position.x, mark1LakeStill3->position.y };
		benemy2->pivot = { benemy2->width / 2, benemy2->height / 2 };
		benemy2->scaleX = 1;
		benemy2->scaleY = 1;
		mark1LakeStill3->shoot -= 80;
	}
	if(arrow1LakeStill4->shoot > 0) {
		benemy3 = new Benemy((AnimatedSprite*)arrow1LakeStill4, character->position.x, character->position.y, 5, "Arrow90");
		benemy3->distance = 20;
		this->addChild(benemy3);
		benemy3->position = {arrow1LakeStill4->position.x, arrow1LakeStill4->position.y };
		benemy3->pivot = { benemy3->width / 2, benemy3->height / 2 };
		benemy3->scaleX = 1;
		benemy3->scaleY = 1;
		arrow1LakeStill4->shoot -= 80;
	}
	
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

		// if (controls::toggleVisibility() && !test->textLock) {
		// 	if (test->nextLine == 0)
		// 		test->initBox();
		// 	else if (test->nextLine == test->maxLine)
		// 		test->closeBox();
		// 	else
		// 		test->drawNextLine();
		// }
	}
	//test->position = { character->position.x - test->background->width / 2, character->position.y - 300 };

	Game::update(pressedKeys);
	controls::update(pressedKeys);

	gameCamera.x = character->position.x - gameCamera.viewportWidth / 2;
	gameCamera.y = character->position.y - gameCamera.viewportHeight / 2;

	if (!transLock) {
		checkTransition();
	}
	// need to write code for sending player back to town
	if (character->health <= 0) {
		// character->position = { 1500, 500 };
		// this->setScene(townScene);
		// character->health = 100;
		// initLake();
		// initTown();
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
	std::cout << "SCENE CHANGEEEEEE\n";
	if (curScene != NULL)
		this->unlinkImmediateChild(curScene->id);
	this->curScene = scene;
	if (curScene != NULL) {
		this->addChild(curScene);
		initEnemies(scene);
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

// checks if entire entity is inside area described by box (rudimentary "collision" until engine team implements it)
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
	TransitionStruct(SDL_Point{ 192, 300 }, SDL_Point{ 535, 900 }, 1),
	TransitionStruct(SDL_Point{550, 288}, SDL_Point{ 535, 900 }, 2),
	TransitionStruct(SDL_Point{900, 300}, SDL_Point{ 535, 900 }, 3),
	TransitionStruct(SDL_Point{1268, 300}, SDL_Point{ 535, 900 }, 4),
	TransitionStruct(SDL_Point{1632, 272}, SDL_Point{ 535, 900 }, 5),
	TransitionStruct(SDL_Point{2040, 300}, SDL_Point{ 535, 900 }, 6),
	TransitionStruct(SDL_Point{2333, 292}, SDL_Point{ 535, 900 }, 7),
	TransitionStruct(SDL_Point{636, 964}, SDL_Point{ 550, 80 }, 8)
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
	//this->addChild(lake1);
	lake2 = new Scene();
	lake2->loadScene("./resources/scene/lake2.txt");
	//foreground->addChild(lake2);
	lake3 = new Scene();
	lake3->loadScene("./resources/scene/lake3.txt");
	//this->addChild(lake3);
	lake4 = new Scene();
	lake4->loadScene("./resources/scene/lake4.txt");
	//this->addChild(lake4);	
	lake5 = new Scene();
	lake5->loadScene("./resources/scene/lake5.txt");
	//this->addChild(lake5);	
	lake6 = new Scene();
	lake6->loadScene("./resources/scene/lake6.txt");
	//this->addChild(lake6);	
	lake7 = new Scene();
	lake7->loadScene("./resources/scene/lake7.txt");
	//this->addChild(lake7);	
	lake8 = new Scene();
	lake8->loadScene("./resources/scene/lake8.txt");
	//this->addChild(lake8);	
	lake9 = new Scene();
	lake9->loadScene("./resources/scene/lake9.txt");
	//this->addChild(lake9);	

	// initialize lake transitions
	vector<TransitionStruct> lake1Points = {
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
	// TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 12, TransitionDetection::AXIS, Cardinal::EAST),
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
	// TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 12, TransitionDetection::AXIS, Cardinal::WEST)
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

	// initialize scene info
	sceneInfo.push_back(SceneInfo(lake1, SDL_Rect{ 0, 0, 1100, 610 })); // 8
	sceneInfo.push_back(SceneInfo(lake2, SDL_Rect{ 0, 0, 1100, 610 })); // 9
	sceneInfo.push_back(SceneInfo(lake3, SDL_Rect{ 0, 0, 1100, 610 })); // 10
	sceneInfo.push_back(SceneInfo(lake4, SDL_Rect{ 0, 0, 1100, 610 })); // 11
	sceneInfo.push_back(SceneInfo(lake5, SDL_Rect{ 0, 0, 1100, 610 })); // 12
	sceneInfo.push_back(SceneInfo(lake6, SDL_Rect{ 0, 0, 1100, 610 })); // 13
	sceneInfo.push_back(SceneInfo(lake7, SDL_Rect{ 0, 0, 1100, 610 })); // 14
	sceneInfo.push_back(SceneInfo(lake8, SDL_Rect{ 0, 0, 1100, 610 })); // 15
	sceneInfo.push_back(SceneInfo(lake9, SDL_Rect{ 0, 0, 1100, 610 })); // 16

	// initialize lake still enemies

	// wolf2LakeStill1 = new Wolf((Player*)character);	
	// wolf2LakeStill1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
	// wolf2LakeStill1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
	// wolf2LakeStill1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
	// wolf2LakeStill1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
	// lake1->addChild(wolf2LakeStill1);
	// wolf2LakeStill1->position = { 700, 500 };
	// wolf2LakeStill1->pivot = { wolf2LakeStill1->width / 2, wolf2LakeStill1->height / 2 };
	// wolf2LakeStill1->scaleX = 0.5;
	// wolf2LakeStill1->scaleY = 0.5;
	// wolf2LakeStill1->width = 50;
	// wolf2LakeStill1->play("WolfLeft");

	mark1LakeStill3 = new GangMarksman((Player*)character);	
	mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
	mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
	mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
	mark1LakeStill3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
	lake3->addChild(mark1LakeStill3);
	mark1LakeStill3->position = { 500, 400 };
	mark1LakeStill3->pivot = { mark1LakeStill3->width / 2, mark1LakeStill3->height / 2 };
	mark1LakeStill3->scaleX = 0.5;
	mark1LakeStill3->scaleY = 0.5;
	mark1LakeStill3->width = 90;
	mark1LakeStill3->play("GangMarksmanLeft");

	arrow1LakeStill4 = new ArrowGuy((Player*)character);
	arrow1LakeStill4->addAnimation("resources/enemies/", "Arrow", 1, 1, true);
	lake4->addChild(arrow1LakeStill4);
	arrow1LakeStill4->position = { 900, 150 };
	arrow1LakeStill4->pivot = { arrow1LakeStill4->width / 2, arrow1LakeStill4->height / 2 };
	arrow1LakeStill4->scaleX = 0.5;
	arrow1LakeStill4->scaleY = 0.5;
	arrow1LakeStill4->height = 400;
	arrow1LakeStill4->width = 250;
	arrow1LakeStill4->play("Arrow");
}

void MyGame::initEnemies(Scene* s) {
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

		thug1LakeStill2 = new GangThug((Player*)character);	
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		lake2->addChild(thug1LakeStill2);
		thug1LakeStill2->position = { 500, 500 };
		thug1LakeStill2->play("GangThugLeft");
		gang_thugs[thug1LakeStill2] = 1;
		
		s->enemiesAdded = true;

	}
	if (s->id == "lake7" && !s->enemiesAdded) {
		creeper1LakeStill7 = new Creeper(character);
		creeper1LakeStill7->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		std::cout << creeper1LakeStill7->getAnimation("Explode") << endl;
		lake7->addChild(creeper1LakeStill7);
		creeper1LakeStill7->position = { 500, 500 };
		creeper1LakeStill7->play("GangThugLeft");
		//gang_thugs[creeper1LakeStill7] = 1;
		
		s->enemiesAdded = true;

	}
}

void MyGame::playerShooting(int gun, string dir){
	if (gun == 1 && character->knife_throws > 0) {
	} else if (gun == 1) {
		bullet = new Projectile(dir,this->position, gun);
		this->addChild(bullet);
		bullet->speed += 5;
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->knife_throws ++;
	} else if (character->gun == 2 && character->revolver_shots > 5) {
	} else if (character->gun == 2) {
		bullet = new Projectile(dir,this->position, character->gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->revolver_shots ++;
	} else if (character->gun == 3 && character->shotgun_shots > 1) {
	} else if (character->gun == 3) {
		bullet = new Projectile(dir,character->position, gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->shotgun_shots ++;
	} else if (character->gun == 4 && character->rifle_shots > 4) {
	} else if (character->gun == 4) {
		bullet = new Projectile(dir,character->position, gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		character->rifle_shots ++;
	}
}
void MyGame::reloadGun(int gun) {
	if (gun == 2) {
		character->revolver_shots = 0;
	} else if (gun == 3) {
		character->shotgun_shots = 0;
	} else if (gun == 4) {
		character->rifle_shots = 0;
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
	cout << "Initializing Obstacles!" << endl;
	Scene* scenePointer = sceneInfo[room_state].scenePointer;
	if (scenePointer->obstaclesAdded)
		return;

	SDL_Rect rect = sceneInfo[room_state].bounds.bounds;
	// initialize outer walls
	DisplayObjectContainer* tmpUp = new DisplayObjectContainer();
	DisplayObjectContainer* tmpDown = new DisplayObjectContainer();
	DisplayObjectContainer* tmpLeft = new DisplayObjectContainer();
	DisplayObjectContainer* tmpRight = new DisplayObjectContainer();

	tmpUp->type = "Obstacle";
	tmpDown->type = "Obstacle";
	tmpLeft->type = "Obstacle";
	tmpRight->type = "Obstacle";

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

	scenePointer->addChild(tmpUp);
	scenePointer->addChild(tmpDown);
	scenePointer->addChild(tmpLeft);
	scenePointer->addChild(tmpRight);

	if (scenePointer == lake2){
		DisplayObjectContainer* river1 = new DisplayObjectContainer();
		river1->type = "Obstacle";
		river1->width = 60;
		river1->height = 110;
		river1->position = {500,0};
		scenePointer->addChild(river1);

		DisplayObjectContainer* river2 = new DisplayObjectContainer();
		river2->type = "Obstacle";
		river2->width = 60;
		river2->height = 350;
		river2->position = {500,250};
		scenePointer->addChild(river2);

		DisplayObjectContainer* river3 = new DisplayObjectContainer();
		river3->type = "Obstacle";
		river3->width = 60;
		river3->height = 300;
		river3->position = {450,350};
		scenePointer->addChild(river3);

		DisplayObjectContainer* river4 = new DisplayObjectContainer();
		river4->type = "Obstacle";
		river4->width = 60;
		river4->height = 250;
		river4->position = {400,400};
		scenePointer->addChild(river4);
	}

	if (scenePointer == townScene){
		DisplayObjectContainer* well = new DisplayObjectContainer();
		well->type = "Obstacle";
		well->width = 80;
		well->height = 85;
		well->position = {1375,570};
		scenePointer->addChild(well);
	}

	if (scenePointer == sheriffScene){
		cout << "\nENTERING SHERIFF\n" << endl;
		DisplayObjectContainer* wall = new DisplayObjectContainer();
		wall->type = "Obstacle";
		wall->width = 1100;
		wall->height = 80;
		wall->position = {0,400};
		scenePointer->addChild(wall);

		DisplayObjectContainer* bench1 = new DisplayObjectContainer();
		bench1->type = "Obstacle";
		bench1->width = 20;
		bench1->height = 130;
		bench1->position = {145,570};
		scenePointer->addChild(bench1);

		DisplayObjectContainer* bench2 = new DisplayObjectContainer();
		bench2->type = "Obstacle";
		bench2->width = 20;
		bench2->height = 130;
		bench2->position = {145,830};
		scenePointer->addChild(bench2);

		DisplayObjectContainer* bench3 = new DisplayObjectContainer();
		bench3->type = "Obstacle";
		bench3->width = 140;
		bench3->height = 30;
		bench3->position = {810,635};
		scenePointer->addChild(bench3);
	}

	if (scenePointer == storeScene){
		cout << "\nENTERING STORE\n" << endl;
		DisplayObjectContainer* wall = new DisplayObjectContainer();
		wall->type = "Obstacle";
		wall->width = 1100;
		wall->height = 100;
		wall->position = {0,400};
		scenePointer->addChild(wall);

		DisplayObjectContainer* table = new DisplayObjectContainer();
		table->type = "Obstacle";
		table->width = 130;
		table->height = 140;
		table->position = {478,500};
		scenePointer->addChild(table);
	}

	if (scenePointer == hotelScene){
		cout << "\nENTERING HOTEL\n" << endl;
		DisplayObjectContainer* wall = new DisplayObjectContainer();
		wall->type = "Obstacle";
		wall->width = 1100;
		wall->height = 100;
		wall->position = {0,400};
		scenePointer->addChild(wall);

		DisplayObjectContainer* table = new DisplayObjectContainer();
		table->type = "Obstacle";
		table->width = 130;
		table->height = 30;
		table->position = {478,605};
		scenePointer->addChild(table);
	}

	if (scenePointer == bankScene){
		cout << "\nENTERING BANK\n" << endl;
		DisplayObjectContainer* wall = new DisplayObjectContainer();
		wall->type = "Obstacle";
		wall->width = 1100;
		wall->height = 200;
		wall->position = {0,320};
		scenePointer->addChild(wall);
	}

	if (scenePointer == postScene){
		cout << "\nENTERING POST\n" << endl;
		DisplayObjectContainer* wall = new DisplayObjectContainer();
		wall->type = "Obstacle";
		wall->width = 570;
		wall->height = 140;
		wall->position = {0,560};
		scenePointer->addChild(wall);

		DisplayObjectContainer* topWall = new DisplayObjectContainer();
		topWall->type = "Obstacle";
		topWall->width = 1100;
		topWall->height = 120;
		topWall->position = {0,0};
		scenePointer->addChild(topWall);

		DisplayObjectContainer* counter = new DisplayObjectContainer();
		counter->type = "Obstacle";
		counter->width = 60;
		counter->height = 650;
		counter->position = {500,0};
		scenePointer->addChild(counter);
	}

	if (scenePointer == cantinaScene){
		cout << "\nENTERING CANTINA\n" << endl;
		DisplayObjectContainer* topWall = new DisplayObjectContainer();
		topWall->type = "Obstacle";
		topWall->width = 1100;
		topWall->height = 100;
		topWall->position = {0,0};
		scenePointer->addChild(topWall);

		DisplayObjectContainer* table1 = new DisplayObjectContainer();
		table1->type = "Obstacle";
		table1->width = 45;
		table1->height = 160;
		table1->position = {250,240};
		scenePointer->addChild(table1);

		DisplayObjectContainer* table2 = new DisplayObjectContainer();
		table2->type = "Obstacle";
		table2->width = 45;
		table2->height = 160;
		table2->position = {500,240};
		scenePointer->addChild(table2);

		DisplayObjectContainer* bar = new DisplayObjectContainer();
		bar->type = "Obstacle";
		bar->width = 300;
		bar->height = 130;
		bar->position = {775,100};
		scenePointer->addChild(bar);
	}

	if (scenePointer == drugScene){
		cout << "\nENTERING DRUG STORE\n" << endl;
		DisplayObjectContainer* topWall = new DisplayObjectContainer();
		topWall->type = "Obstacle";
		topWall->width = 1100;
		topWall->height = 100;
		topWall->position = {0,0};
		scenePointer->addChild(topWall);

		DisplayObjectContainer* shelf1 = new DisplayObjectContainer();
		shelf1->type = "Obstacle";
		shelf1->width = 525;
		shelf1->height = 75;
		shelf1->position = {0,210};
		scenePointer->addChild(shelf1);

		DisplayObjectContainer* shelf2 = new DisplayObjectContainer();
		shelf2->type = "Obstacle";
		shelf2->width = 525;
		shelf2->height = 75;
		shelf2->position = {0,410};
		scenePointer->addChild(shelf2);

		DisplayObjectContainer* shelf3 = new DisplayObjectContainer();
		shelf3->type = "Obstacle";
		shelf3->width = 525;
		shelf3->height = 75;
		shelf3->position = {0,610};
		scenePointer->addChild(shelf3);

		DisplayObjectContainer* bench1 = new DisplayObjectContainer();
		bench1->type = "Obstacle";
		bench1->width = 20;
		bench1->height = 180;
		bench1->position = {910,200};
		scenePointer->addChild(bench1);

		DisplayObjectContainer* bench2 = new DisplayObjectContainer();
		bench2->type = "Obstacle";
		bench2->width = 20;
		bench2->height = 180;
		bench2->position = {910,470};
		scenePointer->addChild(bench2);
	}

	scenePointer->obstaclesAdded = true;
}