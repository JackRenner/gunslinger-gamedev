#ifndef SCENEINFO_H
#define SCENEINFO_H

#include "Scene.h"

using namespace std;

struct SceneInfo {
	Scene* scenePointer;
	SDL_Point startPos;
	bool saveLast = false;
	SDL_Point offset;

	SceneInfo(Scene* scenePointer, SDL_Point startPos, bool saveLast, SDL_Point offset) {
		this->scenePointer = scenePointer;
		this->startPos = startPos;
		this->saveLast = saveLast;
		this->offset = offset;
	}
};

#endif