#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "Sprite.h"
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
	//Added
	string basepath;
	bool fromSheet;
	SDL_Rect** rects;
	string xmlPath;
};

class AnimatedSprite : public Sprite{

public:

	AnimatedSprite();
	AnimatedSprite(string id);
	~AnimatedSprite();

	void addAnimation(string basepath, string animName, int numFrames, int frameRate, bool loop);
	void addAnimation(string spriteSheet, string xml, string animName, int numFrames, int frameRate, bool loop);
	void addAnimation(Animation* anim);

	Animation* getAnimation(string animName);
	Animation* getCopyAnimation(string animName);

	void play(string animName);
	void replay();
	void stop();
	vector<string> parseXML(string spriteSheet, string xml);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);
	std:: vector<string> getAnimationNames();
	std:: string getCurrentAnimationName();




	bool playing = false;

private:
	Animation* current;
	vector<Animation*> animations;
	int frameCount;

};

#endif
