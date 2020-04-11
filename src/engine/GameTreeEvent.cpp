#include "GameTreeEvent.h"

using namespace std;

GameTreeEvent :: GameTreeEvent(string type, EventDispatcher* source, DisplayObject* modifiedObject ) : Event(type, source){
  this->modifiedObject = modifiedObject;
}
DisplayObject* GameTreeEvent:: getModifiedObject(){
  return this->modifiedObject;
}
