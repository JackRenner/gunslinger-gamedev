#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_audio.h>
#include <iostream>

class Sound{ 

public:
	Sound();
	~Sound();

	void playSFX();
	void playMusic();

private:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
	SDL_AudioDeviceID deviceId;
};

#endif