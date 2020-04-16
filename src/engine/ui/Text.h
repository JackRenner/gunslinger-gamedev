#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../Game.h"
#include <vector>
#include <string>

class Text : public DisplayObjectContainer {
public:
	Text(string fontPath, string text, int pts, SDL_Color fontColor, int sz);
	~Text();

private:
	TTF_Font* font = NULL;
	SDL_Color fontColor;

};

#endif