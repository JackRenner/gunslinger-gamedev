#include "WeaponEvent.h"

using namespace std;

const string WeaponEvent::SELECT_FIST_EVENT = "select_0";
const string WeaponEvent::SELECT_KNIFE_EVENT = "select_1";
const string WeaponEvent::SELECT_PISTOL_EVENT = "select_2";
const string WeaponEvent::SELECT_SHOTGUN_EVENT = "select_3";
const string WeaponEvent::SELECT_RIFLE_EVENT = "select_4";
const string WeaponEvent::UPDATE_AMMO = "update_ammo";

WeaponEvent::WeaponEvent(string type, Player* player) : Event(type, player) {

}

Player* WeaponEvent::getPlayer() {
	return this->source;
}