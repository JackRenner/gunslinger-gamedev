#ifndef DISPLAYOBJECTCONTAINER_H
#define DISPLAYOBJECTCONTAINER_H

#include <vector>
#include "DisplayObject.h"
#include "AffineTransform.h"
//#include "Event.h"
#include "EventDispatcher.h"
#include <string>
#include <fstream>


using namespace std;

class DisplayObjectContainer : public DisplayObject {

public:

	DisplayObjectContainer();
	DisplayObjectContainer(string id, string filepath);
	DisplayObjectContainer(string id, int red, int green, int blue);
	virtual ~DisplayObjectContainer();

	void addChild(DisplayObject* child);
	void removeImmediateChild(DisplayObject* child);
	void removeImmediateChild(string id);
	void unlinkImmediateChild(DisplayObject* child);
	void unlinkImmediateChild(string id);
	void removeChild(int index);
	void unlinkChild(int index);
	void removeThis();
	void unlinkFromParent();

	virtual int numChildren();
	virtual DisplayObject* getChild(int index);
	virtual DisplayObject* getChild(string id);

	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform& at);

	vector<DisplayObject*> children;

	EventDispatcher* MyEventDispatcher; //Create an EventDispatcher for DOC when built.
	//Then we can call it for all

//	void addEventListener(CollisionSystem &ourCollisionSystem, string eventType);


	//Call an Event on add and remove children
	//notify subtree Change
	//this->dispatchEvents
	// if the the parent is not null, it dispatches the event for the parent.

	//The event called has three parameters, the event Name, "This" or the current object,
	// and the child that was added or removed.
	// So we need to create another class and make it a child of the event Class
	// It will contain an objects collide.
private:


};

#endif
