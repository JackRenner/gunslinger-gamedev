#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include "DevTool.h"
/*
#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <guisan/opengl.hpp>
// A class used to load images for OpenGL using SDL
#include <guisan/opengl/openglsdlimageloader.hpp>
*/
using namespace std;

DevTool::DevTool() : Game(1200, 1000) {
	testScene1 = new Scene();
	testScene1->loadScene("./resources/scene/test1.txt");
	testScene2 = new Scene();
	testScene2->loadScene("./resources/scene/test2.txt");

	this->setScene(testScene1);

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("GuisanAttributeWindow",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);


/*
		// Attach GL context to our window
		context = SDL_GL_CreateContext(window);

		// Setup OpenGL
		glViewport(0, 0, 640, 480);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// We want to enable key repeat
		//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		imageLoader = new gcn::OpenGLSDLImageLoader();

		// The ImageLoader in use is static and must be set to be
		// able to load images
		gcn::Image::setImageLoader(imageLoader);
		graphics = new gcn::OpenGLGraphics();
		// We need to tell OpenGL graphics how big the screen is.
		graphics->setTargetPlane(640, 480);
		input = new gcn::SDLInput();

		top = new gcn::Container();
		// Set the dimension of the top container to match the screen.
		top->setDimension(gcn::Rectangle(0, 0, 640, 480));
		gui = new gcn::Gui();
		// Set gui to use the OpenGLGraphics object.
		gui->setGraphics(graphics);
		// Set gui to use the SDLInput object


		gui->setInput(input);
		// Set the top container

		gui->setTop(top);
		// Load the image font.
		font = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		// The global font is static and must be set.
		gcn::Widget::setGlobalFont(font);

		// Create a label with test hello world
		label = new gcn::Label("Hello World");
		// Set the labels position
		label->setPosition(280, 220);
		// Add the label to the top container
		top->add(label);

*/

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
