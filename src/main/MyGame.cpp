#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Sprite.h"
#include "MyGame.h"

using namespace std;

MyGame::MyGame() : Game(1200, 1000) {
	instance = this;
	girl = new Sprite("girl", "./resources/character/Dead_31.png");
	//girl->addAnimation("./resources/character/", "Walk", 20, 2, true);
	//girl->play("Walk");
	
	girl->position = {0,0};
	girl->width = 200;
	girl->height = 200;
	
	instance->addChild(girl);
	
	myCoin = new Coin("coin", "./resources/coin/dollar.png");
	myCoin->position = {600, 500};
	myCoin->height = 200;
	myCoin->width = 200;
	
	instance->addChild(myCoin);
	
	myQuestManager = new QuestManager();
	pickedUp = new Event("COIN_PICKED_UP", myCoin);
	myCoin->addEventListener(myQuestManager, pickedUp->getType());
}

MyGame::~MyGame(){
}


void MyGame::update(set<SDL_Scancode> pressedKeys){
	set<SDL_Scancode>::iterator it = pressedKeys.begin();

	while(it != pressedKeys.end()){
	  switch(*it){
	  case SDL_SCANCODE_RIGHT:
		  girl->position.x += 10;
		  break;
	  case SDL_SCANCODE_LEFT:
		  girl->position.x -= 10;
		  break;
	  case SDL_SCANCODE_UP:
		  girl->position.y -= 10;
		  break;
	  case SDL_SCANCODE_DOWN:
		  girl->position.y += 10;
		  break;
	  }
	  it++;
	}
	
	//Be careful if items for collision detection are different sizes
	
	SDL_Point uLGirl = {girl->position.x,girl->position.y};
	SDL_Point uRGirl = {girl->position.x + girl->width,girl->position.y};
	SDL_Point lLGirl = {girl->position.x,girl->position.y + girl->height};
	SDL_Point lRGirl = {girl->position.x + girl->width,girl->position.y + girl->height};
	
	SDL_Point uLCoin = {myCoin->position.x, myCoin->position.y};
	SDL_Point uRCoin = {myCoin->position.x + myCoin->width, myCoin->position.y};
	SDL_Point lLCoin = {myCoin->position.x, myCoin->position.y + myCoin->height};
	SDL_Point lRCoin = {myCoin->position.x + myCoin->height, myCoin->position.y + myCoin->height};
	
	bool rTol = (((uRGirl.x >= uLCoin.x) && (uRGirl.y >= uLCoin.y)) && (uRGirl.y <= lLCoin.y) && (uRGirl.x <= uRCoin.x)) || (((lRGirl.x >= uLCoin.x) && (lRGirl.y >= uLCoin.y)) && (lRGirl.y <= lLCoin.y) && (lRGirl.x <= uRCoin.x)); 
	bool lTor = (((uLGirl.x <= uRCoin.x) && (uLGirl.y >= uRCoin.y)) && (uLGirl.y <= lRCoin.y) && (uLGirl.x >= uLCoin.x)) || (((lLGirl.x <= uRCoin.x) && (lLGirl.y >= uRCoin.y)) && (lLGirl.y <= lRCoin.y) && (lLGirl.x >= uLCoin.x));
	bool tTob = (((uLGirl.x >= lLCoin.x) && (uLGirl.x <= lRCoin.x)) && ((uLGirl.y <= lLCoin.y) && (uLGirl.y >= uRCoin.y))) || (((uRGirl.x >= lLCoin.x) && (uRGirl.x <= lRCoin.x)) && ((uRGirl.y <= lLCoin.y) && (uRGirl.y >= uRCoin.y)));
	bool bToT = (((lLGirl.x >= uLCoin.x) && (lLGirl.x <= uRCoin.x)) && ((lLGirl.y >= uLCoin.y) && (lLGirl.y <= lLCoin.y))) || (((lRGirl.x >= uLCoin.x) && (lRGirl.x <= uRCoin.x)) && ((lRGirl.y >= uLCoin.y) && (lRGirl.y <= lLCoin.y)));
	
	if(rTol || lTor || tTob || bToT){
		myCoin->visible = false;
		myCoin->dispatchEvent(pickedUp);
	}
}

void MyGame::draw(AffineTransform &at){
	Game::draw(at);
}