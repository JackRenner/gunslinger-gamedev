#ifndef AMMO_COUNT_H
#define AMMO_COUNT_H

#include "../Sprite.h"
#include "../events/EventListener.h"
#include "../tweens/Tween.h"
#include "../tweens/TweenJuggler.h"
#include "Text.h"
#include "WeaponSelectEvent.h"
#include "../Player.h"

class Player;
class WeaponSelectEvent;

class AmmoCount : public DisplayObjectContainer, public EventListener {
public:
	AmmoCount();
	~AmmoCount();

	virtual void handleEvent(Event* e);

private:
	TweenJuggler* juggler;

	DisplayObjectContainer* countArea;

	Text* noneCount;
	vector<Text*> knifeCount;
	vector<Text*> pistolCount;
	vector<Text*> shotgunCount;
	vector<Text*> rifleCount;
};

#endif