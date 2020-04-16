#include "DisplayObjectContainer.h"
#include "AffineTransform.h"
#include <vector>
#include <string>
#include "Controls.h"
#include <iostream>

using namespace std;


DisplayObjectContainer::DisplayObjectContainer() : DisplayObject() {
    this->type = "DisplayObjectContainer";
    //this->MyEventDispatcher = new EventDispatcher();
}

DisplayObjectContainer::DisplayObjectContainer(string id, string filepath) : DisplayObject(id, filepath) {
    this->type = "DisplayObjectContainer";
    //this->MyEventDispatcher = new EventDispatcher();
}

DisplayObjectContainer::DisplayObjectContainer(string id, int red, int green, int blue) : DisplayObject(id, red, green, blue) {
    this->type = "DisplayObjectContainer";
    //this->MyEventDispatcher = new EventDispatcher();
}

DisplayObjectContainer::~DisplayObjectContainer() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

void DisplayObjectContainer::addChild(DisplayObject* child) {
    children.push_back(child);
    child->parent = this; // make sure to include reverse reference also
    //Add the event code to add a child here.
    //setChildEventDispatcher(child);
    callAddChildEvent(child);

}

void DisplayObjectContainer::removeImmediateChild(DisplayObject* child) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            callRemoveChildEvent(child);
            delete child;
            children.erase(children.begin() + i);
        }
    }
}

void DisplayObjectContainer::removeImmediateChild(string id) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->id == id) {
            // delete the child
            callRemoveChildEvent(children[i]);
            delete children[i];
            children.erase(children.begin() + i);
        }
    }
}

void DisplayObjectContainer::unlinkImmediateChild(DisplayObject* child) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            child->parent = NULL;
            children.erase(children.begin() + i);
        }
    }
}

void DisplayObjectContainer::unlinkImmediateChild(string id) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->id == id) {
            callRemoveChildEvent(children[i]);
            children[i]->parent = NULL;
            children.erase(children.begin() + i);

        }
    }
}

void DisplayObjectContainer::removeChild(int index) {
    if (index < children.size()) {
        //Call the remove EventFunction here, before its deleted.
        callRemoveChildEvent(children[index]);
        delete children[index];
        children.erase(children.begin() + index);
    }
}

void DisplayObjectContainer::unlinkChild(int index) {
    if (index < children.size()) {
        children[index]->parent = NULL;
        children.erase(children.begin() + index);
    }
}

void DisplayObjectContainer::removeThis() {
    if (this->parent != NULL) {
      //Call Delete Event here
        //Remove this from its Parents
        ((DisplayObjectContainer*)this->parent)->callRemoveChildEvent(this);
        ((DisplayObjectContainer*)this->parent)->removeImmediateChild(this);
    }
}

void DisplayObjectContainer::unlinkFromParent() {
    if (this->parent != NULL) {
        ((DisplayObjectContainer*)this->parent)->unlinkImmediateChild(this);
    }
}

int DisplayObjectContainer::numChildren() {
    return this->children.size();
}

DisplayObject* DisplayObjectContainer::getChild(int index) {
    if (index < 0 || index > numChildren()) {
        return NULL;
    }
    else return children[index];
}

DisplayObject* DisplayObjectContainer::getChild(string id) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->id == id) {
            return children[i];
        }
        // check recursively?
    }
    return NULL;
}

void DisplayObjectContainer::update(set<SDL_Scancode> pressedKeys) {
    DisplayObject::update(pressedKeys);
    for (int i = 0; i < children.size(); i++) {
        children[i]->update(pressedKeys);
    }
}



void DisplayObjectContainer::draw(AffineTransform& at) {
    DisplayObject::draw(at);
    applyTransformations(at);
    // undo the parent's pivot
    at.translate(pivot.x, pivot.y);
    for (int i = 0; i < children.size(); i++) {
        children[i]->draw(at);
    }
    // redo the parent's pivot
    at.translate(-pivot.x, -pivot.y);
    reverseTransformations(at);
}

//Here for utility if need be. But ideally should not be called.
void DisplayObjectContainer:: swapEventDispatcher(EventDispatcher* mainEventDispatcher){

    if(this->MyEventDispatcher != NULL){
      cout << "EventDispatcherIsNotNull" <<endl;
      free(this->MyEventDispatcher);
    }

    MyEventDispatcher = mainEventDispatcher;

}

void DisplayObjectContainer:: addParentEventDispatcher(){
  this->MyEventDispatcher = this->parent->MyEventDispatcher;
}
void DisplayObjectContainer :: setChildEventDispatcher(DisplayObject* myChild){
  myChild->MyEventDispatcher = this->MyEventDispatcher;
}

void DisplayObjectContainer :: callAddChildEvent(DisplayObject* ChildToAdd){
  if(this->parent != NULL){
    //Assumes the parent is a displayobjectcontainer or a subvariant of one.
    DisplayObjectContainer* temp = (DisplayObjectContainer*) this->parent;
     temp->callAddChildEvent(ChildToAdd);
  }else{
    GameTreeEvent* addEvent;
    addEvent = new GameTreeEvent("addObject", this->MyEventDispatcher, ChildToAdd);
    this->MyEventDispatcher->dispatchEvent(addEvent);
    free(addEvent);
  }
}


void DisplayObjectContainer :: callRemoveChildEvent(DisplayObject* ChildToRemove){
  if(this->parent != NULL){
    //Assumes the parent is a displayobjectcontainer or a subvariant of one.
    DisplayObjectContainer* temp = (DisplayObjectContainer*) this->parent;
    temp->callRemoveChildEvent(ChildToRemove);
  }else{
    GameTreeEvent* removeEvent;
    removeEvent = new GameTreeEvent("removeObject", this->MyEventDispatcher, ChildToRemove);
    this->MyEventDispatcher->dispatchEvent(removeEvent);
    free(removeEvent);
  }
}

 void DisplayObjectContainer :: saveAllPositions(){
  DisplayObject::savePosition();
  for (int i = 0; i < children.size(); i++) {
      children[i]-> saveAllPositions();
  }
}
