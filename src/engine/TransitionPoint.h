#ifndef TRANSITIONPOINT_H
#define TRANSITIONPOINT_H

#include <SDL2/SDL.h>

using namespace std;

struct TransitionPoint {
	SDL_Point point;
	int newstate;

	TransitionPoint(SDL_Point point, int newstate) {
		this->point = point;
		this->newstate = newstate;
	}
};

#endif