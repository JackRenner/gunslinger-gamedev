#ifndef WEAPONSELECT_H
#define WEAPONSELECT_H

#include "../Sprite.h"
#include "../events/EventListener.h"
#include "../tweens/Tween.h"
#include "../tweens/TweenJuggler.h"
#include "../ui/Text.h"

class WeaponSelectEvent;

class WeaponSelect : public DisplayObjectContainer, public EventListener {
public:
	WeaponSelect();
	~WeaponSelect();

	virtual void handleEvent(Event* e);

private:
	Sprite* background;
	Sprite* box0;
	Sprite* box1;
	Sprite* box2;
	Sprite* box3;
	Sprite* box4;
	Sprite* box5;

	int food = 5;

	Sprite* selector;

	TweenJuggler* juggler;

};

#endif