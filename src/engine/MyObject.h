#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "Sprite.h"

using namespace std;

class MyObject : public Sprite{

public:

	MyObject();
	MyObject(string id, string filepath);
	MyObject(string id, int red, int green, int blue);

//	virtual void savePosition();

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);
	virtual void onCollision(DisplayObject * otherObject);

private:
//	saving the AffineTransform is the full way to do this.
};

#endif
