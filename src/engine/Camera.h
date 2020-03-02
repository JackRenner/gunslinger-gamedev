#ifndef CAMERA_H
#define CAMERA_H

#include "SDL2/SDL_image.h"

class Camera{ 

public:
	Camera();
	~Camera();

	int x = 0;
	int y = 0;
	int viewportWidth = 800;
	int viewportHeight = 600;
	double scale = 1.0;
	SDL_Point zoomPoint = { 0, 0 };

private:
	
};

#endif