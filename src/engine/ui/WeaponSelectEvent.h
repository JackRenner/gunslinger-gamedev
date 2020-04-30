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
	static const string SELECT_RIFLE_EVENT;
	static const string SELECT_SHOTGUN_EVENT;
	static const string PLAYER_HEAL;
	static const string PLAYER_HEAL2;
	static const string NEW_LEVEL;
	static const string UPDATE_AMMO;
	static const string UNLOCK_PISTOL;
	static const string UNLOCK_RIFLE;
	static const string UNLOCK_SHOTGUN;

	Player* getPlayer();

private:
	string eventType = "";
	Player* source = NULL;

};

#endif