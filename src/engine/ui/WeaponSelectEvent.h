#ifndef WEAPONSELECTEVENT_H
#define WEAPONSELECTEVENT_H

#include"../events/Event.h"
#include"../Player.h"

class Player;

class WeaponSelectEvent : public Event {
public:
	WeaponSelectEvent(string type, Player* player);

	static const string SELECT_FIST_EVENT;
	static const string SELECT_KNIFE_EVENT;
	static const string SELECT_PISTOL_EVENT;
	static const string SELECT_SHOTGUN_EVENT;
	static const string SELECT_RIFLE_EVENT;
	static const string PLAYER_HEAL;

	Player* getPlayer();

private:
	string eventType = "";
	Player* source = NULL;

};

#endif