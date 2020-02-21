#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include "MyGame.h"

using namespace std;

Camera DisplayObject::gameCamera;

MyGame::MyGame() : Game(gameCamera.viewportWidth, gameCamera.viewportHeight) {

	gameCamera = Camera();
	boundsCalc = new AffineTransform();

	cameraDemoScene = new Scene();
	cameraDemoScene->loadScene("./resources/scene/cameraDemoScene.txt");

	character = new AnimatedSprite("character");
	character->addAnimation("./resources/character/", "Run", 20, 2, true);


	this->setScene(cameraDemoScene);
	this->addChild(character);
	character->position = { 300, 1100 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->play("Run");
	character->width = 90;

	SDL_Rect pivot = { 0, 0, 600, 800 };
	SDL_Rect long_hall = { 600, 0, 1000, 800 };
	SDL_Rect small_room = { 0, 800, 600, 500 };

	addCameraBound(small_room, false, true, true, true);
	addCameraBound(pivot, 1, 0, 1, 0);
	addCameraBound(long_hall, true, true, false, true);

	room_state = 0;

	gunshot = new Sound();

	zoomPoint = { gameCamera.viewportWidth / 2, gameCamera.viewportHeight / 2 };
}

MyGame::~MyGame() {
	delete boundsCalc;
}


void MyGame::update(set<SDL_Scancode> pressedKeys) {
	if (pressedKeys.find(SDL_SCANCODE_M) != pressedKeys.end()) {
		gunshot->playSFX();
	}

	if (pressedKeys.find(SDL_SCANCODE_UP) != pressedKeys.end()) {
		character->position.y -= 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_DOWN) != pressedKeys.end()) {
		character->position.y += 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_RIGHT) != pressedKeys.end()) {
		character->position.x += 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_LEFT) != pressedKeys.end()) {
		character->position.x -= 5;
	}

	if (pressedKeys.find(SDL_SCANCODE_Q) != pressedKeys.end()) {
		gameCamera.scale -= 0.05;
	}
	if (pressedKeys.find(SDL_SCANCODE_W) != pressedKeys.end()) {
		gameCamera.scale += 0.05;
	}

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

	/*if (room_state == 0)
		gameCamera.scale = 1.5;*/

	enforceCameraBounds();
}

void MyGame::draw(AffineTransform& at) {
	at.translate(zoomPoint.x, zoomPoint.y);
	at.scale(gameCamera.scale, gameCamera.scale);
	at.translate(-(gameCamera.x + zoomPoint.x), -(gameCamera.y + zoomPoint.y));

	Game::draw(at);

	at.translate(gameCamera.x + zoomPoint.x, gameCamera.y + zoomPoint.y);
	at.scale(1 / gameCamera.scale, 1 / gameCamera.scale);
	at.translate(-zoomPoint.x, -zoomPoint.y);
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

	// check right bound
	if (room.check_right) {
		if (gameCamera.x + gameCamera.viewportWidth > room.bounds.x + room.bounds.w)
			gameCamera.x = room.bounds.x + room.bounds.w - gameCamera.viewportWidth;
	}
	// check left bound
	if (room.check_left) {
		if (gameCamera.x < room.bounds.x)
			gameCamera.x = room.bounds.x;
	}
	// check upper bound
	if (room.check_up) {
		if (gameCamera.y < room.bounds.y)
			gameCamera.y = room.bounds.y;
	}
	// check lower bound
	if (room.check_down) {
		if (gameCamera.y + gameCamera.viewportHeight > room.bounds.y + room.bounds.h)
			gameCamera.y = room.bounds.y + room.bounds.h - gameCamera.viewportHeight;
	}
}

// checks if entire entity is inside area described by box (rudimentary "collision" until engine team implements it)
bool MyGame::checkInside(SDL_Rect box, DisplayObject* entity) {
	return (entity->position.x - entity->pivot.x >= box.x &&
		entity->position.x + entity->pivot.x <= box.x + box.w &&
		entity->position.y - entity->pivot.y >= box.y &&
		entity->position.y + entity->pivot.y <= box.y + box.h);
}