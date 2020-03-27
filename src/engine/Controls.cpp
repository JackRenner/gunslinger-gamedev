#include "Controls.h"
#include <SDL2/SDL.h>
#include <set>

std::set<SDL_Scancode> controls::pressedKeys;
std::set<SDL_Scancode> controls::lastPressedKeys;

SDL_Scancode controls::KEY_D = SDL_SCANCODE_D;
SDL_Scancode controls::KEY_A = SDL_SCANCODE_A;
SDL_Scancode controls::KEY_W = SDL_SCANCODE_W;
SDL_Scancode controls::KEY_S = SDL_SCANCODE_S;
SDL_Scancode controls::KEY_1 = SDL_SCANCODE_1;
SDL_Scancode controls::KEY_2 = SDL_SCANCODE_2;
SDL_Scancode controls::KEY_3 = SDL_SCANCODE_3;
SDL_Scancode controls::KEY_4 = SDL_SCANCODE_4;
SDL_Scancode controls::KEY_5 = SDL_SCANCODE_5;


SDL_Scancode controls::KEY_RIGHT = SDL_SCANCODE_RIGHT;
SDL_Scancode controls::KEY_LEFT = SDL_SCANCODE_LEFT;
SDL_Scancode controls::KEY_UP = SDL_SCANCODE_UP;
SDL_Scancode controls::KEY_DOWN = SDL_SCANCODE_DOWN;

SDL_Scancode controls::KEY_SHIFT = SDL_SCANCODE_RSHIFT;

SDL_Scancode controls::KEY_ROTATE_C = SDL_SCANCODE_A;
SDL_Scancode controls::KEY_ROTATE_CC = SDL_SCANCODE_S;

SDL_Scancode controls::KEY_SCALE_OUT = SDL_SCANCODE_Q;
SDL_Scancode controls::KEY_SCALE_IN = SDL_SCANCODE_W;

SDL_Scancode controls::KEY_PIVOT_RIGHT = SDL_SCANCODE_L;
SDL_Scancode controls::KEY_PIVOT_LEFT = SDL_SCANCODE_J;
SDL_Scancode controls::KEY_PIVOT_UP = SDL_SCANCODE_I;
SDL_Scancode controls::KEY_PIVOT_DOWN = SDL_SCANCODE_K;

SDL_Scancode controls::KEY_ALPHA_UP = SDL_SCANCODE_Z;
SDL_Scancode controls::KEY_ALPHA_DOWN = SDL_SCANCODE_X;
SDL_Scancode controls::KEY_VISIBILITY = SDL_SCANCODE_P;

// Position
bool controls::holdD(){ return pressedKeys.count(KEY_D) == 1; }
bool controls::holdA(){ return pressedKeys.count(KEY_A) == 1; }
bool controls::holdW(){ return pressedKeys.count(KEY_W) == 1; }
bool controls::holdS(){ return pressedKeys.count(KEY_S) == 1; }

// Direction/Shooting
bool controls::holdRight(){ return pressedKeys.count(KEY_RIGHT) == 1; }
bool controls::holdLeft(){ return pressedKeys.count(KEY_LEFT) == 1; }
bool controls::holdUp(){ return pressedKeys.count(KEY_UP) == 1; }
bool controls::holdDown(){ return pressedKeys.count(KEY_DOWN) == 1; }
bool controls::pressRight(){ return pressedKeys.count(KEY_RIGHT) == 1 && lastPressedKeys.count(KEY_RIGHT) != 1; }
bool controls::pressLeft(){ return pressedKeys.count(KEY_LEFT) == 1 && lastPressedKeys.count(KEY_LEFT) != 1; }
bool controls::pressUp(){ return pressedKeys.count(KEY_UP) == 1 && lastPressedKeys.count(KEY_UP) != 1; }
bool controls::pressDown(){ return pressedKeys.count(KEY_DOWN) == 1 && lastPressedKeys.count(KEY_DOWN) != 1; }

// gun select

bool controls::press1() {return pressedKeys.count(KEY_1) == 1 && lastPressedKeys.count(KEY_1) != 1;}
bool controls::press2() {return pressedKeys.count(KEY_2) == 1 && lastPressedKeys.count(KEY_2) != 1;}
bool controls::press3() {return pressedKeys.count(KEY_3) == 1 && lastPressedKeys.count(KEY_3) != 1;}
bool controls::press4() {return pressedKeys.count(KEY_4) == 1 && lastPressedKeys.count(KEY_4) != 1;}
bool controls::press5() {return pressedKeys.count(KEY_5) == 1 && lastPressedKeys.count(KEY_5) != 1;}

// Rolling
bool controls::pressShift(){ return pressedKeys.count(KEY_SHIFT) == 1 && lastPressedKeys.count(KEY_SHIFT) != 1; }

// Rotation
bool controls::rotateClockwise(){ return pressedKeys.count(KEY_ROTATE_C) == 1; }
bool controls::rotateCounterClockwise(){ return pressedKeys.count(KEY_ROTATE_CC) == 1; }

// Scaling
bool controls::scaleOut(){ return pressedKeys.count(KEY_SCALE_OUT) == 1; }
bool controls::scaleIn(){ return pressedKeys.count(KEY_SCALE_IN) == 1; }

// Pivot
bool controls::pivotRight(){ return pressedKeys.count(KEY_PIVOT_RIGHT) == 1; }
bool controls::pivotLeft(){ return pressedKeys.count(KEY_PIVOT_LEFT) == 1; }
bool controls::pivotUp(){ return pressedKeys.count(KEY_PIVOT_UP) == 1; }
bool controls::pivotDown(){ return pressedKeys.count(KEY_PIVOT_DOWN) == 1; }

// Alpha and Visibility
bool controls::alphaUp(){ return pressedKeys.count(KEY_ALPHA_UP) == 1; }
bool controls::alphaDown(){ return pressedKeys.count(KEY_ALPHA_DOWN) == 1; }
bool controls::toggleVisibility(){ return pressedKeys.count(KEY_VISIBILITY) == 1 && lastPressedKeys.count(KEY_VISIBILITY) != 1; }

// // Controller Button inputs
// bool controls::pressA(){ return pressedButtons.count(BUTTON_A) == 1 && lastPressedButtons.count(BUTTON_A) != 1; }
// bool controls::pressB(){ return pressedButtons.count(BUTTON_B) == 1 && lastPressedButtons.count(BUTTON_B) != 1; }
// bool controls::pressX(){ return pressedButtons.count(BUTTON_X) == 1 && lastPressedButtons.count(BUTTON_X) != 1; }
// bool controls::pressY(){ return pressedButtons.count(BUTTON_Y) == 1 && lastPressedButtons.count(BUTTON_Y) != 1; }

// bool controls::holdA(){ return pressedButtons.count(BUTTON_A) == 1; }
// bool controls::holdB(){ return pressedButtons.count(BUTTON_B) == 1; }
// bool controls::holdX(){ return pressedButtons.count(BUTTON_X) == 1; }
// bool controls::holdY(){ return pressedButtons.count(BUTTON_Y) == 1; }

// // Controller Axis Inputs

// bool controls::holdLT(){ return pressedAxis.count(LEFT_TRIGGER) == 1; }

void controls::update(std::set<SDL_Scancode> pressedKeys){
	// controls::lastPressedAxis = controls::pressedAxis;
	controls::lastPressedKeys = controls::pressedKeys;
	// controls::lastPressedButtons = controls::pressedButtons;
	// controls::pressedButtons = pressedButtons;
	controls::pressedKeys = pressedKeys;
	// controls::pressedAxis = pressedAxis;
}