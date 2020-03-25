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

	cameraDemoScene = new Scene();
	cameraDemoScene->loadScene("./resources/scene/cameraDemoScene.txt");

	// character = new AnimatedSprite("character");
	// character->addAnimation("./resources/character/", "Run", 20, 2, true);
	character = new Player();
	// character->addAnimation("./resources/character/", "Down", 3, 1, true)


	this->setScene(cameraDemoScene);
	this->addChild(character);
	character->position = { 300, 1100 };
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
	if (pressedKeys.find(SDL_SCANCODE_M) != pressedKeys.end()) {
		gunshot->playSFX();
	}
	if (pressedKeys.find(SDL_SCANCODE_W) != pressedKeys.end()) {
		character->position.y -= 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_S) != pressedKeys.end()) {
		character->position.y += 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_D) != pressedKeys.end()) {
		character->position.x += 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_A) != pressedKeys.end()) {
		character->position.x -= 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_O) != pressedKeys.end()) {
		gameCamera.scale -= 0.05;
	}
	if (pressedKeys.find(SDL_SCANCODE_P) != pressedKeys.end()) {
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