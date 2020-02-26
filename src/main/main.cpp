#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <DisplayObject.h>
#include <iostream>
#include "DevTool.h"

using namespace std;

int main(int argc, char ** argv)
{
	DevTool* myDevTool = new DevTool();
	myDevTool->start();

	delete myDevTool;
	return 0;
}