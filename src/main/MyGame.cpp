#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "MyGame.h"

using namespace std;

Camera DisplayObject::gameCamera;

MyGame::MyGame() : Game(gameCamera.viewportWidth, gameCamera.viewportHeight) {

	gameCamera = Camera();

	tweenDemo = new Scene();
	tweenDemo->loadScene("./resources/scene/tweenDemo.txt");

	character = new AnimatedSprite("character");
	character->addAnimation("./resources/character/", "Run", 20, 2, true);

	coin = new Sprite("coin","./resources/dollar.png");
	coin->position = { 1000, 400 };
	coin->width = 50;
	coin->height = 50;
	coin->pivot = { coin->width / 2, coin->height / 2 };
	
	this->setScene(tweenDemo);
	this->addChild(character);
	this->addChild(coin);

	SDL_Rect room_bound = { -50, -50, 1700, 900 };

	addCameraBound(room_bound, true, true, true, true);

	character->position = { 800, 400 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->play("Run");
	character->width = 90;

	room_state = 0;

	gunshot = new Sound();
	//music = new Sound();
	//music->playMusic();

	zoomPoint = { room_bound.x + room_bound.w / 2, room_bound.y + room_bound.h / 2 };

	juggler = TweenJuggler::getInstance();
	Tween* characterTween = new Tween(character);
	characterTween->animate(TweenableParams::ALPHA, 0, 255, 30, TweenTransitions::EASEINCUBIC);
	characterTween->animate(TweenableParams::SCALE_X, 0, 1, 30, TweenTransitions::EASEOUTCUBIC);
	characterTween->animate(TweenableParams::SCALE_Y, 0, 1, 30, TweenTransitions::EASEOUTCUBIC);
	juggler->add(characterTween);

	coinListener = new CoinListener(coin);
}

MyGame::~MyGame() {
	
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

	if (!coinPickedUp && checkInside(SDL_Rect{ character->position.x - character->pivot.x, character->position.y - character->pivot.y, character->width, character->height }, coin)) {
		coinPickedUp = true;
		Tween* moveCoinTween = new Tween(coin);
		moveCoinTween->animate(TweenableParams::SCALE_X, 1, 4, 30, TweenTransitions::EASEOUTCUBIC);
		moveCoinTween->animate(TweenableParams::SCALE_Y, 1, 4, 30, TweenTransitions::EASEOUTCUBIC);
		moveCoinTween->animate(TweenableParams::POS_X, coin->position.x, 800, 30, TweenTransitions::EASEOUTCUBIC);
		moveCoinTween->animate(TweenableParams::POS_Y, coin->position.y, 400, 30, TweenTransitions::EASEOUTCUBIC);
		TweenJuggler::getInstance()->add(moveCoinTween);

		moveCoinTween->addEventListener(coinListener, TweenEvent::TWEEN_COMPLETE_EVENT);
	}

	juggler->nextFrame();

	/*SDL_Rect room;
	for (int i = 0; i < boundaries.size(); i++) {
		room = boundaries[i].bounds;
		if (checkInside(room, character)) {
			room_state = i;
		}
	}*/

	Game::update(pressedKeys);

	gameCamera.x = character->position.x - gameCamera.viewportWidth / 2;
	gameCamera.y = character->position.y - gameCamera.viewportHeight / 2;

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
		entity->position.x - entity->pivot.x + entity->width <= box.x + box.w &&
		entity->position.y - entity->pivot.y >= box.y &&
		entity->position.y - entity->pivot.y + entity->height <= box.y + box.h);
}