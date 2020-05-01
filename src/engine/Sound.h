#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

class Sound{ 

public:
	Sound(std::string filepath);
	~Sound();

	void playSFX();

	//std::string cur_music = "town";

private:
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;
	SDL_AudioDeviceID deviceId;
};

#endif