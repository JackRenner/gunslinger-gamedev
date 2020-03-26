#ifndef SCENEINFO_H
#define SCENEINFO_H

#include "Scene.h"

using namespace std;

struct SceneInfo {
	Scene* scenePointer;
	SDL_Point startPos;
	bool saveLast = false;
	SDL_Point offset;
};

#endif