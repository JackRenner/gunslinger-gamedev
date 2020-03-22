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
	mWindowId = SDL_GetWindowID(this->window);
	curScene = new Scene();
	
	/*testScene1 = new Scene();
	testScene1->loadScene("./resources/scene/test1.txt");
	testScene2 = new Scene();
	testScene2->loadScene("./resources/scene/test2.txt");

	curScene->loadScene("./resources/scene/test1.txt");*/

	this->setScene(curScene);


	AnimatedSprite* character;
	character = new AnimatedSprite("character");
	character-> id = "TestingSpriteSheetID";
	character->addAnimation("./resources/character/", "Run", 20, 2, true);
	character->addAnimation("./resources/character/", "Walk", 20, 2, true);
	character->addAnimation("./resources/character/dead_spritesheet.png", "./resources/character/dead_spritesheet.xml", "Dead", 12, 5, true);

	character->position = { 500, 200 };
	character->pivot = { character->width / 2, character->height / 2 };
	character->play("Run");
	character->width = 90;

	curScene->addChild(character);

}

DevTool::~DevTool() {
	kiss_clean(objects);
}

void DevTool::setUpPreferences(){
	char a[KISS_MAX_LENGTH];
	strcpy(a, "Preferences");
	rendererDos = kiss_init(a, objects, 300, 500);

	pref = new kiss_window();
	kiss_window_new(pref, NULL, 0, 0, 0, kiss_screen_width, kiss_screen_height);

	//Add widgets for window here
	int windowWidth = pref->rect.w;
	int windowHeight = pref->rect.h;
	int row1Y = 0;
	int row2Y = windowHeight / 10;
	int row3Y = 2 * (windowHeight / 10);
	int row4Y = 3 * (windowHeight / 10);
	int row5Y = 4 * (windowHeight / 10);
	int row6Y = 5 * (windowHeight / 10);
	int row7Y = 6 * (windowHeight / 10);
	int row8Y = 7 * (windowHeight / 10);
	int row9Y = 8 * (windowHeight / 10);
	int row10Y = 9 * (windowHeight / 10);
	int col1X = 0;
	int col2X = windowWidth / 2;

	idLab = new kiss_label();
	char idC[KISS_MAX_LENGTH];
	strcpy(idC, "id = ");
	kiss_label_new(idLab, pref, idC, col1X, row1Y);

	idEnt = new kiss_entry();
	char id2C[KISS_MIN_LENGTH];
	strcpy(id2C, "");
	kiss_entry_new(idEnt, pref, 1, id2C, idLab->rect.x + 70, row1Y, 200);

	xLab = new kiss_label();
	char xC[KISS_MAX_LENGTH];
	strcpy(xC, "x = ");
	kiss_label_new(xLab, pref, xC, col1X, row2Y);

	xEnt = new kiss_entry();
	kiss_entry_new(xEnt, pref, 1, id2C, xLab->rect.x + 50, row2Y, 75);

	yLab = new kiss_label();
	char yC[KISS_MAX_LENGTH];
	strcpy(yC, "y = ");
	kiss_label_new(yLab, pref, yC, col2X, row2Y);

	yEnt = new kiss_entry();
	kiss_entry_new(yEnt, pref, 1, id2C, yLab->rect.x + 50, row2Y, 75);

	scaleLab = new kiss_label();
	char scaleC[KISS_MAX_LENGTH];
	strcpy(scaleC, "scale = ");
	kiss_label_new(scaleLab, pref, scaleC, col1X, row3Y);
	
	scaleEnt = new kiss_entry();
	kiss_entry_new(scaleEnt, pref, 1, id2C, scaleLab->rect.x + 70, row3Y, 60);

	alphaLab = new kiss_label();
	char alphaC[KISS_MAX_LENGTH];
	strcpy(alphaC, "alpha = ");
	kiss_label_new(alphaLab, pref, alphaC, col2X, row3Y);

	alphaEnt = new kiss_entry();
	kiss_entry_new(alphaEnt, pref, 1, id2C, alphaLab->rect.x + 70, row3Y, 60);

	rotLab = new kiss_label();
	char rotC[KISS_MAX_LENGTH];
	strcpy(rotC, "rotation = ");
	kiss_label_new(rotLab, pref, rotC, col1X, row4Y);

	rotEnt = new kiss_entry();
	kiss_entry_new(rotEnt, pref, 1, id2C, rotLab->rect.x + 100, row4Y, 100);

	widthLab = new kiss_label();
	char widthC[KISS_MAX_LENGTH];
	strcpy(widthC, "width = ");
	kiss_label_new(widthLab, pref, widthC, col1X, row5Y);

	widthEnt = new kiss_entry();
	kiss_entry_new(widthEnt, pref, 1, id2C, widthLab->rect.x + 70, row5Y, 75);

	heightLab = new kiss_label();
	char heightC[KISS_MAX_LENGTH];
	strcpy(heightC, "height = ");
	kiss_label_new(heightLab, pref, heightC, col2X, row5Y);

	heightEnt = new kiss_entry();
	kiss_entry_new(heightEnt, pref, 1, id2C, heightLab->rect.x + 75, row5Y, 75);

	deleteSprite = new kiss_button();
	char dsC[KISS_MAX_LENGTH];
	strcpy(dsC, "delete sprite");
	deleteSprite->rect.w = 100;
	kiss_button_new(deleteSprite, pref, dsC, col2X - deleteSprite->rect.w / 3, row6Y);
	SDL_Color butText = {58, 252, 10};
	deleteSprite->textcolor = butText;

	saveSLab = new kiss_label();
	char sslC[KISS_MAX_LENGTH];
	strcpy(sslC, "resources/scene/");
	kiss_label_new(saveSLab, pref, sslC, col1X, row7Y);

	saveSEnt = new kiss_entry();
	kiss_entry_new(saveSEnt, pref, 1, id2C, saveSLab->rect.x + 145, row7Y, 150);

	saveScene = new kiss_button();
	char ssC[KISS_MAX_LENGTH];
	strcpy(ssC, "save scene");
	saveScene->rect.w = 100;
	kiss_button_new(saveScene, pref, ssC, col2X - saveScene->rect.w / 3, row8Y);
	saveScene->textcolor = butText;

	sceneDirLab = new kiss_label();
	kiss_label_new(sceneDirLab, pref, sslC, col1X, row9Y);

	sceneDirEnt = new kiss_entry();
	kiss_entry_new(sceneDirEnt, pref, 1, id2C, sceneDirLab->rect.x + 145, row9Y, 150);

	loadScene = new kiss_button();
	char lsC[KISS_MAX_LENGTH];
	strcpy(lsC, "load scene");
	loadScene->rect.w = 100;
	kiss_button_new(loadScene, pref, lsC, col2X - loadScene->rect.w / 3, row10Y);
	loadScene->textcolor = butText;

	pref->visible = 1;
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
	string name = "new_sprite_" + to_string(spriteCreationInt);
	spriteCreationInt++;
	Sprite* newbie = new Sprite(name, resourceDirectory + *fileNames[index]);
	int scX = curScene->position.x;
	int scY = curScene->position.y;
	newbie->position.x = (double)this->windowWidth / 2.0 * (1 / curScene->scaleX) - scX;
	newbie->position.y = (double)this->windowHeight / 2.0 * (1 / curScene->scaleY) - scY;
	newbie->pivot.x = newbie->width / 2;
	newbie->pivot.y = newbie->height / 2;
	ssAlpha = newbie->alpha;
	selectedSprite = newbie;
	curScene->addChild(newbie);
}

void DevTool::mousePressed(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	bool clickedUpperBar = false;
	for(int u = 0; u < sampleSprites.size(); u++){
		int uBound = 0;
		int loBound = 100;
		int leBound = (u * this->windowWidth / sampleSprites.size()) + sampleSprites[u]->width / 2;
		int rBound = (u * this->windowWidth / sampleSprites.size()) + (int)(sampleSprites[u]->width * 1.5);
		if(x <= rBound && x >= leBound && y <= loBound && y >= uBound){
			if(selectedSprite == NULL){
				clickedUpperBar = true;
				createNewSprite(u + fileIndex);
			}
			break;
		}
	}

	if(!clickedUpperBar){
		for(int h = curScene->children.size() - 1; h >= 0; h--){
			DisplayObject* next = curScene->children.at(h);
			if(next == NULL){
				continue;
			}
			int maxX, maxY, minX, minY;
			maxX = next->transformedOrigin.x;
			minX = next->transformedOrigin.x;
			maxY = next->transformedOrigin.y;
			minY = next->transformedOrigin.y;
			if(next->transformedURight.x > maxX){
				maxX = next->transformedURight.x;
			}
			if(next->transformedLLeft.x > maxX){
				maxX = next->transformedLLeft.x;
			}
			if(next->transformedLRight.x > maxX){
				maxX = next->transformedLRight.x;
			}

			if(next->transformedURight.x < minX){
				minX = next->transformedURight.x;
			}
			if(next->transformedLLeft.x < minX){
				minX = next->transformedLLeft.x;
			}
			if(next->transformedLRight.x < minX){
				minX = next->transformedLRight.x;
			}

			if(next->transformedURight.y > maxY){
				maxY = next->transformedURight.y;
			}
			if(next->transformedLLeft.y > maxY){
				maxY = next->transformedLLeft.y;
			}
			if(next->transformedLRight.y > maxY){
				maxY = next->transformedLRight.y;
			}

			if(next->transformedURight.y < minY){
				minY = next->transformedURight.y;
			}
			if(next->transformedLLeft.y < minY){
				minY = next->transformedLLeft.y;
			}
			if(next->transformedLRight.y < minY){
				minY = next->transformedLRight.y;
			}

			if(x < maxX && x > minX && y < maxY && y > minY){
				if(selectedSprite == NULL){
					ssAlpha = next->alpha;
					next->alpha = 255;
					selectedSprite = next;
				}
				break;
			}
		}
	}
}

void DevTool::updateEntries(){
	if(lastEvent->window.windowID == mWindowId){
		char idC[selectedSprite->id.length()];
		strcpy(idC, selectedSprite->id.c_str());
		kiss_string_copy(idEnt->text, idEnt->textwidth / kiss_textfont.advance, idC, NULL);
	
		char xC[to_string(selectedSprite->position.x).length()];
		strcpy(xC, to_string(selectedSprite->position.x).c_str());
		kiss_string_copy(xEnt->text, xEnt->textwidth / kiss_textfont.advance, xC, NULL);

		char yC[to_string(selectedSprite->position.y).length()];
		strcpy(yC, to_string(selectedSprite->position.y).c_str());
		kiss_string_copy(yEnt->text, yEnt->textwidth / kiss_textfont.advance, yC, NULL);

		char scaleC[to_string(selectedSprite->scaleX).length()];
		strcpy(scaleC, to_string(selectedSprite->scaleX).c_str());
		kiss_string_copy(scaleEnt->text, scaleEnt->textwidth / kiss_textfont.advance, scaleC, NULL);

		char rotC[to_string(selectedSprite->rotation).length()];
		strcpy(rotC, to_string(selectedSprite->rotation).c_str());
		kiss_string_copy(rotEnt->text, rotEnt->textwidth / kiss_textfont.advance, rotC, NULL);

		char alphaC[to_string(ssAlpha).length()];
		strcpy(alphaC, to_string(ssAlpha).c_str());
		kiss_string_copy(alphaEnt->text, alphaEnt->textwidth / kiss_textfont.advance, alphaC, NULL);

		char widthC[to_string(selectedSprite->width).length()];
		strcpy(widthC, to_string(selectedSprite->width).c_str());
		kiss_string_copy(widthEnt->text, widthEnt->textwidth / kiss_textfont.advance, widthC, NULL);

		char heightC[to_string(selectedSprite->height).length()];
		strcpy(heightC, to_string(selectedSprite->height).c_str());
		kiss_string_copy(heightEnt->text, heightEnt->textwidth / kiss_textfont.advance, heightC, NULL);

		drawFlag = 1;
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
	kiss_label_draw(widthLab, rendererDos);
	kiss_label_draw(heightLab, rendererDos);

	kiss_button_draw(deleteSprite, rendererDos);
	kiss_button_draw(saveScene, rendererDos);
	kiss_button_draw(loadScene, rendererDos);

	kiss_entry_draw(idEnt, rendererDos);
	kiss_entry_draw(xEnt, rendererDos);
	kiss_entry_draw(yEnt, rendererDos);
	kiss_entry_draw(scaleEnt, rendererDos);
	kiss_entry_draw(alphaEnt, rendererDos);
	kiss_entry_draw(rotEnt, rendererDos);
	kiss_entry_draw(widthEnt, rendererDos);
	kiss_entry_draw(heightEnt, rendererDos);

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
			if(sampleSwitchCount > sampleSwitchTime && lastEvent->window.windowID == mWindowId){
		  		shiftSamples(true);
				sampleSwitchCount = 0;
			}
		  	break;
	  	case SDL_SCANCODE_W:
			if(sampleSwitchCount > sampleSwitchTime && lastEvent->window.windowID == mWindowId){
		  		shiftSamples(false);
				sampleSwitchCount = 0;
			}
		  	break;
		case SDL_SCANCODE_Z:
			if(makeJerkyMovement > jerkyTime && lastEvent->window.windowID == mWindowId){
				if(curScene->scaleX < 2.0){
					curScene->scaleX += 0.25;
					curScene->scaleY += 0.25;
					if((int)(curScene->scaleX * 100) % 50 == 0){
						gridScale = curScene->scaleX;
					}
				}
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_X:
			if(makeJerkyMovement > jerkyTime && lastEvent->window.windowID == mWindowId){
				if(curScene->scaleX > 0.5){
					curScene->scaleX -= 0.25;
					curScene->scaleY -= 0.25;
					if((int)(curScene->scaleX * 100) % 50 == 0){
						gridScale = curScene->scaleX;
					}
				}
				makeJerkyMovement = 0;
			}
			break;
	  	case SDL_SCANCODE_UP:
			if (makeJerkyMovement > jerkyTime && lastEvent->window.windowID == mWindowId){
				double s = curScene->scaleX;
				curScene->scaleX = 0;
				curScene->scaleY = 0;
				if(selectedSprite == NULL){
					curScene->position.y -= camSpeed;
				}
				else{
					selectedSprite->position.y -= spriteSpeed;
				}
				curScene->scaleX = s;
				curScene->scaleY = s;
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_DOWN:
			if (makeJerkyMovement > jerkyTime && lastEvent->window.windowID == mWindowId){
				double s = curScene->scaleX;
				curScene->scaleX = 0;
				curScene->scaleY = 0;
				if(selectedSprite == NULL){
					curScene->position.y += camSpeed;
				} else {
					selectedSprite->position.y += spriteSpeed;
				}
				curScene->scaleX = s;
				curScene->scaleY = s;
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_RIGHT:
			if (makeJerkyMovement > jerkyTime && lastEvent->window.windowID == mWindowId){		
				double s = curScene->scaleX;
				curScene->scaleX = 0;
				curScene->scaleY = 0;
				if(selectedSprite == NULL){
					curScene->position.x += camSpeed;
				} else {
					selectedSprite->position.x += spriteSpeed;
				}
				curScene->scaleX = s;
				curScene->scaleY = s;
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_LEFT:
			if (makeJerkyMovement > jerkyTime && lastEvent->window.windowID == mWindowId){		
				double s = curScene->scaleX;
				curScene->scaleX = 0;
				curScene->scaleY = 0;
				if(selectedSprite == NULL){
					curScene->position.x -= camSpeed;
				} else {
					selectedSprite->position.x -= spriteSpeed;
				}
				curScene->scaleX = s;
				curScene->scaleY = s;
				makeJerkyMovement = 0;
			}
			break;
		case SDL_SCANCODE_RETURN:
			if(selectedSprite != NULL && lastEvent->window.windowID == mWindowId){
				selectedSprite->alpha = ssAlpha;
				if(dim){
					dim = false;
				}
				selectedSprite = NULL;
			}
			break;
	  }
	  it++;
	}

	typeCheck++;
	int typeCheckLength = 6;

	if(lastEvent){
		if(typeCheck > typeCheckLength){
			if(kiss_entry_event(saveSEnt, lastEvent, &drawFlag) != 0){
				//cout << "saveSEnt enter pressed" << endl;
				cout << saveSEnt->text << endl;
			}
			if(kiss_entry_event(sceneDirEnt, lastEvent, &drawFlag) != 0){
				//cout << "sceneDirEnt enter pressed" << endl;
			}
			if(selectedSprite != NULL){
				if(kiss_entry_event(idEnt, lastEvent, &drawFlag) != 0){
					selectedSprite->id = idEnt->text;
				}
				if(kiss_entry_event(xEnt, lastEvent, &drawFlag) != 0){
					int x;
					try{
						x = stoi(xEnt->text);
						selectedSprite->position.x = x;
					} catch (invalid_argument){
						cout << "Invalid x argument" << endl;
					}
				}
				if(kiss_entry_event(yEnt, lastEvent, &drawFlag) != 0){
					int y;
					try{
						y = stoi(yEnt->text);
						selectedSprite->position.y = y;
					} catch (invalid_argument){
						cout << "Invalid y argument" << endl;
					}
				}
				if(kiss_entry_event(scaleEnt, lastEvent, &drawFlag) != 0){
					double scale;
					try{
						scale = stod(scaleEnt->text);
						selectedSprite->scaleX = scale;
						selectedSprite->scaleY = scale;
					} catch (invalid_argument){
						cout << "Invalid scale argument" << endl;
					}
				}
				if(kiss_entry_event(rotEnt, lastEvent, &drawFlag) != 0){
					double rot;
					try{
						rot = stod(rotEnt->text);
						selectedSprite->rotation = rot;
					} catch (invalid_argument){
						cout << "Invalid rotation argument" << endl;
					}
				}
				if(kiss_entry_event(alphaEnt, lastEvent, &drawFlag) != 0){
					int alpha;
					try{
						alpha = stoi(alphaEnt->text);
						ssAlpha = alpha;
					} catch (invalid_argument){
						cout << "Invalid alpha argument" << endl;
					}
				}
				if(kiss_entry_event(heightEnt, lastEvent, &drawFlag) != 0){
					int h;
					try{
						h = stoi(heightEnt->text);
						selectedSprite->height = h;
					} catch (invalid_argument){
						cout << "Invalid height argument" << endl;
					}
				}
				if(kiss_entry_event(widthEnt, lastEvent, &drawFlag) != 0){
					int w;
					try{
						w = stoi(widthEnt->text);
						selectedSprite->width = w;
					} catch (invalid_argument){
						cout << "Invalid width argument" << endl;
					}
				}
			}
			typeCheck = 0;
		}
		if(kiss_button_event(saveScene, lastEvent, &drawFlag) != 0){
			string sceneToSave = "./resources/scene/";
			for(char c : saveSEnt->text){
				sceneToSave.push_back(c);
			}
			cout << "Saving scene " << sceneToSave << endl;
			curScene->saveScene(sceneToSave);
		}
		if(kiss_button_event(loadScene, lastEvent, &drawFlag) != 0){
			Scene* newScene = new Scene();

			string sceneToLoad = "./resources/scene/";
			for(char c : sceneDirEnt->text){
				sceneToLoad.push_back(c);
			}
			cout << "Loading scene " << sceneToLoad << endl;
			newScene->loadScene(sceneToLoad);
			this->setScene(newScene);
			curScene = newScene;
		}
		if(selectedSprite != NULL){
			if(kiss_button_event(deleteSprite, lastEvent, &drawFlag) != 0){
				curScene->removeImmediateChild(selectedSprite);
				selectedSprite = NULL;
			}
		}
		if(lastEvent->window.windowID == mWindowId){
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
	}

	if(selectedSprite != NULL){
		updateEntries();
	}

	if(drawFlag){
		SDL_RenderClear(rendererDos);
		drawEntries();
		SDL_RenderPresent(rendererDos);
		drawFlag = 0;
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