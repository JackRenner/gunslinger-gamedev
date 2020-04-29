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

	character->position = { 200, 600 };
	character->scaleX = 0.8;
	character->scaleY = 0.8;
	character->pivot = { character->width / 2, character->height / 2 };

	character->gun = 0;

	initTown();
	initLake();
	initCanyon();
	initBadlands();
	initHideout();

	room_state = 17;

	this->setScene(canyon1);
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
	character->addEventListener(selection, WeaponSelectEvent::PLAYER_HEAL);

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


	// code to check if area is complete
	if (curScene->id == "canyon3" && curScene->enemiesLeft == 0){
		character->canyonComplete = true;
	}
	if (curScene->id == "lake4" && curScene->enemiesLeft == 0) {
		character->lakeComplete = true;
	}
	if (curScene->id == "badlands6" && curScene->enemiesLeft == 0) {
		character->badlandsComplete = true;
	}
	if (curScene->id == "hideout8" && curScene->enemiesLeft == 0) {
		character->hideoutComplete = true;
	}

	// code to reset scene
	if (character->health <= 0) {
		curTransition = transitions[0][0];
		transitionScene();
	}

	this->enemyShootingLoops();

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
			this->reloadTime = 300;
		}
		if (controls::pressDown()) {
			//gunshot->playSFX();
			this->playerShooting(character->gun, "down");
			this->reloadTime = 300;

		}
		if (controls::pressRight()) {
			//gunshot->playSFX();	
			this->playerShooting(character->gun, "left");
			this->reloadTime = 300;

		}
		if (controls::pressLeft()) {
			//gunshot->playSFX();	
			this->playerShooting(character->gun, "right");
			this->reloadTime = 300;

		}
		// there isn't any delay from reloading yet
		if (controls::pressR() && (this->reloadTime == 300 || this->reloadTime == 250 || this->reloadTime == 100)) {
			this->reloadTime = 0;
			//this->reloadGun(character->gun);
		}
		if (controls::pressF() && character->health < 500) {
			character->heal("whiskey");
			character->foodNum -=1;
			character->selectWeapon(5);

		}
	}

	// reloading per frame
	if (this->reloadTime < 300 && character->gun == 2) {
		if (this->character->revolver_shots == 0) {
			this->reloadTime = 300;
		}
		else if (this->reloadTime % 50 == 0) {
			character->revolver_shots --;
			character->dispatchEvent(character->updateAmmo);
			this->reloadTime ++;
		} else {
			this->reloadTime ++;
		}
	} else if (this->reloadTime < 100 && character->gun == 3) {
		if (this->character->shotgun_shots == 0) {
			this->reloadTime = 100;
		}
		else if (this->reloadTime % 50 == 0) {
			character->shotgun_shots --;
			character->dispatchEvent(character->updateAmmo);
			this->reloadTime ++;
		} else {
			this->reloadTime ++;
		}
	} else if (this->reloadTime < 250 && character->gun == 4) {
		if (this->character->rifle_shots == 0) {
			this->reloadTime = 250;
		}
		else if (this->reloadTime % 50 == 0) {
			character->rifle_shots --;
			character->dispatchEvent(character->updateAmmo);
			this->reloadTime ++;
		} else {
			this->reloadTime ++;
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
		if (scene->id.substr(0,4) == "lake") {
			initLakeEnemies(scene);
			// if(currentMusic != lakeMusic){
			// 	lakeMusic->play();
			// 	currentMusic = lakeMusic;
			// }
		}
		else if (scene->id.substr(0,4) == "badl") {
			initBadlandsEnemies(scene);
		}
		else if (scene->id.substr(0,4) == "cany") {
			initCanyonEnemies(scene);
		}
		else if (scene->id.substr(0,4) == "hide") {
			initHideoutEnemies(scene);
		}
		//all town scenes end with Scene
		else if (scene->id.length() > 5 && scene->id.substr(scene->id.length() - 5, scene->id.length() - 1) == "Scene") {
			initTownsPeople(scene);
			// if (currentMusic != townMusic) {
			// 	cout << "Start town music!" << endl;
			// 	townMusic->play();
			// 	currentMusic = townMusic;
			// }
		}
		
		if (scene->id == "hideout4" || scene->id == "hideout8") {
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
	TransitionStruct(SDL_Point{550, 300}, SDL_Point{ 535, 900 }, 2),
	TransitionStruct(SDL_Point{900, 300}, SDL_Point{ 535, 900 }, 3),
	TransitionStruct(SDL_Point{1268, 300}, SDL_Point{ 535, 900 }, 4),
	TransitionStruct(SDL_Point{1632, 300}, SDL_Point{ 535, 900 }, 5),
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

}

void MyGame::initTownsPeople(Scene* s) {
	if (s->id == "townScene" && !s->enemiesAdded) {
		string walkingTownee1Text = "What's up homey!";
		walkingTownee1 = new TownsPeople((Player*)character, "walkingTownee1", true, walkingTownee1Text);	
		walkingTownee1->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		walkingTownee1->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		walkingTownee1->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		walkingTownee1->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		townScene->addChild(walkingTownee1);
		walkingTownee1->position = { 700, 500 };
		// walkingTownee1->scaleX = 0.75;
		// walkingTownee1->scaleY = 0.75;
		walkingTownee1->play("storekeeperLeft");
		
		s->enemiesAdded=true;
		s->enemiesLeft=0;
	} else if (s->id == "sheriffScene" && !s->enemiesAdded) {
		string sheriffText1 = "HEY FRIEND, WE'VE BEEN UNDER ATTACK BY A GANG AND IF YOU CLEAR OUT LAKE STILL SOUTHWEST OF TOWN I CAN GIVE YOU SOME FOOD AND WHISKEY IN RETURN. ";
		string sheriffText2 = "THANKS FOR DOING THAT, HERE'S YOUR REWARD.. CAN YOU ALSO CLEAR OUT THE BADLANDS SOUTHEAST OF TOWN?";
		string sheriffText3 = "YOU'RE QUICKLY BECOMING A HELLUVA EFFICIENT DEPUTY, THE GANG IS SO CRIPPLED WE COULD TAKE THEM OUT RIGHT NOW AT THEIR HIDEOUT EAST OF TOWN";
		vector<string> dialogue = {sheriffText1, sheriffText2, sheriffText3};
		sheriff1 = new Sheriff((Player*)character, "sheriff1", dialogue);	
		sheriff1->addAnimation("resources/friendlies/", "SheriffLeft", 1, 1, true);
		sheriff1->addAnimation("resources/friendlies/", "SheriffRight", 1, 1, true);
		sheriff1->addAnimation("resources/friendlies/", "SheriffUp", 1, 1, true);
		sheriff1->addAnimation("resources/friendlies/", "SheriffDown", 1, 1, true);
		sheriffScene->addChild(sheriff1);
		sheriff1->position = { 881, 589 };
		sheriff1->play("SheriffDown");
		
		s->enemiesAdded=true;
		s->enemiesLeft=0;
	} else if (s->id == "storeScene" && !s->enemiesAdded) {
		string storekeeper1Text = "New to Town? Welcome to Stillwater! I hope you can give us a hand with that gang.";
		storekeeper1 = new TownsPeople((Player*)character, "storekeeper", false, storekeeper1Text);	
		storekeeper1->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		storekeeper1->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		storekeeper1->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		storekeeper1->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		storeScene->addChild(storekeeper1);
		storekeeper1->position = { 550, 550 };
		storekeeper1->play("storekeeperLeft");
		
		s->enemiesAdded=true;
		s->enemiesLeft=0;

	} else if (s->id == "hotelScene" && !s->enemiesAdded) {
		string hotelManText = "Sorry my friend, we don't have any vacancies.";
		hotelMan = new TownsPeople((Player*)character, "storekeeper", false, hotelManText);	
		hotelMan->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		hotelMan->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		hotelMan->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		hotelMan->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		hotelScene->addChild(hotelMan);
		hotelMan->position = { 550, 550 };
		hotelMan->play("storekeeperLeft");
		s->enemiesLeft=0;
		s->enemiesAdded=true;
	} else if (s->id == "bankScene" && !s->enemiesAdded) {
		string bankManText = "Hey! You look awful familiar but I can't remember how...";
		bankMan = new TownsPeople((Player*)character, "storekeeper", false, bankManText);	
		bankMan->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		bankMan->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		bankMan->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		bankMan->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		bankScene->addChild(bankMan);
		bankMan->position = { 550, 550 };
		bankMan->play("storekeeperLeft");
		s->enemiesLeft=0;
		s->enemiesAdded=true;
	} else if (s->id == "postScene" && !s->enemiesAdded) {
		string postManText = "I'm glad you got a chance to see our nice little town. All things serve the beam!";
		postMan = new TownsPeople((Player*)character, "storekeeper", false, postManText);	
		postMan->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		postMan->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		postMan->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		postMan->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		postScene->addChild(postMan);
		postMan->position = { 800, 275 };
		postMan->play("storekeeperLeft");
		s->enemiesLeft=0;
		s->enemiesAdded=true;
	} else if (s->id == "cantinaScene" && !s->enemiesAdded) {
		string cantinaManText = "Looks like you scared all my customers out of the bar!!";
		cantinaMan = new TownsPeople((Player*)character, "storekeeper", false, cantinaManText);	
		cantinaMan->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		cantinaMan->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		cantinaMan->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		cantinaMan->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		cantinaScene->addChild(cantinaMan);
		cantinaMan->position = { 890, 300 };
		cantinaMan->play("storekeeperLeft");
		s->enemiesLeft=0;
		s->enemiesAdded=true;
	} else if (s->id == "drugScene" && !s->enemiesAdded) {
		string drugManText = "I hear you have amnesia, I'd offer you some medicine to help but there's not much I can do to solve that for ya.";
		drugMan = new TownsPeople((Player*)character, "storekeeper", false, drugManText);	
		drugMan->addAnimation("resources/friendlies/", "storekeeperLeft", 1, 1, true);
		drugMan->addAnimation("resources/friendlies/", "storekeeperRight", 1, 1, true);
		drugMan->addAnimation("resources/friendlies/", "storekeeperUp", 1, 1, true);
		drugMan->addAnimation("resources/friendlies/", "storekeeperDown", 1, 1, true);
		drugScene->addChild(drugMan);
		drugMan->position = { 711, 308 };
		drugMan->play("storekeeperLeft");
		
		s->enemiesLeft=0;
		s->enemiesAdded=true;
	}
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

	//TransitionStruct(SDL_Point{ 1085, 0 }, SDL_Point{ 80, 305 }, 9, TransitionDetection::AXIS, Cardinal::EAST),
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
		wolf1LakeStill1 = new Wolf((Player*)character, "LakeWolf1");	// Adding wolf sprites
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf1LakeStill1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		lake1->addChild(wolf1LakeStill1);
		wolf1LakeStill1->position = { 500, 500 };
		wolf1LakeStill1->scaleX = 0.75;
		wolf1LakeStill1->scaleY = 0.75;
		wolf1LakeStill1->play("WolfLeft");

		wolf2LakeStill1 = new Wolf((Player*)character, "LakeWolf2");	// Adding wolf sprites
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf2LakeStill1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		lake1->addChild(wolf2LakeStill1);
		wolf2LakeStill1->position = { 700, 500 };
		wolf2LakeStill1->scaleX = 0.75;
		wolf2LakeStill1->scaleY = 0.75;
		wolf2LakeStill1->play("WolfLeft");

		s->enemiesLeft=2;
		s->enemiesAdded = true;
	}
	if (s->id == "lake2" && !s->enemiesAdded) {

		thug1LakeStill2 = new GangThug((Player*)character, "LakeGangThug1");	
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug1LakeStill2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		lake2->addChild(thug1LakeStill2);
		thug1LakeStill2->position = { 700, 500 };
		thug1LakeStill2->play("GangThugLeft");
		gang_thugs[thug1LakeStill2] = 1;

		thug2LakeStill2 = new GangThug((Player*)character, "LakeGangThug2");	
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug2LakeStill2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		lake2->addChild(thug2LakeStill2);
		thug2LakeStill2->position = { 700, 300 };
		thug2LakeStill2->play("GangThugLeft");
		gang_thugs[thug2LakeStill2] = 1;
		
		s->enemiesLeft=2;
		s->enemiesAdded = true;

	}
	if (s->id == "lake3" && !s->enemiesAdded) {
		mark1LakeStill3 = new GangMarksman((Player*)character, "LakeGangMarksman1");	
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

		mark2LakeStill3 = new GangMarksman((Player*)character, "LakeGangMarksman2");	
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

		s->enemiesLeft=2;
		s->enemiesAdded = true;
	}
	if (s->id == "lake4" && !s->enemiesAdded) {
		arrow1LakeStill4 = new ArrowGuy((Player*)character, "LakeArrow1");
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

		arrow2LakeStill4 = new ArrowGuy((Player*)character, "LakeArrow2");
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
	    
		s->enemiesLeft=2;
		s->enemiesAdded = true;
	}
	if (s->id == "lake6" && !s->enemiesAdded) {
		shot1LakeStill = new GangShot((Player*)character, "LakeGangShot1");	
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot1LakeStill->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		lake6->addChild(shot1LakeStill);
		shot1LakeStill->position = { 700, 300 };
		shot1LakeStill->play("GangShotLeft");
		gang_shot[shot1LakeStill] = 1;

		s->enemiesLeft=1;
		s->enemiesAdded = true;
	}
	if (s->id == "lake7" && !s->enemiesAdded) {
		creeper1LakeStill7 = new Creeper(character, "LakeCreeper1");
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper1LakeStill7->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		lake7->addChild(creeper1LakeStill7);
		creeper1LakeStill7->position = { 300, 700 };
		creeper1LakeStill7->play("CreeperUp");

		creeper2LakeStill7 = new Creeper(character, "LakeCreeper2");
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper2LakeStill7->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		lake7->addChild(creeper2LakeStill7);
		creeper2LakeStill7->position = { 700, 500 };
		creeper2LakeStill7->play("CreeperUp");
		
		s->enemiesLeft=2;
		s->enemiesAdded = true;
	}

	if (s->id == "lake8" && !s->enemiesAdded) {
		knifeguy1LakeStill8 = new KnifeGuy(character, "LakeKnifeGuy1");
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy1LakeStill8->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		lake8->addChild(knifeguy1LakeStill8);
		knifeguy1LakeStill8->position = { 300, 300 };
		knifeguy1LakeStill8->play("KnifeGuyLeft");
		
		s->enemiesLeft=1;
		s->enemiesAdded = true;
	}

	if (s->id == "lake9" && !s->enemiesAdded) {
	
		s->enemiesLeft=0;
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
		s->enemiesLeft = 0;
	}
	if (s->id == "canyon2" && !s->enemiesAdded) {
		s->enemiesAdded = true;
		s->enemiesLeft = 0;
	}
	if (s->id == "canyon3" && !s->enemiesAdded) {
		wolf1Canyon3 = new Wolf((Player*)character, "CanyonWolf1");	// Adding wolf sprites
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf1Canyon3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		canyon3->addChild(wolf1Canyon3);
		wolf1Canyon3->position = { 200, 600 };
		wolf1Canyon3->scaleX = 0.75;
		wolf1Canyon3->scaleY = 0.75;
		wolf1Canyon3->play("WolfRight");

		wolf2Canyon3 = new Wolf((Player*)character, "CanyonWolf2");	// Adding wolf sprites
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf2Canyon3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		canyon3->addChild(wolf2Canyon3);
		wolf2Canyon3->position = { 200, 700 };
		wolf2Canyon3->scaleX = 0.75;
		wolf2Canyon3->scaleY = 0.75;
		wolf2Canyon3->play("WolfRight");

		wolf3Canyon3 = new Wolf((Player*)character, "CanyonWolf3");	// Adding wolf sprites
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
		s->enemiesLeft = 3;
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

	TransitionStruct(SDL_Point{510*2, 720*2}, SDL_Point{406*2, 110*2}, 21)
	};
	transitions.push_back(badlands1Points);

	vector<TransitionStruct> badlands2Points = {
	TransitionStruct(SDL_Point{932*2, 663*2}, SDL_Point{95*2, 680*2}, 22),
	TransitionStruct(SDL_Point{406*2, 40*2}, SDL_Point{450*2, 720*2}, 20)
	};
	transitions.push_back(badlands2Points);

	vector<TransitionStruct> badlands3Points = {
	TransitionStruct(SDL_Point{918*2, 85*2}, SDL_Point{38*2, 305*2}, 23),
	TransitionStruct(SDL_Point{15*2, 680*2}, SDL_Point{852*2, 663*2}, 21)
	};
	transitions.push_back(badlands3Points);

	vector<TransitionStruct> badlands4Points = {
	TransitionStruct(SDL_Point{358*2, 50*2}, SDL_Point{556*2, 117*2}, 24),
	TransitionStruct(SDL_Point{38*2, 373}, SDL_Point{918*2, 165*2}, 22)
	};
	transitions.push_back(badlands4Points);

	vector<TransitionStruct> badlands5Points = {
	TransitionStruct(SDL_Point{38*2, 47*2}, SDL_Point{198*2, 318*2}, 25),
	TransitionStruct(SDL_Point{556*2, 173*2}, SDL_Point{358*2, 100*2}, 23)
	};
	transitions.push_back(badlands5Points);

	vector<TransitionStruct> badlands6Points = {
	TransitionStruct(SDL_Point{198*2, 373*2}, SDL_Point{38*2, 100*2}, 24)
	};
	transitions.push_back(badlands6Points);

	// initialize badlands scene info
	sceneInfo.push_back(SceneInfo(badlands1, SDL_Rect{ 0, 0, 960*2, 768*2 })); // 20
	sceneInfo.push_back(SceneInfo(badlands2, SDL_Rect{ 0, 0, 960*2, 768*2 })); // 21
	sceneInfo.push_back(SceneInfo(badlands3, SDL_Rect{ 0, 0, 960*2, 768*2 })); // 22
	sceneInfo.push_back(SceneInfo(badlands4, SDL_Rect{ 0, 0, 400*2, 400*2 })); // 23
	sceneInfo.push_back(SceneInfo(badlands5, SDL_Rect{ 0, 0, 600*2, 200*2 })); // 24
	sceneInfo.push_back(SceneInfo(badlands6, SDL_Rect{ 0, 0, 400*2, 400*2 })); // 25
}

void MyGame::initBadlandsEnemies(Scene* s) {
	if (s->id == "badlands1" && !s->enemiesAdded) {
		wolf1Badlands1 = new Wolf((Player*)character, "BadWolf1"); 
		wolf1Badlands1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf1Badlands1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf1Badlands1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf1Badlands1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands1->addChild(wolf1Badlands1);
		wolf1Badlands1->position = { 200, 600 };
		wolf1Badlands1->scaleX = 0.75;
		wolf1Badlands1->scaleY = 0.75;
		wolf1Badlands1->play("WolfRight");

		wolf2Badlands1 = new Wolf((Player*)character, "BadWolf2"); 
		wolf2Badlands1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf2Badlands1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf2Badlands1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf2Badlands1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands1->addChild(wolf2Badlands1);
		wolf2Badlands1->position = { 800, 600 };
		wolf2Badlands1->scaleX = 0.75;
		wolf2Badlands1->scaleY = 0.75;
		wolf2Badlands1->play("WolfRight");

		wolf3Badlands1 = new Wolf((Player*)character, "BadWolf3"); 
		wolf3Badlands1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf3Badlands1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf3Badlands1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf3Badlands1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands1->addChild(wolf3Badlands1);
		wolf3Badlands1->position = { 800, 1200 };
		wolf3Badlands1->scaleX = 0.75;
		wolf3Badlands1->scaleY = 0.75;
		wolf3Badlands1->play("WolfRight");

		wolf4Badlands1 = new Wolf((Player*)character, "BadWolf4"); 
		wolf4Badlands1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf4Badlands1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf4Badlands1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf4Badlands1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands1->addChild(wolf4Badlands1);
		wolf4Badlands1->position = { 800, 1200 };
		wolf4Badlands1->scaleX = 0.75;
		wolf4Badlands1->scaleY = 0.75;
		wolf4Badlands1->play("WolfRight");

		wolf5Badlands1 = new Wolf((Player*)character, "BadWolf5"); 
		wolf5Badlands1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf5Badlands1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf5Badlands1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf5Badlands1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands1->addChild(wolf5Badlands1);
		wolf5Badlands1->position = { 1000, 1200 };
		wolf5Badlands1->scaleX = 0.75;
		wolf5Badlands1->scaleY = 0.75;
		wolf5Badlands1->play("WolfRight");

		wolf6Badlands1 = new Wolf((Player*)character, "BadWolf6"); 
		wolf6Badlands1->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf6Badlands1->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf6Badlands1->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf6Badlands1->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands1->addChild(wolf6Badlands1);
		wolf6Badlands1->position = { 800, 400 };
		wolf6Badlands1->scaleX = 0.75;
		wolf6Badlands1->scaleY = 0.75;
		wolf6Badlands1->play("WolfRight");

		s->enemiesLeft=6;
		s->enemiesAdded=true;
	}else if (s->id == "badlands2" && !s->enemiesAdded) {
		thug1Badlands2 = new GangThug((Player*)character, "BadGangThug1");	
		thug1Badlands2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug1Badlands2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug1Badlands2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug1Badlands2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands2->addChild(thug1Badlands2);
		thug1Badlands2->position = { 1000, 800 };
		thug1Badlands2->play("GangThugLeft");
		gang_thugs[thug1Badlands2] = 1;

		thug2Badlands2 = new GangThug((Player*)character, "BadGangThug2");	
		thug2Badlands2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug2Badlands2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug2Badlands2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug2Badlands2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands2->addChild(thug2Badlands2);
		thug2Badlands2->position = { 1000, 900 };
		thug2Badlands2->play("GangThugLeft");
		gang_thugs[thug2Badlands2] = 1;

		thug3Badlands2 = new GangThug((Player*)character, "BadGangThug3");	
		thug3Badlands2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug3Badlands2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug3Badlands2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug3Badlands2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands2->addChild(thug3Badlands2);
		thug3Badlands2->position = { 1300, 700 };
		thug3Badlands2->play("GangThugLeft");
		gang_thugs[thug3Badlands2] = 1;

		thug4Badlands2 = new GangThug((Player*)character, "BadGangThug4");	
		thug4Badlands2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug4Badlands2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug4Badlands2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug4Badlands2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands2->addChild(thug4Badlands2);
		thug4Badlands2->position = { 1300, 900 };
		thug4Badlands2->play("GangThugLeft");
		gang_thugs[thug4Badlands2] = 1;

		mark1Badlands2 = new GangMarksman((Player*)character, "BadGangMarksman1");	
		mark1Badlands2->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		mark1Badlands2->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		mark1Badlands2->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		mark1Badlands2->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		badlands2->addChild(mark1Badlands2);
		mark1Badlands2->position = { 1600, 1100 };
		mark1Badlands2->play("GangMarksmanLeft");
		gang_marksmans[mark1Badlands2] = 1;

		mark2Badlands2 = new GangMarksman((Player*)character, "BadGangMarksman2");	
		mark2Badlands2->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		mark2Badlands2->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		mark2Badlands2->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		mark2Badlands2->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		badlands2->addChild(mark2Badlands2);
		mark2Badlands2->position = { 1400, 1300 };
		mark2Badlands2->play("GangMarksmanLeft");
		gang_marksmans[mark2Badlands2] = 1;
		
		s->enemiesLeft=6;
		s->enemiesAdded=true;
	}else if (s->id == "badlands3" && !s->enemiesAdded) {
		wolf1Badlands3 = new Wolf((Player*)character, "BadWolf5"); 
		wolf1Badlands3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf1Badlands3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf1Badlands3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf1Badlands3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands3->addChild(wolf1Badlands3);
		wolf1Badlands3->position = { 800, 400 };
		wolf1Badlands3->scaleX = 0.75;
		wolf1Badlands3->scaleY = 0.75;
		wolf1Badlands3->play("WolfRight");

		wolf2Badlands3 = new Wolf((Player*)character, "BadWolf6"); 
		wolf2Badlands3->addAnimation("resources/enemies/", "WolfUp", 1, 1, true);
		wolf2Badlands3->addAnimation("resources/enemies/", "WolfLeft", 1, 1, true);
		wolf2Badlands3->addAnimation("resources/enemies/", "WolfRight", 1, 1, true);
		wolf2Badlands3->addAnimation("resources/enemies/", "WolfDown", 1, 1, true);
		badlands3->addChild(wolf2Badlands3);
		wolf2Badlands3->position = { 800, 600 };
		wolf2Badlands3->scaleX = 0.75;
		wolf2Badlands3->scaleY = 0.75;
		wolf2Badlands3->play("WolfRight");

		mark1Badlands3 = new GangMarksman((Player*)character, "BadGangMarksman3");	
		mark1Badlands3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		mark1Badlands3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		mark1Badlands3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		mark1Badlands3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		badlands3->addChild(mark1Badlands3);
		mark1Badlands3->position = { 1400, 400 };
		mark1Badlands3->play("GangMarksmanLeft");
		gang_marksmans[mark1Badlands3] = 1;

		mark2Badlands3 = new GangMarksman((Player*)character, "BadGangMarksman4");	
		mark2Badlands3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		mark2Badlands3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		mark2Badlands3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		mark2Badlands3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		badlands3->addChild(mark2Badlands3);
		mark2Badlands3->position = { 1400, 600 };
		mark2Badlands3->play("GangMarksmanLeft");
		gang_marksmans[mark2Badlands3] = 1;

		creeper1Badlands3 = new Creeper(character, "BadCreeper1");
		creeper1Badlands3->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper1Badlands3->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper1Badlands3->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper1Badlands3->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper1Badlands3->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		badlands3->addChild(creeper1Badlands3);
		creeper1Badlands3->position = { 200, 1000 };
		creeper1Badlands3->play("CreeperUp");

		s->enemiesLeft=5;
		s->enemiesAdded = true;
	} else if (s->id=="badlands4" && !s->enemiesAdded){
		knifeguy1Badlands4 = new KnifeGuy(character, "BadKnifeGuy1");
		knifeguy1Badlands4->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy1Badlands4->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy1Badlands4->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy1Badlands4->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		badlands4->addChild(knifeguy1Badlands4);
		knifeguy1Badlands4->position = { 300, 300 };
		knifeguy1Badlands4->play("KnifeGuyLeft");
		
		shot1Badlands4 = new GangShot((Player*)character, "BadGangShot1");	
		shot1Badlands4->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot1Badlands4->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot1Badlands4->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot1Badlands4->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		badlands4->addChild(shot1Badlands4);
		shot1Badlands4->position = { 600, 300 };
		shot1Badlands4->play("GangShotLeft");
		gang_shot[shot1Badlands4] = 1;

		s->enemiesLeft=2;
		s->enemiesAdded = true;
	} else if (s->id == "badlands5" && !s->enemiesAdded) {
		thug1Badlands5 = new GangThug((Player*)character, "BadGangThug5");	
		thug1Badlands5->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug1Badlands5->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug1Badlands5->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug1Badlands5->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands5->addChild(thug1Badlands5);
		thug1Badlands5->position = { 100, 300 };
		thug1Badlands5->play("GangThugLeft");
		gang_thugs[thug1Badlands5] = 1;

		thug2Badlands5 = new GangThug((Player*)character, "BadGangThug6");	
		thug2Badlands5->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug2Badlands5->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug2Badlands5->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug2Badlands5->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands5->addChild(thug2Badlands5);
		thug2Badlands5->position = { 100, 400 };
		thug2Badlands5->play("GangThugLeft");
		gang_thugs[thug2Badlands5] = 1;

		thug3Badlands5 = new GangThug((Player*)character, "BadGangThug7");	
		thug3Badlands5->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug3Badlands5->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug3Badlands5->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug3Badlands5->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands5->addChild(thug3Badlands5);
		thug3Badlands5->position = { 200, 400 };
		thug3Badlands5->play("GangThugLeft");
		gang_thugs[thug3Badlands5] = 1;

		s->enemiesLeft=3;
		s->enemiesAdded = true;
	} else if (s->id == "badlands6" && !s->enemiesAdded) {
		creeper1Badlands6 = new Creeper(character, "BadCreeper2");
		creeper1Badlands6->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper1Badlands6->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper1Badlands6->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper1Badlands6->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper1Badlands6->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		badlands6->addChild(creeper1Badlands6);
		creeper1Badlands6->position = { 70, 650 };
		creeper1Badlands6->play("CreeperRight");

		creeper2Badlands6 = new Creeper(character, "BadCreeper3");
		creeper2Badlands6->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper2Badlands6->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper2Badlands6->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper2Badlands6->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		creeper2Badlands6->addAnimation("resources/enemies/", "Explode", 16, 1, true);
		badlands6->addChild(creeper2Badlands6);
		creeper2Badlands6->position = { 715, 650 };
		creeper2Badlands6->play("CreeperLeft");

		thug1Badlands6 = new GangThug((Player*)character, "BadGangThug5");	
		thug1Badlands6->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug1Badlands6->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug1Badlands6->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug1Badlands6->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands6->addChild(thug1Badlands6);
		thug1Badlands6->position = { 70, 300 };
		thug1Badlands6->play("GangThugLeft");
		gang_thugs[thug1Badlands6] = 1;

		thug2Badlands6 = new GangThug((Player*)character, "BadGangThug6");	
		thug2Badlands6->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug2Badlands6->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug2Badlands6->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug2Badlands6->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands6->addChild(thug2Badlands6);
		thug2Badlands6->position = { 350, 300 };
		thug2Badlands6->play("GangThugLeft");
		gang_thugs[thug2Badlands6] = 1;

		thug3Badlands6 = new GangThug((Player*)character, "BadGangThug7");	
		thug3Badlands6->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		thug3Badlands6->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		thug3Badlands6->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		thug3Badlands6->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		badlands6->addChild(thug3Badlands6);
		thug3Badlands6->position = { 715, 300 };
		thug3Badlands6->play("GangThugLeft");
		gang_thugs[thug3Badlands6] = 1;

		s->enemiesLeft=5;
		s->enemiesAdded = true;
	}
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
	TransitionStruct(SDL_Point{993, 70}, SDL_Point{598, 900}, 33),
	TransitionStruct(SDL_Point{290, 70}, SDL_Point{507, 1005}, 29)
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

void MyGame::initHideoutEnemies(Scene *s) {
	if (s->id == "hideout8" && !s->enemiesAdded) {
		boss_1 = new ShotgunGuy((Player*)character, "Boss1");	
		boss_1->addAnimation("resources/enemies/", "ShotgunGuyUp", 1, 1, true);
		boss_1->addAnimation("resources/enemies/", "ShotgunGuyLeft", 1, 1, true);
		boss_1->addAnimation("resources/enemies/", "ShotgunGuyRight", 1, 1, true);
		boss_1->addAnimation("resources/enemies/", "ShotgunGuyDown", 1, 1, true);
		boss_1->addAnimation("resources/enemies/", "smoke", 50, 1, true);
		hideout8->addChild(boss_1);
		boss_1->position = { 700, 300 };
		boss_1->play("ShotgunGuyLeft");
		shotgun_boss[boss_1] = 1;
		
		s->enemiesLeft=1;
		s->enemiesAdded = true;
	}
	if (s->id == "hideout7" && !s->enemiesAdded) {
		//floryan
		s->enemiesLeft=0;
		s->enemiesAdded = true;

	}
	if (s->id == "hideout6" && !s->enemiesAdded) {
		knifeguy1hideout6 = new KnifeGuy(character, "knifeguy1hideout6");
		knifeguy1hideout6->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy1hideout6->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy1hideout6->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy1hideout6->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout6->addChild(knifeguy1hideout6);
		knifeguy1hideout6->position = {395, 810};
		knifeguy1hideout6->play("KnifeGuyLeft");

		knifeguy2hideout6 = new KnifeGuy(character, "knifeguy2hideout6");
		knifeguy2hideout6->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy2hideout6->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy2hideout6->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy2hideout6->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout6->addChild(knifeguy2hideout6);
		knifeguy2hideout6->position = {395, 880};
		knifeguy2hideout6->play("KnifeGuyLeft");

		knifeguy3hideout6 = new KnifeGuy(character, "knifeguy3hideout6");
		knifeguy3hideout6->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy3hideout6->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy3hideout6->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy3hideout6->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout6->addChild(knifeguy3hideout6);
		knifeguy3hideout6->position = {65, 960};
		knifeguy3hideout6->play("KnifeGuyLeft");

		knifeguy4hideout6 = new KnifeGuy(character, "knifeguy4hideout6");
		knifeguy4hideout6->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy4hideout6->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy4hideout6->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy4hideout6->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout6->addChild(knifeguy4hideout6);
		knifeguy4hideout6->position = {51, 780};
		knifeguy4hideout6->play("KnifeGuyLeft");

		creeper1hideout6 = new Creeper(character, "creeper1hideout6");
		creeper1hideout6->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper1hideout6->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper1hideout6->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper1hideout6->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		hideout6->addChild(creeper1hideout6);
		creeper1hideout6->position = {1080, 510};
		creeper1hideout6->play("CreeperDown");

		creeper2hideout6 = new Creeper(character, "creeper2hideout6");
		creeper2hideout6->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper2hideout6->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper2hideout6->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper2hideout6->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		hideout6->addChild(creeper2hideout6);
		creeper2hideout6->position = {1260, 520};
		creeper2hideout6->play("CreeperDown");

		gangthug1hideout6 = new GangThug(character, "gangthug1hideout6");
		gangthug1hideout6->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug1hideout6->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug1hideout6->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug1hideout6->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout6->addChild(gangthug1hideout6);
		gangthug1hideout6->position = {1260, 920};
		gangthug1hideout6->play("GangThugUp");
		gang_thugs[gangthug1hideout6] = 1;

		gangthug2hideout6 = new GangThug(character, "gangthug2hideout6");
		gangthug2hideout6->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug2hideout6->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug2hideout6->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug2hideout6->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout6->addChild(gangthug2hideout6);
		gangthug2hideout6->position = {1440, 900};
		gangthug2hideout6->play("GangThugUp");
		gang_thugs[gangthug2hideout6] = 1;


		//bedroom
		//4 knife guys in bathroom
		//2 creeper on right
		// gang thugs in middle
		s->enemiesLeft=8;
		s->enemiesAdded = true;

	}
	if (s->id == "hideout5" && !s->enemiesAdded) {
		//prison
		gangmarksmanfree1hideout5 = new GangMarksmanFree(character, "gangmarksmanfree1hideout5");
		gangmarksmanfree1hideout5->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		gangmarksmanfree1hideout5->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		gangmarksmanfree1hideout5->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		gangmarksmanfree1hideout5->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		hideout5->addChild(gangmarksmanfree1hideout5);
		gangmarksmanfree1hideout5->position = {430,630};
		gangmarksmanfree1hideout5->play("GangMarksmanDown");
		gang_marksmansfree[gangmarksmanfree1hideout5] = 1;

		gangmarksmanfree2hideout5 = new GangMarksmanFree(character, "gangmarksmanfree2hideout5");
		gangmarksmanfree2hideout5->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		gangmarksmanfree2hideout5->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		gangmarksmanfree2hideout5->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		gangmarksmanfree2hideout5->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		hideout5->addChild(gangmarksmanfree2hideout5);
		gangmarksmanfree2hideout5->position = {540,630};
		gangmarksmanfree2hideout5->play("GangMarksmanDown");
		gang_marksmansfree[gangmarksmanfree2hideout5] = 1;

		knifeguy1hideout5 = new KnifeGuy(character, "knifeguy1hideout5");
		knifeguy1hideout5->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy1hideout5->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy1hideout5->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy1hideout5->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout5->addChild(knifeguy1hideout5);
		knifeguy1hideout5->position = {260, 810};
		knifeguy1hideout5->play("KnifeGuyDown");

		knifeguy2hideout5 = new KnifeGuy(character, "knifeguy2hideout5");
		knifeguy2hideout5->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy2hideout5->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy2hideout5->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy2hideout5->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout5->addChild(knifeguy2hideout5);
		knifeguy2hideout5->position = {660, 730};
		knifeguy2hideout5->play("KnifeGuyDown");

		shot1hideout5 = new GangShot((Player*)character, "shot1hideout5");	
		shot1hideout5->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot1hideout5->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot1hideout5->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot1hideout5->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		hideout5->addChild(shot1hideout5);
		shot1hideout5->position = { 150, 580 };
		shot1hideout5->play("GangShotRight");
		gang_shot[shot1hideout5] = 1;

		shot2hideout5 = new GangShot((Player*)character, "shot2hideout5");	
		shot2hideout5->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot2hideout5->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot2hideout5->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot2hideout5->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		hideout5->addChild(shot2hideout5);
		shot2hideout5->position = { 270, 580 };
		shot2hideout5->play("GangShotLeft");
		gang_shot[shot2hideout5] = 1;

		
		// 2 new marksmen who don't move
		// 1 knife on each side of tower
		// shotgun guys from behind in the back
		s->enemiesLeft=6;
		s->enemiesAdded = true;

	}
	if (s->id == "hideout4" && !s->enemiesAdded) {
		//hallway
		// 4 new marksmen - don't move, infinite range
		s->enemiesLeft=0;
		s->enemiesAdded = true;

	}
	if (s->id == "hideout3" && !s->enemiesAdded) {
		// //kitchen
		creeper1hideout3 = new Creeper(character, "creeper1hideout3");
		creeper1hideout3->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper1hideout3->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper1hideout3->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper1hideout3->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		hideout3->addChild(creeper1hideout3);
		creeper1hideout3->position = {320, 540};
		creeper1hideout3->play("CreeperRight");

		creeper2hideout3 = new Creeper(character, "creeper2hideout3");
		creeper2hideout3->addAnimation("resources/enemies/", "CreeperUp", 1, 1, true);
		creeper2hideout3->addAnimation("resources/enemies/", "CreeperLeft", 1, 1, true);
		creeper2hideout3->addAnimation("resources/enemies/", "CreeperRight", 1, 1, true);
		creeper2hideout3->addAnimation("resources/enemies/", "CreeperDown", 1, 1, true);
		hideout3->addChild(creeper2hideout3);
		creeper2hideout3->position = {650, 540};
		creeper2hideout3->play("CreeperLeft");

		shot1hideout3 = new GangShot((Player*)character, "shot1hideout3");	
		shot1hideout3->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot1hideout3->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot1hideout3->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot1hideout3->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		hideout3->addChild(shot1hideout3);
		shot1hideout3->position = { 800, 440 };
		shot1hideout3->play("GangShotDown");
		gang_shot[shot1hideout3] = 1;

		shot2hideout3 = new GangShot((Player*)character, "shot2hideout3");	
		shot2hideout3->addAnimation("resources/enemies/", "GangShotUp", 1, 1, true);
		shot2hideout3->addAnimation("resources/enemies/", "GangShotLeft", 1, 1, true);
		shot2hideout3->addAnimation("resources/enemies/", "GangShotRight", 1, 1, true);
		shot2hideout3->addAnimation("resources/enemies/", "GangShotDown", 1, 1, true);
		hideout3->addChild(shot2hideout3);
		shot2hideout3->position = { 560, 620 };
		shot2hideout3->play("GangShotUp");
		gang_shot[shot2hideout3] = 1;

		gangthug1hideout3 = new GangThug(character, "gangthug1hideout3");
		gangthug1hideout3->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug1hideout3->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug1hideout3->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug1hideout3->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout3->addChild(gangthug1hideout3);
		gangthug1hideout3->position = {830, 970};
		gangthug1hideout3->play("GangThugDown");
		gang_thugs[gangthug1hideout3] = 1;

		gangthug2hideout3 = new GangThug(character, "gangthug2hideout3");
		gangthug2hideout3->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug2hideout3->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug2hideout3->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug2hideout3->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout3->addChild(gangthug2hideout3);
		gangthug2hideout3->position = {960, 780};
		gangthug2hideout3->play("GangThugDown");
		gang_thugs[gangthug2hideout3] = 1;

		gangthug3hideout3 = new GangThug(character, "gangthug3hideout3");
		gangthug3hideout3->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug3hideout3->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug3hideout3->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug3hideout3->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout3->addChild(gangthug3hideout3);
		gangthug3hideout3->position = {1070, 870};
		gangthug3hideout3->play("GangThugLeft");
		gang_thugs[gangthug3hideout3] = 1;
		

		gangthug4hideout3 = new GangThug(character, "gangthug4hideout3");
		gangthug4hideout3->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug4hideout3->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug4hideout3->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug4hideout3->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout3->addChild(gangthug4hideout3);
		gangthug4hideout3->position = {970, 920};
		gangthug4hideout3->play("GangThugUp");
		gang_thugs[gangthug4hideout3] = 1;

		gangmarksmanfree1hideout3 = new GangMarksman(character, "gangmarksmanfree1hideout3");
		gangmarksmanfree1hideout3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		gangmarksmanfree1hideout3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		gangmarksmanfree1hideout3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		gangmarksmanfree1hideout3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		hideout3->addChild(gangmarksmanfree1hideout3);
		gangmarksmanfree1hideout3->position = {1130, 550};
		gangmarksmanfree1hideout3->play("GangMarksmanRight");
		gang_marksmans[gangmarksmanfree1hideout3] = 1;

		gangmarksmanfree2hideout3 = new GangMarksman(character, "gangmarksmanfree2hideout3");
		gangmarksmanfree2hideout3->addAnimation("resources/enemies/", "GangMarksmanUp", 1, 1, true);
		gangmarksmanfree2hideout3->addAnimation("resources/enemies/", "GangMarksmanLeft", 1, 1, true);
		gangmarksmanfree2hideout3->addAnimation("resources/enemies/", "GangMarksmanRight", 1, 1, true);
		gangmarksmanfree2hideout3->addAnimation("resources/enemies/", "GangMarksmanDown", 1, 1, true);
		hideout3->addChild(gangmarksmanfree2hideout3);
		gangmarksmanfree2hideout3->position = {1220,470};
		gangmarksmanfree2hideout3->play("GangMarksmanDown");
		gang_marksmans[gangmarksmanfree2hideout3] = 1;

		// knifeguy1hideout3 = new KnifeGuy(character, "knifeguy1hideout3");
		// knifeguy1hideout3->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		// knifeguy1hideout3->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		// knifeguy1hideout3->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		// knifeguy1hideout3->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		// hideout3->addChild(knifeguy1hideout3);
		// knifeguy1hideout3->position = {1580, 770};
		// knifeguy1hideout3->play("KnifeGuyDown");

		// knifeguy2hideout3 = new KnifeGuy(character, "knifeguy2hideout3");
		// knifeguy2hideout3->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		// knifeguy2hideout3->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		// knifeguy2hideout3->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		// knifeguy2hideout3->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		// hideout3->addChild(knifeguy2hideout3);
		// knifeguy2hideout3->position = {1660, 790};
		// knifeguy2hideout3->play("KnifeGuyDown");

		// knifeguy3hideout3 = new KnifeGuy(character, "knifeguy3hideout3");
		// knifeguy3hideout3->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		// knifeguy3hideout3->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		// knifeguy3hideout3->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		// knifeguy3hideout3->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		// hideout3->addChild(knifeguy3hideout3);
		// knifeguy3hideout3->position = {1590, 910};
		// knifeguy3hideout3->play("KnifeGuyUp");

		// knifeguy4hideout3 = new KnifeGuy(character, "knifeguy4hideout3");
		// knifeguy4hideout3->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		// knifeguy4hideout3->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		// knifeguy4hideout3->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		// knifeguy4hideout3->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		// hideout3->addChild(knifeguy4hideout3);
		// knifeguy4hideout3->position = {1660, 910};
		// knifeguy4hideout3->play("KnifeGuyUp");


		s->enemiesLeft=10; // 14 with knife guys
		s->enemiesAdded = true;

	}
	if (s->id == "hideout2" && !s->enemiesAdded) {
		// bar
		// 3 gang thugs at bar
		// 2 knife wielders at first table
		// 2 knife wielders at back table

		knifeguy1hideout2 = new KnifeGuy(character, "knifeguy1hideout2");
		knifeguy1hideout2->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy1hideout2->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy1hideout2->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy1hideout2->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout2->addChild(knifeguy1hideout2);
		knifeguy1hideout2->position = {666, 650};
		knifeguy1hideout2->play("KnifeGuyDown");

		knifeguy2hideout2 = new KnifeGuy(character, "knifeguy2hideout2");
		knifeguy2hideout2->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy2hideout2->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy2hideout2->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy2hideout2->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout2->addChild(knifeguy2hideout2);
		knifeguy2hideout2->position = {867, 650};
		knifeguy2hideout2->play("KnifeGuyDown");

		knifeguy3hideout2 = new KnifeGuy(character, "knifeguy3hideout2");
		knifeguy3hideout2->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy3hideout2->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy3hideout2->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy3hideout2->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout2->addChild(knifeguy3hideout2);
		knifeguy3hideout2->position = {1367, 870};
		knifeguy3hideout2->play("KnifeGuyUp");

		knifeguy4hideout2 = new KnifeGuy(character, "knifeguy4hideout2");
		knifeguy4hideout2->addAnimation("resources/enemies/", "KnifeGuyUp", 1, 1, true);
		knifeguy4hideout2->addAnimation("resources/enemies/", "KnifeGuyLeft", 1, 1, true);
		knifeguy4hideout2->addAnimation("resources/enemies/", "KnifeGuyRight", 1, 1, true);
		knifeguy4hideout2->addAnimation("resources/enemies/", "KnifeGuyDown", 1, 1, true);
		hideout2->addChild(knifeguy4hideout2);
		knifeguy4hideout2->position = {1557, 870};
		knifeguy4hideout2->play("KnifeGuyUp");

		gangthug1hideout2 = new GangThug(character, "gangthug1hideout2");
		gangthug1hideout2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug1hideout2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug1hideout2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug1hideout2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout2->addChild(gangthug1hideout2);
		gangthug1hideout2->position = {830, 970};
		gangthug1hideout2->play("GangThugDown");
		gang_thugs[gangthug1hideout2] = 1;

		gangthug2hideout2 = new GangThug(character, "gangthug2hideout2");
		gangthug2hideout2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug2hideout2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug2hideout2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug2hideout2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout2->addChild(gangthug2hideout2);
		gangthug2hideout2->position = {960, 780};
		gangthug2hideout2->play("GangThugDown");
		gang_thugs[gangthug2hideout2] = 1;

		gangthug3hideout2 = new GangThug(character, "gangthug3hideout2");
		gangthug3hideout2->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug3hideout2->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug3hideout2->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug3hideout2->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout2->addChild(gangthug3hideout2);
		gangthug3hideout2->position = {1070, 870};
		gangthug3hideout2->play("GangThugLeft");
		gang_thugs[gangthug3hideout2] = 1;

		s->enemiesLeft=7;
		s->enemiesAdded = true;

	}
	if (s->id == "hideout1" && !s->enemiesAdded) {
		// 3 gang thugs
		gangthug1hideout1 = new GangThug(character, "gangthug1hideout1");
		gangthug1hideout1->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug1hideout1->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug1hideout1->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug1hideout1->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout1->addChild(gangthug1hideout1);
		gangthug1hideout1->position = {830, 970};
		gangthug1hideout1->play("GangThugDown");
		gang_thugs[gangthug1hideout1] = 1;


		gangthug2hideout1 = new GangThug(character, "gangthug2hideout1");
		gangthug2hideout1->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug2hideout1->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug2hideout1->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug2hideout1->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout1->addChild(gangthug2hideout1);
		gangthug2hideout1->position = {960, 780};
		gangthug2hideout1->play("GangThugDown");
		gang_thugs[gangthug2hideout1] = 1;

		gangthug3hideout1 = new GangThug(character, "gangthug3hideout1");
		gangthug3hideout1->addAnimation("resources/enemies/", "GangThugUp", 1, 1, true);
		gangthug3hideout1->addAnimation("resources/enemies/", "GangThugLeft", 1, 1, true);
		gangthug3hideout1->addAnimation("resources/enemies/", "GangThugRight", 1, 1, true);
		gangthug3hideout1->addAnimation("resources/enemies/", "GangThugDown", 1, 1, true);
		hideout1->addChild(gangthug3hideout1);
		gangthug3hideout1->position = {1070, 870};
		gangthug3hideout1->play("GangThugLeft");
		gang_thugs[gangthug3hideout1] = 1;

		s->enemiesLeft=3;
		s->enemiesAdded = true;
	}
}


void MyGame::enemyShootingLoops() {
	// GANG THUG LOOP
	for (std::map<GangThug*, int>::iterator it=gang_thugs.begin(); it!=gang_thugs.end(); ++it) {
		if (it->first->health == 0 ) {
			it->first->clean = true;
			gang_thugs.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			Benemy* benemy = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "revolver", "Benemy"+to_string(iterate));
			benemy->distance = 20;
			this->addChild(benemy);
			benemy->position = {it->first->position.x, it->first->position.y };
			benemy->pivot = { benemy->width / 2, benemy->height / 2 };
			benemy->scaleX = 1;
			benemy->scaleY = 1;
			if (it->first->shots_fired == 5) {
				it->first->shoot -= 200;
				it->first->shots_fired = 0;
			} else{
				it->first->shoot -= 80;
				it->first->shots_fired ++;
			}
			iterate ++;
		}
	}
	// GANG SHOT loop
	for (std::map<GangShot*, int>::iterator it=gang_shot.begin(); it!=gang_shot.end(); ++it) {
		if (it->first->health == 0 ) {
			it->first->clean = true;
			gang_shot.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			Benemy* benemya = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "shotgun", "Benemy"+to_string(iterate));
			benemya->distance = 20;
			this->addChild(benemya);
			benemya->position = {it->first->position.x, it->first->position.y };
			benemya->pivot = { benemya->width / 2, benemya->height / 2 };
			benemya->scaleX = 1;
			benemya->scaleY = 1;
			if (it->first->shots_fired == 1) {
				it->first->shoot -= 100;
				it->first->shots_fired = 0;
			} else{
				it->first->shoot -= 75;
				it->first->shots_fired ++;
			}
			iterate ++;
		}
	}
	// GANG MARKSMAN loop
	for (std::map<GangMarksman*, int>::iterator it=gang_marksmans.begin(); it!=gang_marksmans.end(); ++it) {
		if (it->first->health == 0 ) {
			it->first->clean = true;
			gang_marksmans.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			Benemy* benemy2 = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 5, "rifle", "Benemy"+to_string(iterate));
			benemy2->distance = 20;
			this->addChild(benemy2);
			benemy2->position = {it->first->position.x, it->first->position.y };
			benemy2->pivot = { benemy2->width / 2, benemy2->height / 2 };
			benemy2->scaleX = 1;
			benemy2->scaleY = 1;
			it->first->shoot -= 200;
			iterate++;
		}
	}
	// ARROW GUY loop
	for (std::map<ArrowGuy*, int>::iterator it=arrow_guys.begin(); it!=arrow_guys.end(); ++it) {
		if (it->first->health == 0 ) {
			it->first->clean = true;
			arrow_guys.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			Benemy * benemy3 = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 5, "arrow", "Benemy"+to_string(iterate));
			benemy3->distance = 20;
			this->addChild(benemy3);
			benemy3->position = {it->first->position.x, it->first->position.y };
			benemy3->pivot = { benemy3->width / 2, benemy3->height / 2 };
			benemy3->scaleX = 1;
			benemy3->scaleY = 1;
			it->first->shoot -= 80;
			iterate++;
		}
	}
	// SHOTGUN BOSS loop
	for (std::map<ShotgunGuy*, int>::iterator it=shotgun_boss.begin(); it!=shotgun_boss.end(); ++it) {
		if (it->first->health == 0 ) {
			it->first->clean = true;
			shotgun_boss.erase(it->first);
			break;
		}
		if(it->first->shoot > 0) {
			Benemy* benemyb = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "shotgun", "Benemy"+to_string(iterate));
			benemyb->distance = 20;
			this->addChild(benemyb);
			benemyb->position = {it->first->position.x, it->first->position.y };
			benemyb->pivot = { benemyb->width / 2, benemyb->height / 2 };
			benemyb->scaleX = 1;
			benemyb->scaleY = 1;
			if (it->first->shots_fired == 1) {
				it->first->state = 4;
				it->first->shoot = 0;
				//it->first->shots_fired = 0;
			} else{
				it->first->shoot -= 75;
				it->first->shots_fired ++;
			}
			iterate++;
		}
		if(it->first->dynamite){
			// ensure dynamite is only thrown once
			it->first->dynamite = false;
			Benemy* benemyc = new Benemy((AnimatedSprite*)it->first, character->position.x, character->position.y, 6, "dynamite", "Benemy"+to_string(iterate));
			benemyc->distance = 20;
			this->addChild(benemyc);
			benemyc->position = {it->first->position.x, it->first->position.y };
			benemyc->pivot = { benemyc->width / 2, benemyc->height / 2 };
			benemyc->scaleX = 1;
			benemyc->scaleY = 1;
			iterate++;
		}
	}
}

void MyGame::playerShooting(int gun, string dir){
	if (gun == 1 && character->knife_throws > 0) {
	} else if (gun == 1) {
		bullet = new Projectile(dir,this->position, gun);
		bullet->id = "bullet" + projectileIdNum;
		projectileIdNum++;
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
		while (this->reloadTime < 1200 || character->revolver_shots != 0) {
			if (this->reloadTime % 200 == 0) {
				character->revolver_shots --;
				character->dispatchEvent(character->updateAmmo);
				this->reloadTime ++;
			} else {
				this->reloadTime ++;
			}
			// character->revolver_shots = 3;
			// character->dispatchEvent(character->updateAmmo);
		}
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
			if (checkInsidePoint(cur.point, character) && curScene->enemiesLeft == 0) {
				if (curScene->id == "townScene") {
					// checking to see if areas are unlocked
					if (cur.dest.x == 550 && character->lakeUnlocked || cur.dest.x == 406 && character->badlandsUnlocked
						|| cur.dest.x == 80 && character->hideoutUnlocked) {
						curTransition = cur;
						transitionScene();
						break;
					} else if (cur.point.y < 600) { // this means it's a transition to one of the buildings
						curTransition = cur;
						transitionScene();
						break;
					}
				} else {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
		}
		else if (cur.detection == TransitionDetection::AXIS) {
			if (cur.direction == Cardinal::NORTH) {
				if (character->position.y - character->pivot.y <= cur.point.y && curScene->enemiesLeft == 0) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
			else if (cur.direction == Cardinal::EAST) {
				if (character->position.x - character->pivot.x + character->width >= cur.point.x && curScene->enemiesLeft == 0) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
			else if (cur.direction == Cardinal::SOUTH) {
				if (character->position.y - character->pivot.y + character->height >= cur.point.y && curScene->enemiesLeft == 0) {
					curTransition = cur;
					transitionScene();
					break;
				}
			}
			else if (cur.direction == Cardinal::WEST) {
				if (character->position.x - character->pivot.x <= cur.point.x && curScene->enemiesLeft == 0) {
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
		//addDOC(s, obs, 113, 20, 437, 510);
		//addDOC(s, obs, 97, 20, 432, 662);
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

		// river boxes
		addDOC(s, riv, 99, 32, 797, 240);
		addDOC(s, riv, 50, 28, 858, 280);

		addDOC(s, riv, 158, 139, 704, 935);
		addDOC(s, riv, 124, 280, 688, 918);
		addDOC(s, riv, 110, 23, 700, 641);
		addDOC(s, riv, 110, 28, 710, 617);
		addDOC(s, riv, 110, 26, 722, 594);
		addDOC(s, riv, 110, 26, 736, 571);
		addDOC(s, riv, 98, 26, 766, 540);

		// bridge boxes
		addDOC(s, obs, 172, 15, 743, 505);
		addDOC(s, obs, 172, 15, 818, 331);
		addDOC(s, obs, 30, 50, 792, 456);
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
		// // wall
		// addDOC(s, obs, 1920, 350, 0, 0);

		// // shelves
		// addDOC(s, obs, 571, 236, 100, 165);
		// addDOC(s, obs, 680, 82, 1161, 307);

		// // tables
		// addDOC(s, obs, 273, 163, 351, 478);
		// addDOC(s, obs, 273, 163, 765, 810);
		// addDOC(s, obs, 273, 163, 1158, 490);
		// addDOC(s, obs, 273, 163, 1525, 810);

		// // food bar
		// addDOC(s, obs, 501, 94, 51, 909);
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
		addDOC(s, cac, 60, 60, 1052, 204);
		addDOC(s, cac, 60, 60, 400, 520);
		addDOC(s, cac, 60, 60, 1488, 534);
		addDOC(s, cac, 60, 60, 556, 766);
		addDOC(s, cac, 60, 60, 1108, 772);
		addDOC(s, cac, 60, 60, 416, 1102);
		addDOC(s, cac, 60, 60, 1272, 1224);

	}

	if (s == badlands2) {
		// cacti
		addDOC(s, cac, 60, 60, 526*2, 76*2+60);
		addDOC(s, cac, 60, 60, 450*2, 192*2+60);
		addDOC(s, cac, 60, 60, 343*2, 292*2+60);
		addDOC(s, cac, 60, 60, 279*2, 431*2+60);
		addDOC(s, cac, 60, 60, 524*2, 73*2+60);
		addDOC(s, cac, 60, 60, 769*2, 460*2+60);
		addDOC(s, cac, 60, 60, 636*2, 507*2+60);
		addDOC(s, cac, 60, 60, 636*2, 583*2+60);

	}

	if (s == badlands3) {
		// cacti
		addDOC(s, cac, 60, 60, 524*2, 73*2+60);
		addDOC(s, cac, 60, 60, 707*2, 176*2+60);
		addDOC(s, cac, 60, 60, 343*2, 293*2+60);
		addDOC(s, cac, 60, 60, 343*2, 293*2+60);
		addDOC(s, cac, 60, 60, 706*2, 379*2+60);
		addDOC(s, cac, 60, 60, 561*2, 389*2+60);
		addDOC(s, cac, 60, 60, 279*2, 432*2+60);
		addDOC(s, cac, 60, 60, 500*2, 452*2+60);
		addDOC(s, cac, 60, 60, 770*2, 460*2+60);
		addDOC(s, cac, 60, 60, 636*2, 510*2+60);

	}

	if (s == badlands4) {
		// tables
		addDOC(s, obs, 140, 100, 0, 20*2);
		addDOC(s, obs, 160, 160, 160*2, 134*2+40);
		addDOC(s, obs, 140, 100, 320*2, 300*2+40);

		//wall
		addDOC(s, obs, 400*2, 70*2, 0, -40*2+40);
		addDOC(s, obs, 400*2, 70*2, 0, 360*2+40);

	}

	if (s == badlands5) {
		//walls
		addDOC(s, obs, 600*2, 70*2, 0, -40*2+40);
		addDOC(s, obs, 600*2, 70*2, 0, 160*2+40);

	}

	if (s == badlands6) {
		//walls
		addDOC(s, obs, 400*2, 70*2, 0, -40*2+40);
		addDOC(s, obs, 400*2, 70*2, 0, 360*2+40);

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