#include "TextBox.h"

using namespace std;

TextBox::TextBox(SDL_Point position, int width, int height, int red, int green, int blue, int alpha) {
	this->type = "TextBox";
	this->position = position;
	background = new Sprite("textbox", red, green, blue);
	background->width = width;
	background->height = height;
	background->position = { width / 2, height / 2 };
	background->pivot = { width / 2, height / 2 };
	background->alpha = alpha;

	sz = width - (TEXTBOX_MARGIN * 2);

	juggler = TweenJuggler::getInstance();

	this->addChild(background);
	background->scaleX = 0.0;
	background->scaleY = 0.0;
}

TextBox::~TextBox() {
	if (background->children.size() != 0)
		background->unlinkChild(0);
	for (int i = 0; i < textLines.size(); i++) {
		delete textLines[i];
	}
}

void TextBox::addTextLine(string fontPath, string text, int pts, SDL_Color fontColor) {

	Text* displayText = new Text(fontPath, text, pts, fontColor, this->sz);
	displayText->position = { TEXTBOX_MARGIN, TEXTBOX_MARGIN };
	displayText->pivot = { background->width / 2, background->height / 2 };

	textLines.push_back(displayText);
	this->maxLine++;
}

void TextBox::initBox() {
	textLock = true;
	this->nextLine = 0;

	background->addChild(textLines[0]);
	Tween* initTween = new Tween(this->background);
	initTween->animate(TweenableParams::SCALE_X, 0, 1.0, 10, TweenTransitions::EASEINCUBIC);
	initTween->animate(TweenableParams::SCALE_Y, 0, 1.0, 10, TweenTransitions::EASEINCUBIC);
	juggler->add(initTween);
	initTween->addEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
}

void TextBox::closeBox() {
	textLock = true;
	this->nextLine = this->maxLine;

	Tween* closeTween = new Tween(this->background);
	closeTween->animate(TweenableParams::SCALE_X, 1.0, 0, 10, TweenTransitions::EASEINCUBIC);
	closeTween->animate(TweenableParams::SCALE_Y, 1.0, 0, 10, TweenTransitions::EASEINCUBIC);
	juggler->add(closeTween);
	closeTween->addEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
}

void TextBox::drawNextLine() {
	textLock = true;

	Tween* nextTween = new Tween(textLines[nextLine - 1]);
	nextTween->animate(TweenableParams::ALPHA, 255, 0, 10, TweenTransitions::EASEINCUBIC);
	juggler->add(nextTween);
	nextTween->addEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
}

void TextBox::handleEvent(Event* e) {
	if (this->nextLine == this->maxLine) {
		e->getSource()->removeEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
		background->unlinkImmediateChild(textLines[maxLine - 1]);
		this->nextLine = 0;
		textLock = false;
	}
	else if (this->nextLine == 0) {
		e->getSource()->removeEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
		this->nextLine++;
		textLock = false;
	}
	else if (!nextLock) {
		e->getSource()->removeEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
		background->unlinkImmediateChild(textLines[nextLine - 1]);
		textLines[nextLine - 1]->alpha = 255;
		textLines[nextLine]->alpha = 0;
		background->addChild(textLines[nextLine]);
		Tween* nextTween = new Tween(textLines[nextLine]);
		nextTween->animate(TweenableParams::ALPHA, 0, 255, 10, TweenTransitions::EASEINCUBIC);
		juggler->add(nextTween);
		nextLock = true;
		nextTween->addEventListener(this, TweenEvent::TWEEN_COMPLETE_EVENT);
	}
	else {
		this->nextLine++;
		textLock = false;
		nextLock = false;
	}
}