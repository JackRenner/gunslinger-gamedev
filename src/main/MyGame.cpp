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
	this->gun = 0;

	gameCamera = Camera();

	cameraDemoScene = new Scene();
	cameraDemoScene->loadScene("./resources/scene/cameraDemoScene.txt");

	townSquareScene = new Scene();
	townSquareScene->loadScene("./resources/scene/townsquare.txt");

	// character = new AnimatedSprite("character");
	// character->addAnimation("./resources/character/", "Run", 20, 2, true);
	character = new Player();
	// character->addAnimation("./resources/character/", "Down", 3, 1, true)


	this->setScene(townSquareScene);
	this->addChild(character);
	character->position = { 50, 250 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->scaleX = 0.5;
	character->scaleY = 0.5;
	character->width = 90;

	SDL_Rect pivot = { 0, 0, 600, 800 };
	SDL_Rect long_hall = { 600, 0, 1000, 800 };
	//SDL_Rect small_room = { 0 - 150, 800 - 125, 600 * 1.5, 500 * 1.5 };
	SDL_Rect small_room = { 0, 800, 600, 500 };

	addCameraBound(small_room, false, true, true, true);
	addCameraBound(pivot, 1, 0, 1, 0);
	addCameraBound(long_hall, true, true, false, true);

	room_state = 0;


	gunshot = new Sound();
	//music = new Sound();
	//music->playMusic();

	zoomPoint = { small_room.x + small_room.w / 2, small_room.y + small_room.h / 2 };
}

MyGame::~MyGame() {
	
}


void MyGame::update(set<SDL_Scancode> pressedKeys) {
	controls::update(pressedKeys);

	// gun select
	if(controls::press1()){
		this->gun = 0;
	};
	if(controls::press2()){
		this->gun = 1;
	};
	if(controls::press3()){
		this->gun = 2;
	};
	if(controls::press4()){
		this->gun = 3;
	};
	if(controls::press5()){
		this->gun = 4;
	};
	// shooting
	if (controls::pressUp()) {
		//gunshot->playSFX();
		this->playerShooting(this->gun, "up");
	}
	if (controls::pressDown()) {
		//gunshot->playSFX();
		this->playerShooting(this->gun, "down");
	}
	if (controls::pressRight()) {
		//gunshot->playSFX();	
		this->playerShooting(this->gun, "left");
	}
	if (controls::pressLeft()) {
		//gunshot->playSFX();	
		this->playerShooting(this->gun, "right");
	}
	// there isn't any delay from reloading yet
	if (controls::pressR()) {
		this->reloadGun(this->gun);
	}


	// if (pressedKeys.find(SDL_SCANCODE_O) != pressedKeys.end()) {
	// 	gameCamera.scale -= 0.05;
	// }
	// if (pressedKeys.find(SDL_SCANCODE_P) != pressedKeys.end()) {
	// 	gameCamera.scale += 0.05;
	// }

	SDL_Rect room;
	for (int i = 0; i < boundaries.size(); i++) {
		room = boundaries[i].bounds;
		if (checkInside(room, character)) {
			room_state = i;
		}
	}

	Game::update(pressedKeys);

	gameCamera.x = character->position.x - gameCamera.viewportWidth / 2;
	gameCamera.y = character->position.y - gameCamera.viewportHeight / 2;

	if (room_state == 0)
		gameCamera.scale = 1.3;
	else
		gameCamera.scale = 1.0;

	enforceCameraBounds();
}

void MyGame::draw(AffineTransform& at) {
	at.translate(-gameCamera.x + zoomPoint.x, -gameCamera.y + zoomPoint.y);
	at.scale(gameCamera.scale, gameCamera.scale);
	at.translate(-zoomPoint.x, -zoomPoint.y);

	Game::draw(at);

	at.translate(zoomPoint.x, zoomPoint.y);
	at.scale(1 / gameCamera.scale, 1 / gameCamera.scale);
	at.translate(gameCamera.x - zoomPoint.x, gameCamera.y - zoomPoint.y);
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

// Add a camera bound given an area to enforce and bools for whether you want to enforce that cardinal direction for this bound
void MyGame::addCameraBound(SDL_Rect bounds, bool up, bool down, bool left, bool right) {
	Bound tmp_bound = Bound(bounds, up, down, left, right);
	boundaries.push_back(tmp_bound);
}

// Enforce camera bounds for the current room state. Does not currently account for room rotations.
void MyGame::enforceCameraBounds() {
	Bound room = boundaries[room_state];

	AffineTransform boundCalc = AffineTransform();

	boundCalc.translate(zoomPoint.x, zoomPoint.y);
	boundCalc.scale(gameCamera.scale, gameCamera.scale);
	boundCalc.translate(-zoomPoint.x, -zoomPoint.y);

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
bool MyGame::checkInside(SDL_Rect box, DisplayObject* entity) {
	return (entity->position.x - entity->pivot.x >= box.x &&
		entity->position.x + entity->pivot.x <= box.x + box.w &&
		entity->position.y - entity->pivot.y >= box.y &&
		entity->position.y + entity->pivot.y <= box.y + box.h);
}

void MyGame::playerShooting(int gun, string dir){
	if (gun == 1 && this->knife_throws > 0) {
	} else if (gun == 1) {
		bullet = new Projectile(dir,this->position, gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		this->knife_throws ++;
	} else if (this->gun == 2 && this->revolver_shots > 5) {
	} else if (this->gun == 2) {
		bullet = new Projectile(dir,this->position, this->gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		this->revolver_shots ++;
	} else if (this->gun == 3 && this->shotgun_shots > 1) {
	} else if (this->gun == 3) {
		bullet = new Projectile(dir,character->position, gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		this->shotgun_shots ++;
	} else if (this->gun == 4 && this->rifle_shots > 4) {
	} else if (this->gun == 4) {
		bullet = new Projectile(dir,character->position, gun);
		this->addChild(bullet);
		bullet->position = { character->position.x - character->pivot.x, character->position.y - character->pivot.y };
		this->rifle_shots ++;
	}
}
void MyGame::reloadGun(int gun) {
	if (gun == 2) {
		this->revolver_shots = 0;
	} else if (gun == 3) {
		this->shotgun_shots = 0;
	} else if (gun == 4) {
		this->rifle_shots = 0;
	}
}