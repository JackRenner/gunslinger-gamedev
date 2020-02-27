#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include <string>
#include "DevTool.h"
#include "kiss_sdl.h"

using namespace std;

DevTool::DevTool() : Game(1200, 1000) {
	testScene1 = new Scene();
	testScene1->loadScene("./resources/scene/test1.txt");
	testScene2 = new Scene();
	testScene2->loadScene("./resources/scene/test2.txt");

	this->setScene(testScene1);
}

DevTool::~DevTool() {
	kiss_clean(objects);
}

void DevTool::setUpPreferences(){
	char a[] = {'P', 'r', 'e', 'f', 'e', 'r', 'e', 'n', 'c', 'e', 's', NULL};
	rendererDos = kiss_init(a, objects, 300, 500);

	pref = new kiss_window();
	kiss_window_new(pref, NULL, 0, 0, 0, kiss_screen_width, kiss_screen_height);

	//Add widgets for window here
	int windowWidth = pref->rect.w;
	int windowHeight = pref->rect.h;
	int row1Y = 0;
	int row2Y = windowHeight / 9;
	int row3Y = 2 * (windowHeight / 9);
	int row4Y = 3 * (windowHeight / 9);
	int row5Y = 4 * (windowHeight / 9);
	int row6Y = 5 * (windowHeight / 9);
	int row7Y = 6 * (windowHeight / 9);
	int row8Y = 7 * (windowHeight / 9);
	int row9Y = 8 * (windowHeight / 9);
	int col1X = 0;
	int col2X = windowWidth / 2;

	idLab = new kiss_label();
	char idC[] = {'i', 'd', ' ', '=', ' ', NULL};
	kiss_label_new(idLab, pref, idC, col1X, row1Y);

	idEnt = new kiss_entry();
	char id2C[] = {NULL};
	kiss_entry_new(idEnt, pref, 1, id2C, idLab->rect.x + 70, row1Y, 200);

	xLab = new kiss_label();
	char xC[] = {'x', ' ', '=', ' ', NULL};
	kiss_label_new(xLab, pref, xC, col1X, row2Y);

	xEnt = new kiss_entry();
	kiss_entry_new(xEnt, pref, 1, id2C, xLab->rect.x + 50, row2Y, 75);

	yLab = new kiss_label();
	char yC[] = {'y', ' ', '=', ' ', NULL};
	kiss_label_new(yLab, pref, yC, col2X, row2Y);

	yEnt = new kiss_entry();
	kiss_entry_new(yEnt, pref, 1, id2C, yLab->rect.x + 50, row2Y, 75);

	scaleLab = new kiss_label();
	char scaleC[] = {'s', 'c', 'a', 'l', 'e', ' ', '=', ' ', NULL};
	kiss_label_new(scaleLab, pref, scaleC, col1X, row3Y);
	
	scaleEnt = new kiss_entry();
	kiss_entry_new(scaleEnt, pref, 1, id2C, scaleLab->rect.x + 70, row3Y, 60);

	alphaLab = new kiss_label();
	char alphaC[] = {'a', 'l', 'p', 'h', 'a', ' ', '=', ' ', NULL};
	kiss_label_new(alphaLab, pref, alphaC, col2X, row3Y);

	alphaEnt = new kiss_entry();
	kiss_entry_new(alphaEnt, pref, 1, id2C, alphaLab->rect.x + 70, row3Y, 60);

	rotLab = new kiss_label();
	char rotC[] = {'r', 'o', 't', 'a', 't', 'i', 'o', 'n', ' ', '=', ' ', NULL};
	kiss_label_new(rotLab, pref, rotC, col1X, row4Y);

	rotEnt = new kiss_entry();
	kiss_entry_new(rotEnt, pref, 1, id2C, rotLab->rect.x + 100, row4Y, 100);

	deleteSprite = new kiss_button();
	char dsC[] = {'d', 'e', 'l', 'e', 't', 'e', ' ', 's', 'p', 'r', 'i', 't', 'e', NULL};
	deleteSprite->rect.w = 100;
	kiss_button_new(deleteSprite, pref, dsC, col2X - deleteSprite->rect.w / 3, row5Y);
	SDL_Color butText = {58, 252, 10};
	deleteSprite->textcolor = butText;

	saveSLab = new kiss_label();
	char sslC[] = {'r', 'e', 's', 'o', 'u', 'r', 'c', 'e', 's','/', 's', 'c', 'e', 'n', 'e', '/', NULL};
	kiss_label_new(saveSLab, pref, sslC, col1X, row6Y);

	saveSEnt = new kiss_entry();
	kiss_entry_new(saveSEnt, pref, 1, id2C, saveSLab->rect.x + 145, row6Y, 150);

	saveScene = new kiss_button();
	char ssC[] = {'s', 'a', 'v', 'e', ' ', 's', 'c', 'e', 'n', 'e', NULL};
	saveScene->rect.w = 100;
	kiss_button_new(saveScene, pref, ssC, col2X - saveScene->rect.w / 3, row7Y);
	saveScene->textcolor = butText;

	sceneDirLab = new kiss_label();
	kiss_label_new(sceneDirLab, pref, sslC, col1X, row8Y);

	sceneDirEnt = new kiss_entry();
	kiss_entry_new(sceneDirEnt, pref, 1, id2C, sceneDirLab->rect.x + 145, row8Y, 150);

	loadScene = new kiss_button();
	char lsC[] = {'l', 'o', 'a', 'd', ' ', 's', 'c', 'e', 'n', 'e', NULL};
	loadScene->rect.w = 100;
	kiss_button_new(loadScene, pref, lsC, col2X - loadScene->rect.w / 3, row9Y);
	loadScene->textcolor = butText;

	pref->visible = 1;

	SDL_RenderClear(rendererDos);

	kiss_window_draw(pref, rendererDos);
	kiss_label_draw(idLab, rendererDos);
	kiss_label_draw(xLab, rendererDos);
	kiss_label_draw(yLab, rendererDos);
	kiss_label_draw(scaleLab, rendererDos);
	kiss_label_draw(alphaLab, rendererDos);
	kiss_label_draw(rotLab, rendererDos);

	kiss_button_draw(deleteSprite, rendererDos);
	kiss_button_draw(saveScene, rendererDos);
	kiss_button_draw(loadScene, rendererDos);

	kiss_entry_draw(idEnt, rendererDos);
	kiss_entry_draw(xEnt, rendererDos);
	kiss_entry_draw(yEnt, rendererDos);
	kiss_entry_draw(scaleEnt, rendererDos);
	kiss_entry_draw(alphaEnt, rendererDos);
	kiss_entry_draw(rotEnt, rendererDos);

	kiss_label_draw(saveSLab, rendererDos);
	kiss_entry_draw(saveSEnt, rendererDos);
	kiss_label_draw(sceneDirLab, rendererDos);
	kiss_entry_draw(sceneDirEnt, rendererDos);

	SDL_RenderPresent(rendererDos);
}

void DevTool::setUpPictureSelector(){
	
}

void DevTool::setUpGrid(){

}

void DevTool::init(){
	//set up objects array
	objects = new kiss_array();
	kiss_array_new(objects);

	//sets up preferences window
	setUpPreferences();

	//sets up picture selector bar at top of screen
	setUpPictureSelector();

	//sets up grid for easy drag and drop
	setUpGrid();
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
