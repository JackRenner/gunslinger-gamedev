#include "Music.h"
#include <SDL2/SDL_mixer.h>

using namespace std;

Music::Music(string filepath){
    this->music = Mix_LoadMUS(filepath.c_str());
}

Music::~Music(){
}

void Music::play(){
    Mix_PlayMusic(music, -1);
}

