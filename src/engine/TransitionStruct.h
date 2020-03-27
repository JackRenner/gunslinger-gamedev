#ifndef TRANSITIONPOINT_H
#define TRANSITIONPOINT_H

#include <SDL2/SDL.h>

using namespace std;

enum class TransitionDetection { POINT, AXIS };
enum class Cardinal { NORTH, EAST, SOUTH, WEST, NONE };

struct TransitionStruct {
	SDL_Point point;
	SDL_Point dest;
	int newstate;
	TransitionDetection detection;
	Cardinal direction;

	TransitionStruct() {
		this->point = { 0, 0 };
		this->dest = { 0, 0 };
		this->newstate = -1;
		this->detection = TransitionDetection::POINT;
		this->direction = Cardinal::NONE;
	}

	TransitionStruct(SDL_Point point, SDL_Point dest, int newstate) {
		this->point = point;
		this->dest = dest;
		this->newstate = newstate;
		this->detection = TransitionDetection::POINT;
		this->direction = Cardinal::NONE;
	}

	TransitionStruct(SDL_Point point, SDL_Point dest, int newstate, TransitionDetection detection, Cardinal direction) {
		this->point = point;
		this->dest = dest;
		this->newstate = newstate;
		this->detection = detection;
		this->direction = direction;
	}
};

#endif