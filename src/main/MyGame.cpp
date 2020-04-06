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

	character = new AnimatedSprite("character");
	character-> id = "PlayerCharacter";
	character->addAnimation("./resources/character/", "Run", 20, 2, true);
	character->addAnimation("./resources/character/", "Walk", 20, 2, true);
	character->addAnimation("./resources/character/dead_spritesheet.png", "./resources/character/dead_spritesheet.xml", "Dead", 12, 5, true);

	object1 = new MyObject("type1", "./resources/coloredSquare/redSquare.png");
	object2 = new MyObject("type2", "./resources/coloredSquare/blueSquare.jpg");

	this->setScene(cameraDemoScene);
	this->addChild(character);



	character->position = { 300, 1100 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->play("Run");
	character->width = 90;

	character-> addChild(object1);
	character-> addChild(object2);

/*
//Used to test the remove code.
	character->removeImmediateChild(object1);

	object2->removeThis();
*/

	object1 -> height = 50;
	object1 -> width = 50;

	object1 -> position = {100, 0};

	object2 -> height = 50;
	object2 -> width = 50;
	object2 -> position = {-100, 0};

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
	object1->savePosition();
	object2->savePosition();
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
	if (pressedKeys.find(SDL_SCANCODE_Q) != pressedKeys.end()) {
		gameCamera.scale -= 0.05;
	}
	if (pressedKeys.find(SDL_SCANCODE_W) != pressedKeys.end()) {
		gameCamera.scale += 0.05;
	}
	if(pressedKeys.find(SDL_SCANCODE_T) != pressedKeys.end()){
		character->play("Dead");
	}
	if(pressedKeys.find(SDL_SCANCODE_Y) != pressedKeys.end()){
		character->play("Walk");
	}

	/*
	if(pressedKeys.find(SDL_SCANCODE_Z) != pressedKeys.end()){
			character->hitboxDrawn = true;
	}
	*/
	if(pressedKeys.find(SDL_SCANCODE_E) != pressedKeys.end()){
			character->rotation += .1;
	}
	if(pressedKeys.find(SDL_SCANCODE_R) != pressedKeys.end()){
			character->rotation -= .1;
	}
	if(pressedKeys.find(SDL_SCANCODE_F) != pressedKeys.end()){

					character->scaleX += .1;
					character->scaleY += .1;
	}
	if(pressedKeys.find(SDL_SCANCODE_G) != pressedKeys.end()){

			character->scaleX -= .1;
			character->scaleY -= .1;
	}




		if (pressedKeys.find(SDL_SCANCODE_I) != pressedKeys.end()) {
			object1->position.y -= 5;
		}
		if (pressedKeys.find(SDL_SCANCODE_K) != pressedKeys.end()) {
			object1->position.y += 5;
		}
		if (pressedKeys.find(SDL_SCANCODE_L) != pressedKeys.end()) {
			object1->position.x += 5;
		}
		if (pressedKeys.find(SDL_SCANCODE_J) != pressedKeys.end()) {
			object1->position.x -= 5;
		}
		if(pressedKeys.find(SDL_SCANCODE_N) != pressedKeys.end()){

						object1->scaleX += .1;
						object1->scaleY += .1;
		}
		if(pressedKeys.find(SDL_SCANCODE_M) != pressedKeys.end()){

				object1->scaleX -= .1;
				object1->scaleY -= .1;
		}

		if(pressedKeys.find(SDL_SCANCODE_O) != pressedKeys.end()){
				object1->rotation += .1;
		}
		if(pressedKeys.find(SDL_SCANCODE_P) != pressedKeys.end()){
				object1->rotation -= .1;
		}
	if (pressedKeys.find(SDL_SCANCODE_UP) != pressedKeys.end()) {

		object2->position.y -= 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_DOWN) != pressedKeys.end()) {

		object2->position.y += 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_RIGHT) != pressedKeys.end()) {

		object2->position.x += 5;
	}
	if (pressedKeys.find(SDL_SCANCODE_LEFT) != pressedKeys.end()) {

		object2->position.x -= 5;
	}

	if(pressedKeys.find(SDL_SCANCODE_X) != pressedKeys.end()){

					object2->scaleX += .1;
					object2->scaleY += .1;
	}
	if(pressedKeys.find(SDL_SCANCODE_C) != pressedKeys.end()){

			object2->scaleX -= .1;
			object2->scaleY -= .1;
	}

	if(pressedKeys.find(SDL_SCANCODE_V) != pressedKeys.end()){
			object2->rotation += .1;
	}
	if(pressedKeys.find(SDL_SCANCODE_B) != pressedKeys.end()){
			object2->rotation -= .1;
	}





/*
		if (pressedKeys.find(SDL_SCANCODE_W) != pressedKeys.end()) {
			character->play("Run");
			character->position.y -= 5;
		}
		if (pressedKeys.find(SDL_SCANCODE_S) != pressedKeys.end()) {
			character->play("Run");
			character->position.y += 5;
		}
		if (pressedKeys.find(SDL_SCANCODE_D) != pressedKeys.end()) {
			character->play("Run");
			character->position.x += 5;
		}
		if (pressedKeys.find(SDL_SCANCODE_A) != pressedKeys.end()) {
			character->play("Run");
			character->position.x -= 5;
		}
*/

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
