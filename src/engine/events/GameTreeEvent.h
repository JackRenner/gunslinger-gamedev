#ifndef GAMETREEEVENT_H
#define GAMETREEEVENT_H
#include "Event.h"
#include "EventDispatcher.h"
#include <string>
#include "DisplayObject.h"

using namespace std;
class GameTreeEvent : public  Event{
  public:
  DisplayObject* modifiedObject;
  GameTreeEvent(string type, EventDispatcher* source, DisplayObject* modifiedObject);
  DisplayObject* getModifiedObject();
};

#endif
