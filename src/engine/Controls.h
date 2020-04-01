#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include <set>

namespace controls{

	/* Pointer to the pressed keys. Needs to be updated every frames */
	extern std::set<SDL_Scancode> pressedKeys;
	extern std::set<SDL_Scancode> lastPressedKeys;

	extern SDL_Scancode KEY_D;
	extern SDL_Scancode KEY_A;
	extern SDL_Scancode KEY_W;
	extern SDL_Scancode KEY_S;
	extern SDL_Scancode KEY_1;
	extern SDL_Scancode KEY_2;
	extern SDL_Scancode KEY_3;
	extern SDL_Scancode KEY_4;
	extern SDL_Scancode KEY_5;

	extern SDL_Scancode KEY_RIGHT;
	extern SDL_Scancode KEY_LEFT;
	extern SDL_Scancode KEY_UP;
	extern SDL_Scancode KEY_DOWN;
	extern SDL_Scancode KEY_R;

	extern SDL_Scancode KEY_SHIFT;

	extern SDL_Scancode KEY_SPACE;
	
	extern SDL_Scancode KEY_ROTATE_C;
	extern SDL_Scancode KEY_ROTATE_CC;
	
	extern SDL_Scancode KEY_SCALE_OUT;
	extern SDL_Scancode KEY_SCALE_IN;

	extern SDL_Scancode KEY_PIVOT_RIGHT;
	extern SDL_Scancode KEY_PIVOT_LEFT;
	extern SDL_Scancode KEY_PIVOT_UP;
	extern SDL_Scancode KEY_PIVOT_DOWN;

	extern SDL_Scancode KEY_ALPHA_UP;
	extern SDL_Scancode KEY_ALPHA_DOWN;
	extern SDL_Scancode KEY_VISIBILITY;


	// Position controls
	bool holdD();
	bool holdA();
	bool holdW();
	bool holdS();

	// Direction/Shooting controls
	bool holdRight();
	bool holdLeft();
	bool holdUp();
	bool holdDown();
	bool pressRight();
	bool pressLeft();
	bool pressUp();
	bool pressDown();
	bool pressR();

	//gun select
	bool press1();
	bool press2();
	bool press3();
	bool press4();
	bool press5();

	// Rolling
	bool pressShift();

	// Rotation controls
	bool rotateClockwise();
	bool rotateCounterClockwise();

	// Scaling controls
	bool scaleOut();
	bool scaleIn();

	// Pivot controls
	bool pivotLeft();
	bool pivotRight();
	bool pivotUp();
	bool pivotDown();

	// Alpha and Visibility
	bool alphaUp();
	bool alphaDown();
	bool toggleVisibility();

	// Controller Button Inputs
	bool pressA();
	bool pressB();
	bool pressX();
	bool pressY();

	bool holdA();
	bool holdB();
	bool holdX();
	bool holdY();

	// Controller Axis Inputs
	bool holdLT();
	
	void update(std::set<SDL_Scancode> pressedKeys);

}

#endif