#ifndef COINLISTENER_H
#define COINLISTENER_H

#include "../tweens/TweenEvent.h"
#include "../tweens/TweenJuggler.h"

using namespace std;

class TweenEvent;
class Tween;

class CoinListener : public EventListener {

public:
	CoinListener(DisplayObject* object);

	void handleEvent(Event* e);

private:

	DisplayObject* displayObject;
};

#endif