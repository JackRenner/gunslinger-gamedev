#include "WeaponSelectEvent.h"

using namespace std;

const string WeaponSelectEvent::SELECT_FIST_EVENT = "select_0";
const string WeaponSelectEvent::SELECT_KNIFE_EVENT = "select_1";
const string WeaponSelectEvent::SELECT_PISTOL_EVENT = "select_2";
const string WeaponSelectEvent::SELECT_SHOTGUN_EVENT = "select_3";
const string WeaponSelectEvent::SELECT_RIFLE_EVENT = "select_4";
const string WeaponSelectEvent::PLAYER_HEAL = "heal";
const string WeaponSelectEvent::UPDATE_AMMO = "update_ammo";
const string WeaponSelectEvent::UNLOCK_PISTOL = "unlock_pistol";
const string WeaponSelectEvent::UNLOCK_RIFLE = "unlock_rifle";
const string WeaponSelectEvent::UNLOCK_SHOTGUN = "unlock_shotgun";

WeaponSelectEvent::WeaponSelectEvent(string type, Player* player) : Event(type, player) {

}

Player* WeaponSelectEvent::getPlayer() {
	return this->source;
}