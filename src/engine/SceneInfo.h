#ifndef SCENEINFO_H
#define SCENEINFO_H

#include "Scene.h"

using namespace std;

struct Bound {
	SDL_Rect bounds;
	bool check_up = true;
	bool check_down = true;
	bool check_left = true;
	bool check_right = true;

	Bound() {
		this->bounds = SDL_Rect{ 0, 0, 0, 0 };
	}

	Bound(SDL_Rect rect) {
		this->bounds = rect;
	}

	Bound(SDL_Rect bounds, bool up, bool down, bool left, bool right) {
		this->bounds = bounds;
		this->check_up = up;
		this->check_down = down;
		this->check_left = left;
		this->check_right = right;
	};
};

struct SceneInfo {
	Scene* scenePointer = NULL;
	Bound bounds;

	SceneInfo(Scene* scenePointer, SDL_Rect rect) {
		this->scenePointer = scenePointer;
		this->bounds = Bound(rect);

	}

	SceneInfo(Scene* scenePointer, Bound bounds) {
		this->scenePointer = scenePointer;
		this->bounds = bounds;
	}
};

#endif