#include <iostream>
#include "Game.h"
#include <string>
#include <ctime>
#include "DisplayObject.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <SDL2/SDL_mixer.h>

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



}

Game::~Game(){
	// DisplayObjectContainer::~DisplayObjectContainer();
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
				pressedKeys.insert(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				pressedKeys.erase(event.key.keysym.scancode);
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
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				cout << "Controller Button is Down!";
				break;
			case SDL_CONTROLLERBUTTONUP:
				cout <<"Controller Button is UP"<<endl;
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
}

void Game::draw(AffineTransform &at){
	SDL_RenderClear(Game::renderer);
	DisplayObjectContainer::draw(at);
	SDL_RenderPresent(Game::renderer);
}
