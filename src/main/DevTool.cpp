#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include "DevTool.h"

using namespace std;

DevTool::DevTool() : Game(1200, 1000) {

	testScene1 = new Scene();
	testScene1->loadScene("./resources/scene/test1.txt");
	testScene2 = new Scene();
	testScene2->loadScene("./resources/scene/test2.txt");

	this->setScene(testScene1);
}

DevTool::~DevTool() {

}


void DevTool::update(set<SDL_Scancode> pressedKeys) {
	Game::update(pressedKeys);

	if (pressedKeys.find(SDL_SCANCODE_P) != pressedKeys.end()) {
		if (keyToggle) {
			if (sceneFlip) {
				this->setScene(testScene1);
				sceneFlip = !sceneFlip;
			}
			else {
				this->setScene(testScene2);
				sceneFlip = !sceneFlip;
			}
		}
		keyToggle = false;
	}
	if (pressedKeys.find(SDL_SCANCODE_P) == pressedKeys.end()) {
		keyToggle = true;
	}
}

void DevTool::draw(AffineTransform& at) {
	Game::draw(at);
}

// sets the current scene and adds as child to game and unlinks the old scene from game (does not destroy it)
// we can tweak this to destroy the scene for memory reasons (or add a new method to destroy), but left it like this for now
void DevTool::setScene(Scene* scene) {
	if (curScene != NULL)
		this->unlinkImmediateChild(curScene->id);
	this->curScene = scene;
	if (curScene != NULL)
		this->addChild(curScene);
}