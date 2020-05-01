#include "Sound.h"
#include <SDL2/SDL_mixer.h>

using namespace std;


Sound::Sound(std::string filepath) {
	sound = Mix_LoadWAV(filepath.c_str());
}

Sound::~Sound() {
	Mix_FreeChunk(sound);
}

void Sound::playSFX() {
	Mix_PlayChannel(-1, sound, 0);
}