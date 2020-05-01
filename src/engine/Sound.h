#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>


class Sound {
public:
	Sound(std::string filepath);
	~Sound();

	void playSFX();

private:
	Mix_Chunk* sound = NULL;
};

#endif