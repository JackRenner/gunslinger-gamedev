#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

using namespace std;

class Music{ 

public:
	Music(string filepath);
	~Music();

	void play();

private:
	Mix_Music * music = NULL;
};

#endif