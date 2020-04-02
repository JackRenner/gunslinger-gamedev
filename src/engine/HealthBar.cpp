#include "HealthBar.h"

HealthBar::HealthBar(Player* player, int minVal, int maxVal){
    player->addEventListener(this,HealthEvent::HEALTH_CHANGE_EVENT);
    this->minVal = minVal;
    this->maxVal = maxVal;
    this->currVal = maxVal;
    this->position = {-50,-65};
    this->width = 100;
    this->height = 20;
    DisplayObject::loadRGBTexture(0,255,0);
}

void HealthBar::handleEvent(Event* e){
    this->currVal = ((Player *)e->getSource())->health;
    this->scaleX = (float)currVal/(float)maxVal;
}