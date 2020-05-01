#include "Music.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

Music::Music(string filepath){
    if (music != NULL)
        return;

    this->music = Mix_LoadMUS(filepath.c_str());
}

Music::~Music(){
    Mix_FreeMusic(music);
}

void Music::play(){
    Mix_VolumeMusic(64);
    Mix_PlayMusic(music, -1);
}

