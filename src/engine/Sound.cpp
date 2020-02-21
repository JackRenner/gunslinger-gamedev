#include "Sound.h"
#include <SDL2/SDL_mixer.h>

using namespace std;

// Sound::Sound(string filepath, bool isMusic){
//     if(isMusic)
//         music = Mix_LoadMUS(filepath.c_str());
// }   

Sound::Sound(){
    music = Mix_LoadMUS("./resources/music/MoodyLoop");
}

Sound::~Sound(){
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Sound::playSFX(){
    SDL_LoadWAV("./resources/sfx/Gun+Shot2.wav", &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}

void Sound::playMusic(){
    Mix_PlayMusic(music, -1);
}