#ifndef HEALTHEVENT_H
#define HEALTHEVENT_H

#include "Player.h"
#include "Event.h"
#include "Tween.h"

using namespace std;

class Player;

class HealthEvent : public Event {

public:
    HealthEvent(string type, Player* player);
	static const string HEALTH_CHANGE_EVENT;
    Player* getPlayer();

private:

    Player * source = NULL;

};

#endif