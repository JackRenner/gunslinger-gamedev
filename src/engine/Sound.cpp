#include "Sound.h"
#include <SDL2/SDL_mixer.h>

using namespace std;

// Sound::Sound(string filepath, bool isMusic){
//     if(isMusic)
//         music = Mix_LoadMUS(filepath.c_str());
// }   

Sound::Sound(){
    town_music = Mix_LoadMUS("./resources/music/MoodyLoop.wav");
    lakestill_music = Mix_LoadMUS("./resources/music/lakestill_music.mp3");
    //this->cur_music = "town";
}

Sound::~Sound(){
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Sound::playSFX(){
    SDL_LoadWAV("./resources/sfx/DeathFlash.wav", &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}

void Sound::playMusic(std::string area){
    if (area == "town") {
        Mix_PlayMusic(town_music, -1);
        //this->cur_music = "town";
    }
    if (area == "lakestill") {
        Mix_PlayMusic(lakestill_music, -1);
        //this->cur_music = "lakestill";
    }
}
