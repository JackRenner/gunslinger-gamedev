#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "DisplayObjectContainer.h"
#include <vector>
#include <set>
#include <SDL2/SDL_mixer.h>
#include <map>
#include "CollisionSystem.h"


using namespace std;

class Game : public DisplayObjectContainer {

public:

	/* Singleton pattern */
	static Game* instance;

	int frames_per_sec = 60;
	int windowWidth, windowHeight;

	SDL_Window * window;
	static SDL_Renderer* renderer;

	//Global frame counter
	static unsigned int frameCounter;

	Game(int windowWidth, int windowHeight);
	virtual ~Game();
	void start();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

	bool controllerPluggedIn = false;

	SDL_Joystick* joystick = NULL;
	SDL_GameController *controller = NULL;

	CollisionSystem* ourCollisionSystem;

//	map<SDL_JoystickID , vector<uint8> > controllerButtonMap;
	//map<SDL_JoystickID, int > ControllerDirection;


	int LeftStickX;
	int leftStickY;
	int RightStickX;
	int RightStickY;


	set<Uint8> GameButtonUp;



	set<SDL_ControllerButtonEvent> GameButtonDown;
//	set<SDL_cbutton> GameButtonDown;

	set <SDL_JoyAxisEvent> JoyStickMovement;

private:

	void initSDL();
	void quitSDL();
	set<SDL_Scancode> pressedKeys;

};

#endif
