#include "CoinListener.h"

using namespace std;

TweenJuggler* juggler = TweenJuggler::getInstance();

CoinListener::CoinListener(DisplayObject* object) {
	this->displayObject = object;
}

void CoinListener::handleEvent(Event* e) {
	e->getSource()->removeEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
	Tween* fadeCoinTween = new Tween(this->displayObject);
	fadeCoinTween->animate(TweenableParams::ALPHA, 255, 0, 40, TweenTransitions::EASEOUTCUBIC);
	juggler->add(fadeCoinTween);
}