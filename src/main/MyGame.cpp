#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include "../engine/Controls.h"
#include "../engine/Player.h"
#include "MyGame.h"

using namespace std;

Camera DisplayObject::gameCamera;

MyGame::MyGame() : Game(gameCamera.viewportWidth, gameCamera.viewportHeight) {

	gameCamera = Camera();

	foreground = new DisplayObjectContainer();
	foreground->id = "foreground";

    character = new Player();
	foreground->addChild(character);

	character->position = { 1500, 500 };
	character->scaleX = 0.8;
	character->scaleY = 0.8;
	character->pivot = { character->width / 2, character->height / 2 };

	character->gun = 0;

	initTown();
	initLake();

	this->setScene(townScene);
	this->addChild(foreground);
	
	juggler = TweenJuggler::getInstance();

	room_state = 0;

	blackBox = new Sprite("blackbox",0,0,0);
	blackBox->alpha = 0;
	blackBox->width = 5000;
	blackBox->height = 5000;
	foreground->addChild(blackBox);

	test = new TextBox(SDL_Point{ 1500, 500 }, 300, 200);

	string testText = "This is test text. This is test text. This is test text. This is test text. This is test text. This is test text. This is test text. This is test text.";
	test->addTextLine("./resources/fonts/arial.ttf", testText, 18, SDL_Color{ 255, 255, 255 });
	string testText2 = "This is other text. This is other text. This is other text. This is other text. This is other text. This is other text. This is other text. This is other text.";
	test->addTextLine("./resources/fonts/arial.ttf", testText2, 18, SDL_Color{ 255, 50, 50 });
	string testText3 = "Deus volt";
	test->addTextLine("./resources/fonts/arial.ttf", testText3, 18, SDL_Color{ 50, 50, 255 });
	string testText4 = "Lorem ipsum.";
	test->addTextLine("./resources/fonts/arial.ttf", testText4, 18, SDL_Color{ 50, 255, 50 });

	foreground->addChild(test);

	healthBackground = new Sprite("blackbox", 255, 0, 0);
	healthBackground->id = "healthbackground";
	healthBackground->position = { -50,-65 };
	healthBackground->width = 100;
	healthBackground->height = 20;
	playerHealth = new HealthBar(character, 0, 100);
	character->addChild(healthBackground);
	character->addChild(playerHealth);
}

MyGame::~MyGame() {
	
}


void MyGame::update(set<SDL_Scancode> pressedKeys) {
	controls::update(pressedKeys);

	//Demo trigger for taking damage to show health bar depletion
	if(controls::holdSpace()){
		character->takeDamage(1);
	}

	if (!transLock) {
		// gun select
		if (controls::press1()) {
			character->gun = 0;
		};
		if (controls::press2()) {
			character->gun = 1;
		};
		if (controls::press3()) {
			character->gun = 2;
		};
		if (controls::press4()) {
			character->gun = 3;
		};
		if (controls::press5()) {
			character->gun = 4;
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

		if (controls::toggleVisibility() && !test->textLock) {
			if (test->nextLine == 0)
				test->initBox();
			else if (test->nextLine == test->maxLine)
				test->closeBox();
			else
				test->drawNextLine();
		}
	}
	
	gameCamera.x = character->position.x - gameCamera.viewportWidth / 2;
	gameCamera.y = character->position.y - gameCamera.viewportHeight / 2;

	test->position = { character->position.x - test->background->width / 2, character->position.y - 300 };

	Game::update(pressedKeys);
	controls::update(pressedKeys);

	if (!transLock) {
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


	enforceCameraBounds();
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
	if (curScene != NULL)
		this->addChild(curScene);
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
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{720, 964}, 0, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 9, TransitionDetection::AXIS, Cardinal::EAST),
	TransitionStruct(SDL_Point{ 0, 595 }, SDL_Point{ 550, 80 }, 11, TransitionDetection::AXIS, Cardinal::SOUTH) };
	transitions.push_back(lake1Points);

	vector<TransitionStruct> lake2Points = {
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 10, TransitionDetection::AXIS, Cardinal::EAST),
	TransitionStruct(SDL_Point{ 0, 595}, SDL_Point{ 550, 80 }, 12, TransitionDetection::AXIS, Cardinal::SOUTH),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 8, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake2Points);

	vector<TransitionStruct> lake3Points = {
	TransitionStruct(SDL_Point{ 0, 595 }, SDL_Point{ 550, 80 }, 13, TransitionDetection::AXIS, Cardinal::SOUTH),
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 9, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake3Points);

	vector<TransitionStruct> lake4Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 8, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 12, TransitionDetection::AXIS, Cardinal::EAST),
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
	TransitionStruct(SDL_Point{ 15, 0 }, SDL_Point{ 1020, 305 }, 12, TransitionDetection::AXIS, Cardinal::WEST) };
	transitions.push_back(lake6Points);

	vector<TransitionStruct> lake7Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 11, TransitionDetection::AXIS, Cardinal::NORTH),
	TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 15, TransitionDetection::AXIS, Cardinal::EAST) };
	transitions.push_back(lake7Points);

	vector<TransitionStruct> lake8Points = {
	TransitionStruct(SDL_Point{ 0, 15 }, SDL_Point{ 550, 530 }, 12, TransitionDetection::AXIS, Cardinal::NORTH),
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
}

void MyGame::playerShooting(int gun, string dir){
	if (gun == 1 && character->knife_throws > 0) {
	} else if (gun == 1) {
		bullet = new Projectile(dir,this->position, gun);
		this->addChild(bullet);
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