#include "Sound.h"
#include <SDL2/SDL_mixer.h>

using namespace std;


Sound::Sound(std::string filepath) {
    SDL_LoadWAV(filepath.c_str(), &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
}

Sound::~Sound(){
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Sound::playSFX(){
    SDL_ClearQueuedAudio(deviceId);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}
