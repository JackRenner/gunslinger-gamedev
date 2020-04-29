#include "WeaponSelect.h"

using namespace std;

WeaponSelect::WeaponSelect() {
	this->type = "WeaponSelect";
	string fontPath = "./resources/fonts/arial.ttf";

	background = new Sprite("background", 230, 230, 230);
	background->width = 80;
	background->height = 250;
	background->position = { -10, -10 };
	background->alpha = 180;

	box0 = new Sprite("box0", 10, 10, 10);
	box0->width = 40;
	box0->height = 30;
	Text* placeholder0 = new Text(fontPath, "None", 12, SDL_Color{255, 255, 255}, 40);
	box0->addChild(placeholder0);

	box1 = new Sprite("box1", "./resources/weapons/knife_ui.png");
	box1->width = 40;
	box1->height = 30;
	box1->position = { 0, 40 };

	box2 = new Sprite("box2", "./resources/weapons/revolver_ui.png");
	box2->width = 40;
	box2->height = 30;
	box2->position = { 0, 80 };

	box3 = new Sprite("box3", "./resources/weapons/shotgun_ui.png");
	box3->width = 40;
	box3->height = 30;
	box3->position = { 0, 120 };

	box4 = new Sprite("box4", "./resources/weapons/rifle_ui.png");
	box4->width = 40;
	box4->height = 30;
	box4->position = { 0, 160 };

	box5 = new Sprite("box5", 10, 10, 10);
	box5->width = 40;
	box5->height = 30;
	box5->position = { 0, 200 };
	Text* placeholder5 = new Text(fontPath, "W:" + std::to_string(this->food), 12, SDL_Color{ 240, 240, 240 }, 40);
	box5->addChild(placeholder5);

	selector = new Sprite("selector", 10, 10, 10);
	selector->width = 10;
	selector->height = 6;
	selector->pivot = { 5, 3 };
	selector->position = { 55, 15 };
	
	this->addChild(background);
	this->addChild(box0);
	this->addChild(box1);
	this->addChild(box2);
	this->addChild(box3);
	this->addChild(box4);
	this->addChild(box5);
	this->addChild(selector);

	juggler = TweenJuggler::getInstance();
}

WeaponSelect::~WeaponSelect() {

}

void WeaponSelect::handleEvent(Event* e) {
	string type = e->getType();
	int newPos = 0;
	Tween* moveSelect = new Tween(selector);
	if (type == "select_0") {
		newPos = 15;
	}
	else if (type == "select_1") {
		newPos = 55;
	}
	else if (type == "select_2") {
		newPos = 95;
	}
	else if (type == "select_3") {
		newPos = 135;
	}
	else {
		newPos = 175;
	}
	moveSelect->animate(TweenableParams::POS_Y, selector->position.y, newPos, 5, TweenTransitions::EASEINCUBIC);
	juggler->add(moveSelect);

	if(type == "heal" && this->food > 0) {
		this->food -=1;
		//cout << " landon" + to_string(this->food) + "test";
		string fontPath = "./resources/fonts/arial.ttf";
		Text* placeholder5 = new Text(fontPath, "W:" + std::to_string(this->food), 12, SDL_Color{ 240, 240, 240 }, 40);

		this->box5->removeChild(0);
		this->box5->addChild(placeholder5);
	}
	

}