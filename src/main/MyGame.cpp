#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "MyGame.h"

using namespace std;

Camera DisplayObject::gameCamera;

MyGame::MyGame() : Game(gameCamera.viewportWidth, gameCamera.viewportHeight) {

	gameCamera = Camera();

	foreground = new DisplayObjectContainer();
	foreground->id = "foreground";

	townScene = new Scene();
	townScene->loadScene("./resources/scene/townScene.txt");

	bankScene = new Scene();
	bankScene->loadScene("./resources/scene/bank.txt");

	character = new AnimatedSprite("character");
	character->addAnimation("./resources/character/", "Run", 20, 2, true);
	foreground->addChild(character);

	this->setScene(townScene);
	this->addChild(foreground);

	character->position = { 1500, 500 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->play("Run");
	character->width = 90;

	// initialize town transition points, hardcoded for now
	vector<TransitionPoint> townPoints = { { SDL_Point{ 192, 288 }, 1 } };
	transitionPoints.push_back(townPoints);

	vector<TransitionPoint> bankPoint = { { SDL_Point{ 500, 960 }, 0 } };
	transitionPoints.push_back(bankPoint);

	// initilizing scene info
	sceneInfo.push_back(SceneInfo{ townScene, SDL_Point{0, 0}, true, SDL_Point{0, character->height } });
	sceneInfo.push_back(SceneInfo{ bankScene, SDL_Point{500, 900} });

	room_state = 0;

	juggler = TweenJuggler::getInstance();
	Tween* characterTween = new Tween(character);
	characterTween->animate(TweenableParams::ALPHA, 0, 255, 30, TweenTransitions::EASEINCUBIC);
	characterTween->animate(TweenableParams::SCALE_X, 0, 1, 30, TweenTransitions::EASEOUTCUBIC);
	characterTween->animate(TweenableParams::SCALE_Y, 0, 1, 30, TweenTransitions::EASEOUTCUBIC);
	juggler->add(characterTween);
}

MyGame::~MyGame() {
	
}


void MyGame::update(set<SDL_Scancode> pressedKeys) {

	if (pressedKeys.find(SDL_SCANCODE_UP) != pressedKeys.end()) {
		character->position.y -= 8;
	}
	if (pressedKeys.find(SDL_SCANCODE_DOWN) != pressedKeys.end()) {
		character->position.y += 8;
	}
	if (pressedKeys.find(SDL_SCANCODE_RIGHT) != pressedKeys.end()) {
		character->position.x += 8;
	}
	if (pressedKeys.find(SDL_SCANCODE_LEFT) != pressedKeys.end()) {
		character->position.x -= 8;
	}


	Game::update(pressedKeys);

	gameCamera.x = character->position.x - gameCamera.viewportWidth / 2;
	gameCamera.y = character->position.y - gameCamera.viewportHeight / 2;

	for (int i = 0; i < transitionPoints[room_state].size(); i++) {
		if (checkInsidePoint(transitionPoints[room_state][i].point, character)) {
			transitionScene(transitionPoints[room_state][i]);
		}
	}

	//enforceCameraBounds();
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

// Add a camera bound given an area to enforce and bools for whether you want to enforce that cardinal direction for this bound
void MyGame::addCameraBound(SDL_Rect bounds, bool up, bool down, bool left, bool right) {
	Bound tmp_bound = Bound(bounds, up, down, left, right);
	boundaries.push_back(tmp_bound);
}

// Enforce camera bounds for the current room state. Does not currently account for room rotations.
void MyGame::enforceCameraBounds() {
	Bound room = boundaries[room_state];

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
	this->unlinkImmediateChild("foreground");
	this->setScene(sceneInfo[room_state].scenePointer);
	this->addChild(foreground);

	character->position = sceneInfo[room_state].startPos;

	e->getSource()->removeEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
	Tween* gameFade = new Tween(character);
	gameFade->animate(TweenableParams::ALPHA, 0, 255, 30, TweenTransitions::EASEINCUBIC);
	juggler->add(gameFade);
}

void MyGame::transitionScene(TransitionPoint tp) {
	if (sceneInfo[room_state].saveLast)
		sceneInfo[room_state].startPos = SDL_Point{ tp.point.x + sceneInfo[room_state].offset.x, tp.point.y + sceneInfo[room_state].offset.y };
	room_state = tp.newstate;

	Tween* gameFade = new Tween(character);
	gameFade->animate(TweenableParams::ALPHA, 255, 0, 30, TweenTransitions::EASEINCUBIC);
	juggler->add(gameFade);

	gameFade->addEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
}