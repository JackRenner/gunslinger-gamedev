#ifndef DEVTOOL_H
#define DEVTOOL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Game.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Scene.h"


using namespace std;

class DevTool : public Game {

public:
	DevTool();
	virtual ~DevTool();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);

	void setScene(Scene* scene);

private:

	Scene* curScene = NULL;

	Scene* testScene1;
	Scene* testScene2;

	bool sceneFlip = false;
	bool keyToggle = true;
};

#endif