#include "AmmoCount.h"

using namespace std;

AmmoCount::AmmoCount() {
	this->type = "AmmoCount";
	juggler = TweenJuggler::getInstance();

	countArea = new DisplayObjectContainer();
	this->addChild(countArea);

	string fontPath = "./resources/fonts/arial.ttf";
	noneCount = new Text(fontPath, "- | -", 30, SDL_Color{ 255, 255, 255 }, 100);
	//noneCount.push_back(new Text(fontPath, "- | -", 30, SDL_Color{ 255, 255, 255 }, 100));

	knifeCount.push_back(new Text(fontPath, "1 | 1", 30, SDL_Color{ 255, 255, 255 }, 100));
	knifeCount.push_back(new Text(fontPath, "0 | 1", 30, SDL_Color{ 255, 255, 255 }, 100));

	pistolCount.push_back(new Text(fontPath, "6 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));
	pistolCount.push_back(new Text(fontPath, "5 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));
	pistolCount.push_back(new Text(fontPath, "4 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));
	pistolCount.push_back(new Text(fontPath, "3 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));
	pistolCount.push_back(new Text(fontPath, "2 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));
	pistolCount.push_back(new Text(fontPath, "1 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));
	pistolCount.push_back(new Text(fontPath, "0 | 6", 30, SDL_Color{ 255, 255, 255 }, 100));

	shotgunCount.push_back(new Text(fontPath, "2 | 2", 30, SDL_Color{ 255, 255, 255 }, 100));
	shotgunCount.push_back(new Text(fontPath, "1 | 2", 30, SDL_Color{ 255, 255, 255 }, 100));
	shotgunCount.push_back(new Text(fontPath, "0 | 2", 30, SDL_Color{ 255, 255, 255 }, 100));

	rifleCount.push_back(new Text(fontPath, "5 | 5", 30, SDL_Color{ 255, 255, 255 }, 100));
	rifleCount.push_back(new Text(fontPath, "4 | 5", 30, SDL_Color{ 255, 255, 255 }, 100));
	rifleCount.push_back(new Text(fontPath, "3 | 5", 30, SDL_Color{ 255, 255, 255 }, 100));
	rifleCount.push_back(new Text(fontPath, "2 | 5", 30, SDL_Color{ 255, 255, 255 }, 100));
	rifleCount.push_back(new Text(fontPath, "1 | 5", 30, SDL_Color{ 255, 255, 255 }, 100));
	rifleCount.push_back(new Text(fontPath, "0 | 5", 30, SDL_Color{ 255, 255, 255 }, 100));

	countArea->addChild(noneCount);
}

AmmoCount::~AmmoCount() {

}

void AmmoCount::handleEvent(Event* e) {
	string type = e->getType();
	Player* player = (Player*)(e->getSource());
	int weapon = player->gun;
	int index = 0;
	countArea->unlinkChild(0);
	switch (weapon) {
	case 0:
		countArea->addChild(noneCount);
		return;
	case 1:
		index = player->knife_throws;
		countArea->addChild(knifeCount[index]);
		return;
	case 2:
		index = player->revolver_shots;
		countArea->addChild(pistolCount[index]);
		return;
	case 3:
		index = player->shotgun_shots;
		countArea->addChild(shotgunCount[index]);
		return;
	case 4:
		index = player->rifle_shots;
		countArea->addChild(rifleCount[index]);
		return;
	}

	/*if (type == WeaponEvent::SELECT_FIST_EVENT) {
		countArea->addChild(noneCount);
	}
	else if (type == WeaponEvent::SELECT_KNIFE_EVENT) {
		index = player->knife_throws;
		countArea->addChild(knifeCount[index]);
	}
	else if (type == WeaponEvent::SELECT_PISTOL_EVENT) {
		index = player->revolver_shots;
		countArea->addChild(pistolCount[index]);
	}
	else if (type == WeaponEvent::SELECT_SHOTGUN_EVENT) {
		index = player->shotgun_shots;
		countArea->addChild(shotgunCount[index]);
	}
	else if (type == WeaponEvent::SELECT_RIFLE_EVENT) {
		index = player->rifle_shots;
		countArea->addChild(rifleCount[index]);
	}
	else {

	}*/
}