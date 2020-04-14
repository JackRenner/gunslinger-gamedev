#include <iostream>
#include "Game.h"
#include <string>
#include <ctime>
#include "DisplayObject.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include "CollisionSystem.h"

using namespace std;

SDL_Renderer* Game::renderer;
Game* Game::instance;
unsigned int Game::frameCounter = 0;

Game::Game(int windowWidth, int windowHeight){
	Game::instance = this;

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	initSDL();
	TTF_Init();

 	ourCollisionSystem = new CollisionSystem();
	this->MyEventDispatcher->addEventListener(ourCollisionSystem, "addObject");
	this->MyEventDispatcher->addEventListener(ourCollisionSystem, "removeObject");
	ourCollisionSystem->watchForCollisions("Projectile", "GangThug");
	ourCollisionSystem->watchForCollisions("Benemy", "Player");
	ourCollisionSystem->watchForCollisions("Wolf", "Wolf");
	//ourCollisionSystem->setSpecialCollisionType("Sprite", "Sprite", 1);
}

Game::~Game(){
	// DisplayObjectContainer::~DisplayObjectContainer ();
	quitSDL();
}

void Game::quitSDL(){
	cout << "Quitting sdl" << endl;
	SDL_DestroyRenderer(Game::renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void Game::initSDL(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
	IMG_Init(IMG_INIT_PNG);
	SDL_Init(SDL_INIT_AUDIO);

	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

	window = SDL_CreateWindow("myGame",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->windowWidth, this->windowHeight, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	Game::renderer = renderer;
}

void Game::start(){

	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
 			//SDL_GameControllerAddMapping("03000000790000004418000010010000,Mayflash GameCube Controller,a:b1,b:b2,dpdown:b14,dpleft:b15,dpright:b13,dpup:b12,lefttrigger:a3,leftx:a0,lefty:a1,rightshoulder:b7,righttrigger:a4,rightx:a5,righty:a2,start:b9,x:b0,y:b3,");
 			cout << SDL_NumJoysticks() << endl;
 			for (int i = 0; i < SDL_NumJoysticks(); ++i) {
 				if (SDL_IsGameController(i)) {
 						controller = SDL_GameControllerOpen(i);
 						printf("%s", "Found a controller!" );
 					    if (controller) {
 								cout << "Controller OPENED!";
								controllerPluggedIn = true;
 					       break;
 					       } else {
 					          fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
 					      }
 				}
 				else{
 					cout << "No Controller found";
 						printf("%s", "No Controller found");
 					}
 				}


	int ms_per_frame = (1.0/(double)this->frames_per_sec)*1000;
	std::clock_t start = std::clock();

	bool quit = false;
	SDL_Event event;

	while(!quit){
		std::clock_t end = std::clock();
		double duration = (( end - start ) / (double) CLOCKS_PER_SEC)*1000;
		if(duration > ms_per_frame){
			start = end;
			this->update(pressedKeys);
			AffineTransform at;
			this->draw(at);
		}

		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if(controllerPluggedIn == false){
					pressedKeys.insert(event.key.keysym.scancode);
				}
				break;
			case SDL_KEYUP:
				if(controllerPluggedIn == false){
					pressedKeys.erase(event.key.keysym.scancode);
				}
				break;
				/*
			case SDL_JOYDEVICEADDED:
				cout << "JoyDevice added!";
				break;
			case SDL_JOYDEVICEREMOVED:
				cout <<"JoyDevice Removed";
				break;
				*/
			case SDL_CONTROLLERDEVICEADDED:
				cout <<"Controller was added";
				controllerPluggedIn = true;
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				cout << "Controller Removed";
				controllerPluggedIn = false;
				break;
			case SDL_CONTROLLERBUTTONDOWN:
			/*
			SDL_CONTROLLER_BUTTON_INVALID
			SDL_CONTROLLER_BUTTON_A
			SDL_CONTROLLER_BUTTON_B
			SDL_CONTROLLER_BUTTON_X
			SDL_CONTROLLER_BUTTON_Y
			SDL_CONTROLLER_BUTTON_BACK
			SDL_CONTROLLER_BUTTON_GUIDE
			SDL_CONTROLLER_BUTTON_START
			SDL_CONTROLLER_BUTTON_LEFTSTICK
			SDL_CONTROLLER_BUTTON_RIGHTSTICK
			SDL_CONTROLLER_BUTTON_LEFTSHOULDER
			SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
			SDL_CONTROLLER_BUTTON_DPAD_UP
			SDL_CONTROLLER_BUTTON_DPAD_DOWN
			SDL_CONTROLLER_BUTTON_DPAD_LEFT
			SDL_CONTROLLER_BUTTON_DPAD_RIGHT
			SDL_CONTROLLER_BUTTON_MAX
			*/
//				cout << "Controller Button is Down!";
					cout << event.cbutton.button;
				if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
					//cout << "A Button is pressed down";
					pressedKeys.insert(SDL_SCANCODE_T);
				}
				if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
					//cout << "B Button is pressed down";
					pressedKeys.insert(SDL_SCANCODE_Y);
				}
				break;
				//GameButtonDown.insert(event.cbutton);
			case SDL_CONTROLLERBUTTONUP:
				if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
					pressedKeys.erase(SDL_SCANCODE_T);
				}
				if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
					pressedKeys.erase(SDL_SCANCODE_Y);
				}
				cout <<" : Controller Button is UP"<<endl;
//				if(GameButtonDown.erase(event.cbutton.button))
				//GameButtonUp.insert(event);
				//	if((event.cbutton = ))


				break;
				case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */

        			if( event.jaxis.axis == 0){ //Of Left Joystick
            			/* Left-right movement code goes here */
									if (event.jaxis.value > 3200){
											pressedKeys.insert(SDL_SCANCODE_RIGHT);
									}
									else if(event.jaxis.value < -3200){
											pressedKeys.insert(SDL_SCANCODE_LEFT);
									}
									if(event.jaxis.value < 3200 && event.jaxis.value > -3200){
										pressedKeys.erase(SDL_SCANCODE_RIGHT);
										pressedKeys.erase(SDL_SCANCODE_LEFT);
									}

        			}
        			if( event.jaxis.axis == 1)//LEFT JOYSTICK
        			{
            			/* Up-Down movement code goes here */
									if (event.jaxis.value > 3200){
											pressedKeys.insert(SDL_SCANCODE_DOWN);
									}
									else if(event.jaxis.value < -3200){
											pressedKeys.insert(SDL_SCANCODE_UP);
									}
									if(event.jaxis.value < 3200 && event.jaxis.value > -3200){
										pressedKeys.erase(SDL_SCANCODE_UP);
										pressedKeys.erase(SDL_SCANCODE_DOWN);
									}
							}

							if( event.jaxis.axis == 3){ //RIGHT STICK X
            			/* Left-right movement code goes here */
									if (event.jaxis.value > 3200){
											pressedKeys.insert(SDL_SCANCODE_D);
									}
									else if(event.jaxis.value < -3200){
											pressedKeys.insert(SDL_SCANCODE_A);
									}
									if(event.jaxis.value < 3200 && event.jaxis.value > -3200){
										pressedKeys.erase(SDL_SCANCODE_A);
										pressedKeys.erase(SDL_SCANCODE_D);
									}

        			}
        			if( event.jaxis.axis == 4)
        			{
            			/* Up-Down movement code goes here */
									if (event.jaxis.value > 3200){
											pressedKeys.insert(SDL_SCANCODE_S);
									}
									else if(event.jaxis.value < -3200){
											pressedKeys.insert(SDL_SCANCODE_W);
									}
									if(event.jaxis.value < 3200 && event.jaxis.value > -3200){
										pressedKeys.erase(SDL_SCANCODE_W);
										pressedKeys.erase(SDL_SCANCODE_S);
									}
							}

    break;



			/*
			case SDL_JOYBUTTONDOWN:
				cout << "JoyButton Down!";
				break;
			case SDL_JOYBUTTONUP:
				cout << "JOYBUTTON UP";
				break;
*/
		}

	}
}

void Game::update(set<SDL_Scancode> pressedKeys){
	frameCounter++;
	DisplayObjectContainer::update(pressedKeys);
	TweenJuggler::getInstance()->nextFrame();
	this->ourCollisionSystem->update();
}



void Game::draw(AffineTransform &at){
	SDL_RenderClear(Game::renderer);
	DisplayObjectContainer::draw(at);
	SDL_RenderPresent(Game::renderer);
}
