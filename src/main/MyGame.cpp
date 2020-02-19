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
	character->addAnimation("./resources/character/", "Run", 20, 2, true);

	this->setScene(cameraDemoScene);
	this->addChild(character);
	character->affixToCamera = true;
	character->position = { 300, 1100 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->play("Run");
	character->width = 90;
	character->affixPoint = { (gameCamera.viewportWidth / 2), (gameCamera.viewportHeight / 2) };
}

MyGame::~MyGame() {

}


void MyGame::update(set<SDL_Scancode> pressedKeys) {
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

	Game::update(pressedKeys);
}

void MyGame::draw(AffineTransform& at) {
	Game::draw(at);
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