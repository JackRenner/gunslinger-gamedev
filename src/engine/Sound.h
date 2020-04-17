#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

class Sound{ 

public:
	Sound();
	// Sound(string filepath, bool isMusic);
	~Sound();

	void playSFX();
	void playMusic(std::string area);

	//std::string cur_music = "town";

private:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
	SDL_AudioDeviceID deviceId;
	Mix_Music * town_music;
	Mix_Music * lakestill_music;
};

#endif