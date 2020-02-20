#include "Sound.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

Sound::Sound(){
    //SDL_LoadWAV("Gun+Shot2.wav", &wavSpec, &wavBuffer, &wavLength);
    //deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
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