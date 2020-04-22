#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Text.h"
#include "../Sprite.h"
#include "../Controls.h"
#include <vector>
#include <string>

extern bool transLock;

class TextBox : public DisplayObjectContainer, public EventListener {
public:
	TextBox(SDL_Point position, int width, int height, int red, int blue, int green, int alpha);
	~TextBox();

	void addTextLine(string fontPath, string text, int pts, SDL_Color fontColor);

	void initBox();
	void closeBox();
	void drawNextLine();

	Sprite* background;

	//Text* displayText = NULL;
	int sz;

	const int TEXTBOX_MARGIN = 20;

	TweenJuggler* juggler;

	int nextLine = 0;
	int maxLine = 0;
	vector<Text*> textLines;

	virtual void handleEvent(Event* e);
	bool textLock = false;
	bool nextLock = false;

private:

};

#endif