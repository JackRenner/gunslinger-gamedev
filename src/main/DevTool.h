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

struct Bound {
	SDL_Rect bounds;
	bool check_up = true;
	bool check_down = true;
	bool check_left = true;
	bool check_right = true;

	Bound(SDL_Rect bounds, bool up, bool down, bool left, bool right) {
		this->bounds = bounds;
		this->check_up = up;
		this->check_down = down;
		this->check_left = left;
		this->check_right = right;
	};
};

class DevTool : public Game {

public:
	DevTool();
	virtual ~DevTool();

	void setUpPreferences();
	void setUpPictureSelector();
	void setUpGrid();
	void init();
	void createAnimation(AnimatedSprite* sprite, int index, string name);
	int getNumFrames(string xmlName);
	void shiftSamples(bool direction);
	void createNewSprite(int index);
	void createNewAnimatedSprite(int index);
	void pasteSprite();
	void mousePressed();
	void updateEntries();
	void drawEntries();
	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);
	void setScene(Scene* scene);
	bool checkInside(SDL_Rect box, DisplayObject* entity);
	void addCameraBound(SDL_Rect bounds, bool up, bool down, bool left, bool right);
	void enforceCameraBounds();

private:
	//SDL_Renderers for kiss windows
	SDL_Renderer* rendererDos;

	//main window id
	int mWindowId = 0;

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
	kiss_label* widthLab;
	kiss_entry* widthEnt;
	kiss_label* heightLab;
	kiss_entry* heightEnt;
	//objects dealing with loading and saving a scene
	kiss_label* saveSLab;
	kiss_entry* saveSEnt;
	kiss_button* saveScene;
	kiss_label* sceneDirLab;
	kiss_entry* sceneDirEnt;
	kiss_button* loadScene;

	Scene* curScene;
	DisplayObject* selectedSprite;
	DisplayObject* copySprite;
	SDL_Rect* basicSrcRect;
	//Alpha value for the selected sprite
	int ssAlpha;

	//flag to see if a mouse is currently down
	bool mouseDown = false;

	int drawFlag = 1;
	int spriteCreationInt = 0;
	int animSpriteCreationInt = 0;
	int newAnimationFrameRate = 5;
	int makeJerkyMovement = 0;
	int flashSelected = 0;
	bool dim = false;
	int typeCheck = 0;

	//Picture viewer
	vector<string*> fileNames;
	vector<string*> animationPNGFileNames;
	vector<string*> animationXMLFileNames;
	Sprite* displayRect;
	vector<AnimatedSprite*> sampleSprites;
	string resourceDirectory = "./resources/sprites/";
	int fileIndex = 0; //Keeps track of which filename the first sprite has
	int sampleSwitchCount = 0; //Makes sure shiftSamples doesn't happen too fast

	Scene* testScene1;
	Scene* testScene2;

	bool sceneFlip = false;
	bool keyToggle = true;

};

#endif
