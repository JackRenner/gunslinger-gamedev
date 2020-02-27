#ifndef DEVTOOL_H
#define DEVTOOL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include "../engine/Game.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Scene.h"
#include "kiss_sdl.h"

using namespace std;

class DevTool : public Game {

public:
	DevTool();
	virtual ~DevTool();

	void setUpPreferences();
	void setUpPictureSelector();
	void setUpGrid();
	void init();
	void shiftSamples(bool direction);
	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);
	void setScene(Scene* scene);

private:
	//SDL_Renderers for kiss windows
	SDL_Renderer* rendererDos;

	//kiss thangs
	kiss_array* objects;
	kiss_window* pref;
	//pref objects
	kiss_entry* idEnt;
	kiss_label* idLab;
	kiss_entry* xEnt;
	kiss_label* xLab;
	kiss_entry* yEnt;
	kiss_label* yLab;
	kiss_entry* scaleEnt;
	kiss_label* scaleLab;
	kiss_entry* rotEnt;
	kiss_label* rotLab;
	kiss_entry* alphaEnt;
	kiss_label* alphaLab;
	kiss_button* deleteSprite;
	//objects dealing with loading and saving a scene
	kiss_label* saveSLab;
	kiss_entry* saveSEnt;
	kiss_button* saveScene;
	kiss_label* sceneDirLab;
	kiss_entry* sceneDirEnt;
	kiss_button* loadScene;

	Scene* curScene = NULL;
	Sprite* selectedSprite;

	//Picture viewer
	vector<string*> fileNames;
	Sprite* displayRect;
	vector<Sprite*> sampleSprites;
	string resourceDirectory = "./resources/sprites/";
	int fileIndex = 0; //Keeps track of which filename the first sprite has
	int sampleSwitchCount = 0; //Makes sure shiftSamples doesn't happen too fast

	Scene* testScene1;
	Scene* testScene2;

	bool sceneFlip = false;
	bool keyToggle = true;
};

#endif
