#ifndef WEAPONSELECT_H
#define WEAPONSELECT_H

#include "../Sprite.h"
#include "../events/EventListener.h"
#include "../tweens/Tween.h"
#include "../tweens/TweenJuggler.h"
#include "Text.h"
#include "WeaponSelectEvent.h"

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
	Sprite* box2_unlock;
	Sprite* box2_lock;
	Sprite* box3_unlock;
	Sprite* box3_lock;
	Sprite* box4_unlock;
	Sprite* box4_lock;
	Sprite* box5;

	int food = 5;

	Sprite* selector;

	TweenJuggler* juggler;

};

#endif