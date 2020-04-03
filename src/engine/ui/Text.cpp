#include "Text.h"

using namespace std;


Text::Text(string fontPath, string text, int pts, SDL_Color fontColor, int sz) {
	this->id = "text";
	font = TTF_OpenFont(fontPath.c_str(), pts);
	this->fontColor = fontColor;

	this->image = TTF_RenderText_Blended_Wrapped(font, text.c_str(), fontColor, sz);
	this->texture = SDL_CreateTextureFromSurface(Game::renderer, image);
	this->curTexture = texture;

	SDL_QueryTexture(curTexture, NULL, NULL, &(this->width), &(this->height));
}

Text::~Text() {
	TTF_CloseFont(font);
}