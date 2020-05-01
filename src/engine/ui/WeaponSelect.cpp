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

	box0 = new Sprite("box0", "./resources/weapons/nosymbol.png");
	box0->width = 40;
	box0->height = 30;

	box1 = new Sprite("box1", "./resources/weapons/knife_ui.png");
	box1->width = 40;
	box1->height = 30;
	box1->position = { 0, 40 };

	box2_unlock = new Sprite("box2_unlock", "./resources/weapons/revolver_ui.png");
	box2_unlock->width = 40;
	box2_unlock->height = 30;
	box2_unlock->position = { 0, 80 };

	box2_lock = new Sprite("box2_lock", "./resources/wepaons/locked.png");
	box2_lock->width = 40;
	box2_lock->height = 30;
	box2_lock->position = { 0, 80 };

	box3_unlock = new Sprite("box3_unlock", "./resources/weapons/shotgun_ui.png");
	box3_unlock->width = 40;
	box3_unlock->height = 30;
	box3_unlock->position = { 0, 160 };

	box3_lock = new Sprite("box3_lock", "./resources/wepaons/locked.png");
	box3_lock->width = 40;
	box3_lock->height = 30;
	box3_lock->position = { 0, 160 };

	box4_unlock = new Sprite("box4_unlock", "./resources/weapons/rifle_ui.png");
	box4_unlock->width = 40;
	box4_unlock->height = 30;
	box4_unlock->position = { 0, 120 };

	box4_lock = new Sprite("box4_lock", "./resources/wepaons/locked.png");
	box4_lock->width = 40;
	box4_lock->height = 30;
	box4_lock->position = { 0, 120 };

	box5 = new Sprite("box5", "./resources/weapons/whiskey.png");
	box5->width = 40;
	box5->height = 30;
	box5->position = { 0, 200 };
	Text* placeholder5 = new Text(fontPath, std::to_string(this->food), 12, SDL_Color{ 10, 10, 10 }, 40);
	box5->addChild(placeholder5);

	selector = new Sprite("selector", 10, 10, 10);
	selector->width = 10;
	selector->height = 6;
	selector->pivot = { 5, 3 };
	selector->position = { 55, 15 };
	
	this->addChild(background);
	this->addChild(box0);
	this->addChild(box1);
	this->addChild(box2_lock);
	this->addChild(box3_lock);
	this->addChild(box4_lock);
	this->addChild(box5);
	this->addChild(selector);

	juggler = TweenJuggler::getInstance();
}

WeaponSelect::~WeaponSelect() {

}

void WeaponSelect::handleEvent(Event* e) {
	string type = e->getType();

	if (type == "unlock_pistol") {
		this->removeImmediateChild(box2_lock);
		this->addChild(box2_unlock);
		return;
	}
	else if (type == "unlock_rifle") {
		this->removeImmediateChild(box4_lock);
		this->addChild(box4_unlock);
		return;
	}
	else if (type == "unlock_shotgun") {
		this->removeImmediateChild(box3_lock);
		this->addChild(box3_unlock);
		return;
	}

	int newPos = selector->position.y;
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
	else if (type == "select_4") {
		newPos = 135;
	}
	else if (type == "select_3") {
		newPos = 175;
	}
	moveSelect->animate(TweenableParams::POS_Y, selector->position.y, newPos, 5, TweenTransitions::EASEINCUBIC);
	juggler->add(moveSelect);

	if(type == "heal" && this->food > 0) {
		this->food -=1;
		//cout << " landon" + to_string(this->food) + "test";
		string fontPath = "./resources/fonts/arial.ttf";
		Text* placeholder5 = new Text(fontPath, std::to_string(this->food), 12, SDL_Color{ 10, 10, 10 }, 40);

		this->box5->removeChild(0);
		this->box5->addChild(placeholder5);
		
	}
	if(type == "heal2") {
		this->food +=10;
		//cout << " landon" + to_string(this->food) + "test";
		string fontPath = "./resources/fonts/arial.ttf";
		Text* placeholder5 = new Text(fontPath, std::to_string(this->food), 12, SDL_Color{ 10, 10, 10 }, 40);

		this->box5->removeChild(0);
		this->box5->addChild(placeholder5);
		
	}

	if(type == "newlevel") {
		this->food +=10;
		cout << " landon" + to_string(this->food) + "test";
		string fontPath = "./resources/fonts/arial.ttf";
		Text* placeholder5 = new Text(fontPath, std::to_string(this->food), 12, SDL_Color{ 10, 10, 10 }, 40);

		this->box5->removeChild(0);
		this->box5->addChild(placeholder5);
	}
	

}