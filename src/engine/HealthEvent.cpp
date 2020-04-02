#include "HealthEvent.h"

const string HealthEvent::HEALTH_CHANGE_EVENT = "health_changed";

HealthEvent::HealthEvent(string type, Player* player) : Event(type, player) {

}

Player* HealthEvent::getPlayer(){
    return this->source;
}