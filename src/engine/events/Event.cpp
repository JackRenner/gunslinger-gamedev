#include "Event.h"
#include "EventDispatcher.h"

Event::Event(string type, EventDispatcher* source){
	this->eventType = type;
	this->source = source;
}

string Event::getType(){
	return this->eventType;
}

EventDispatcher* Event::getSource(){
	return this->source;
}