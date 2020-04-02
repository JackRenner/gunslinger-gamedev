#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "EventListener.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Player.h"
#include "HealthEvent.h"
#include "Game.h"

using namespace std;

class HealthBar : public EventListener, public Sprite {

public:

	HealthBar(Player* player, int minVal, int maxVal);
    void handleEvent(Event* e);
    int minVal = 0;
    int maxVal = 10;
    int currVal;

private:


};

#endif


