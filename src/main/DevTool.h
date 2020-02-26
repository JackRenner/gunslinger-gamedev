#ifndef DEVTOOL_H
#define DEVTOOL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Game.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Scene.h"

#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <guisan/opengl.hpp>
// A class used to load images for OpenGL using SDL
#include <guisan/opengl/openglsdlimageloader.hpp>


using namespace std;

class DevTool : public Game {

public:
	DevTool();
	virtual ~DevTool();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);
	void setScene(Scene* scene);


	/*
	 * Common stuff we need
	 */
	bool running = true;

	/*
	 * SDL Stuff we need
	 */
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Event event;

	/*
	 * Guichan OpenGL/SDL stuff we need
	 */
	gcn::SDLInput* input;					// Input driver
	gcn::OpenGLGraphics* graphics;		   // Graphics driver
	gcn::OpenGLSDLImageLoader* imageLoader;  // For loading images

	/*
	 * Guichan stuff we need
	 */
	gcn::Gui* gui;			// A Gui object - binds it all together
	gcn::Container* top;	  // A top container
	gcn::ImageFont* font;	 // A font
	gcn::Label* label;		// And a label for the Hello World text

	


private:

	Scene* curScene = NULL;

	Scene* testScene1;
	Scene* testScene2;

	bool sceneFlip = false;
	bool keyToggle = true;
};

#endif
