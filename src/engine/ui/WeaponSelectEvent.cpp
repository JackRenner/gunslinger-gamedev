#include "WeaponSelectEvent.h"

using namespace std;

const string WeaponSelectEvent::SELECT_FIST_EVENT = "select_0";
const string WeaponSelectEvent::SELECT_KNIFE_EVENT = "select_1";
const string WeaponSelectEvent::SELECT_PISTOL_EVENT = "select_2";
const string WeaponSelectEvent::SELECT_SHOTGUN_EVENT = "select_3";
const string WeaponSelectEvent::SELECT_RIFLE_EVENT = "select_4";
const string WeaponSelectEvent::PLAYER_HEAL = "heal";
const string WeaponSelectEvent::UPDATE_AMMO = "update_ammo";

WeaponSelectEvent::WeaponSelectEvent(string type, Player* player) : Event(type, player) {

}

Player* WeaponSelectEvent::getPlayer() {
	return this->source;
}