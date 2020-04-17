#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "Sprite.h"
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Controls.h"

using namespace std;

struct Frame {
	SDL_Surface* image;
	SDL_Texture* texture;
};

struct Animation {
	Frame** frames;
	string animName;
	int numFrames;
	int frameRate;
	bool loop;
	int curFrame;
	bool fromSheet;
	SDL_Rect** rects;
	bool whenDoneRemove = false; //removes object when animation plays through
};

class AnimatedSprite : public Sprite{

public:

	AnimatedSprite();
	AnimatedSprite(string id);
	~AnimatedSprite();

	void addAnimation(string basepath, string animName, int numFrames, int frameRate, bool loop);
	void addAnimation(string spriteSheet, string xml, string animName, int numFrames, int frameRate, bool loop);
	Animation* getAnimation(string animName);

	void play(string animName);
	void replay();
	void stop();
	void whenDoneRemove(string animName);
	vector<string> parseXML(string spriteSheet, string xml);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);

	bool playing = false;

private:
	Animation* current;
	vector<Animation*> animations;
	int frameCount;
	bool removed = false; //to prevent seg fault

};

#endif
