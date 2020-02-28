#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../engine/Sprite.h"
#include "../engine/Scene.h"
#include <vector>
#include <string>
#include <dirent.h>
#include "DevTool.h"
#include "kiss_sdl.h"

using namespace std;

DevTool::DevTool() : Game(1200, 1000) {
	curScene = new Scene();

	testScene1 = new Scene();
	testScene1->loadScene("./resources/scene/test1.txt");
	testScene2 = new Scene();
	testScene2->loadScene("./resources/scene/test2.txt");

	curScene->loadScene("./resources/scene/test1.txt");

	this->setScene(curScene);
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

	/*SDL_RenderClear(rendererDos);

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

	SDL_RenderPresent(rendererDos);*/
}

void DevTool::setUpPictureSelector(){
	//set up nice display rect thingy
	displayRect = new Sprite("rect", "./resources/dev-tool-resources/whiterect.png");
	displayRect->width = this->windowWidth;
	displayRect->alpha = 175;
	this->addChild(displayRect);

	//searches through sprites folder in resources and pulls all the files located therein
	DIR *d;
	struct dirent *dir;
	int i = 0;
	d = opendir("./resources/sprites/");
	if(d){
		while((dir = readdir(d)) != NULL){
			i++;
			string* name = new string();
			*name = dir->d_name;
			fileNames.push_back(name);
		}
		closedir(d);
	}
	//erases filenames that aren't .png
	for(i = 0; i < fileNames.size(); i++){
		string fileName = *fileNames[i];
		if(fileName.find(".png") == string::npos){
			fileNames.erase(fileNames.begin() + i);
		}
	}

	//creates display sprites
	Sprite* pic1 = new Sprite("sample1", resourceDirectory + *fileNames[0]);
	Sprite* pic2 = new Sprite("sample2", resourceDirectory + *fileNames[1]);
	Sprite* pic3 = new Sprite("sample3", resourceDirectory + *fileNames[2]);
	Sprite* pic4 = new Sprite("sample4", resourceDirectory + *fileNames[3]);
	Sprite* pic5 = new Sprite("sample5", resourceDirectory + *fileNames[4]);
	Sprite* pic6 = new Sprite("sample6", resourceDirectory + *fileNames[5]);
	Sprite* pic7 = new Sprite("sample7", resourceDirectory + *fileNames[6]);
	sampleSprites.push_back(pic1);
	sampleSprites.push_back(pic2);
	sampleSprites.push_back(pic3);
	sampleSprites.push_back(pic4);
	sampleSprites.push_back(pic5);
	sampleSprites.push_back(pic6);
	sampleSprites.push_back(pic7);

	int x1 = pic1->width / 2;
	int x2 = (this->windowWidth / 7) + (pic2->width / 2);
	int x3 = (2 * this->windowWidth / 7) + (pic3->width / 2);
	int x4 = (3 * this->windowWidth / 7) + (pic4->width / 2);
	int x5 = (4 * this->windowWidth / 7) + (pic5->width / 2);
	int x6 = (5 * this->windowWidth / 7) + (pic6->width / 2);
	int x7 = (6 * this->windowWidth / 7) + (pic7->width / 2);

	pic1->position.x = x1;
	pic2->position.x = x2;
	pic3->position.x = x3;
	pic4->position.x = x4;
	pic5->position.x = x5;
	pic6->position.x = x6;
	pic7->position.x = x7;

	for(Sprite* spr : sampleSprites){
		spr->alpha = 215;
		displayRect->addChild(spr);
	}
}

void DevTool::init(){
	//set up objects array
	objects = new kiss_array();
	kiss_array_new(objects);

	//sets up preferences window
	setUpPreferences();

	//sets up picture selector bar at top of screen
	setUpPictureSelector();

}

void DevTool::shiftSamples(bool direction){
	for(int k = sampleSprites.size() - 1; k >= 0; k--){
		int newIndex = fileIndex + k;
		//Changes index
		if(direction){
			newIndex++;
		} else {
			newIndex--;
		}
		//Checks to make sure doesn't overflow fileNames
		if(newIndex >= (signed)fileNames.size()){
			newIndex = newIndex - fileNames.size();
		} else if(newIndex < 0){
			newIndex = fileNames.size() - abs(newIndex);
		}
		//Sets fileIndex to new index of first sprite
		if(k == 0){
			fileIndex = newIndex;
		}
		//Sets actual resource name using newIndex
		sampleSprites[k]->loadTexture(resourceDirectory + *fileNames[newIndex]);
	}
}

void DevTool::createNewSprite(int index){
	string name = "new_sprite_" + spriteCreationInt;
	spriteCreationInt++;
	Sprite* newbie = new Sprite(name, resourceDirectory + *fileNames[index]);
	int scX = curScene->position.x;
	int scY = curScene->position.y;
	newbie->position.x = (double)this->windowWidth / 2.0 * (1 / curScene->scaleX) - scX;
	newbie->position.y = (double)this->windowHeight / 2.0 * (1 / curScene->scaleY) - scY;
	newbie->pivot.x = newbie->width / 2;
	newbie->pivot.y = newbie->height / 2;
	selectedSprite = newbie;
	curScene->addChild(newbie);
}

void DevTool::mousePressed(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	for(int u = 0; u < sampleSprites.size(); u++){
		int uBound = 0;
		int loBound = 100;
		int leBound = (u * this->windowWidth / sampleSprites.size()) + sampleSprites[u]->width / 2;
		int rBound = (u * this->windowWidth / sampleSprites.size()) + (int)(sampleSprites[u]->width * 1.5);
		if(x <= rBound && x >= leBound && y <= loBound && y >= uBound){
			if(selectedSprite == NULL){
				createNewSprite(u + fileIndex);
			}
			break;
		}
	}
}


void DevTool::updateFields(){

}

static void idEnt_Event(kiss_entry* idEnt, SDL_Event *e, int *drawFlag){
	idEnt->active = true;
	if(kiss_entry_event(idEnt, e, drawFlag)){
		char h[] = {'h','e','l','l','o', NULL};
		kiss_string_copy(idEnt->text, idEnt->textwidth / kiss_textfont.advance, h, NULL);
		*drawFlag = 1;
	}
}

void DevTool::drawEntries(){
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
	//idEnt->active = true;
	kiss_entry_draw(xEnt, rendererDos);
	//xEnt->active = true;
	kiss_entry_draw(yEnt, rendererDos);
	kiss_entry_draw(scaleEnt, rendererDos);
	kiss_entry_draw(alphaEnt, rendererDos);
	kiss_entry_draw(rotEnt, rendererDos);

	kiss_label_draw(saveSLab, rendererDos);
	kiss_entry_draw(saveSEnt, rendererDos);
	kiss_label_draw(sceneDirLab, rendererDos);
	kiss_entry_draw(sceneDirEnt, rendererDos);
}

void DevTool::update(set<SDL_Scancode> pressedKeys) {
	Game::update(pressedKeys);
	set<SDL_Scancode>::iterator it = pressedKeys.begin();

	sampleSwitchCount++;
	makeJerkyMovement++;
	int sampleSwitchTime = 15;
	int jerkyTime = 8;

	flashSelected++;
	int dimmer = 75;
	if(flashSelected > 10){
		if(selectedSprite != NULL){
			if(!dim){
				selectedSprite->alpha -= dimmer;
				dim = true;
			} else {
				selectedSprite->alpha += dimmer;
				dim = false;
			}
		}
		flashSelected = 0;
	}

	//Although it may seem fast, it guarantees everything stays within blocks
	int camSpeed = 50;
	int spriteSpeed = 50;

	while(it != pressedKeys.end()){
	  	switch(*it){
	  	case SDL_SCANCODE_Q:
			if(sampleSwitchCount > sampleSwitchTime){
		  		shiftSamples(true);
				sampleSwitchCount = 0;
			}
		  	break;
	  	case SDL_SCANCODE_W:
			if(sampleSwitchCount > sampleSwitchTime){
		  		shiftSamples(false);
				sampleSwitchCount = 0;
			}
		  	break;
		case SDL_SCANCODE_Z:
			if(makeJerkyMovement > jerkyTime){
				if(curScene->scaleX < 2.0){
					curScene->scaleX += 0.25;
					curScene->scaleY += 0.25;
				}
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_X:
			if(makeJerkyMovement > jerkyTime){
				if(curScene->scaleX > 0.25){
					curScene->scaleX -= 0.25;
					curScene->scaleY -= 0.25;
				}
				makeJerkyMovement = 0;
			}
			break;
	  	case SDL_SCANCODE_UP:
			if (makeJerkyMovement > jerkyTime){
				if(selectedSprite == NULL){
					curScene->position.y += camSpeed;
				}
				else{
					selectedSprite->position.y -= spriteSpeed;
				}
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_DOWN:
			if (makeJerkyMovement > jerkyTime){
				if(selectedSprite == NULL){
					curScene->position.y -= camSpeed;
				} else {
					selectedSprite->position.y += spriteSpeed;
				}
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_RIGHT:
			if (makeJerkyMovement > jerkyTime){
				if(selectedSprite == NULL){
					curScene->position.x -= camSpeed;
				} else {
					selectedSprite->position.x += spriteSpeed;
				}
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_LEFT:
			if (makeJerkyMovement > jerkyTime){
				if(selectedSprite == NULL){
					curScene->position.x += camSpeed;
				} else {
					selectedSprite->position.x -= spriteSpeed;
				}
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_RETURN:
			if(selectedSprite != NULL){
				if(dim){
					selectedSprite->alpha += dimmer;
					dim = false;
				}
				selectedSprite = NULL;
			}
			break;
		case SDL_SCANCODE_S:
			curScene->saveScene("Take2.txt");
			break;
		case SDL_SCANCODE_L:
		curScene = new Scene();
			curScene->loadScene("./resources/scene/test1.txt");
			this->setScene(curScene);
//			curScene->loadScene("./resources/scene/Trial.txt");
	//		this->setScene(curScene);
			break;
	  }
	  it++;
	}

	if(lastEvent){
		kiss_entry_event(xEnt, lastEvent, &drawFlag);
		kiss_entry_event(idEnt, lastEvent, &drawFlag);
		switch(lastEvent->type){
			case SDL_MOUSEBUTTONDOWN:
				if(!mouseDown){
					mousePressed();
				}
				mouseDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;
			case SDL_MOUSEMOTION:
				if(selectedSprite != NULL && mouseDown){
					selectedSprite->position.x = (double)lastEvent->motion.x * (1 / curScene->scaleX);
					selectedSprite->position.y = (double)lastEvent->motion.y * (1 / curScene->scaleY);

					selectedSprite->position.x -= (selectedSprite->position.x % 50);
					selectedSprite->position.y -= (selectedSprite->position.y % 50);
				}
				break;
		}
	}

	if(drawFlag){
		SDL_RenderClear(rendererDos);
		drawEntries();
		SDL_RenderPresent(rendererDos);
		drawFlag = 0;
	}
	updateFields();
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
