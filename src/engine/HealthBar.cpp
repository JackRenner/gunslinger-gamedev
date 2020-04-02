#include "HealthBar.h"

HealthBar::HealthBar(Player* player, int minVal, int maxVal){
    player->addEventListener(this,HealthEvent::HEALTH_CHANGE_EVENT);
    this->minVal = minVal;
    this->maxVal = maxVal;
    this->currVal = maxVal;
    this->position = {-50,-65};
	this->red = 0;
	this->blue = 0;
	this->green = 255;
    this->width = 100;
    this->height = 20;
    SDL_Surface* image = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
	SDL_FillRect(image, NULL, SDL_MapRGB(image->format, red, green, blue));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, image);
	SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
	setTexture(texture);
}

void HealthBar::handleEvent(Event* e){
    this->currVal = ((Player *)e->getSource())->health;
    this->scaleX = (float)currVal/(float)maxVal;
}