#include "EventDispatcher.h"
#include "EventListener.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

EventDispatcher::EventDispatcher(){
	listeners = new unordered_map<string, vector<EventListener*>*>();
}

EventDispatcher::~EventDispatcher(){
	delete listeners;
}

void EventDispatcher::addEventListener(EventListener* l, string eventType){
	unordered_map<string, vector<EventListener*>*>::const_iterator find(listeners->find(eventType));
	if(find == listeners->end()){
		vector<EventListener*>* list = new vector<EventListener*>();
		list->push_back(l);
		pair<string, vector<EventListener*>*> myPair;
		myPair = make_pair(eventType, list);
		listeners->insert(myPair);
	} else {
		listeners->at(eventType)->push_back(l);
	}
}

void EventDispatcher::removeEventListener(EventListener* l, string eventType){
	unordered_map<string, vector<EventListener*>*>::const_iterator find(listeners->find(eventType));
	if(find != listeners->end()){
		vector<EventListener*>* vec = listeners->at(eventType);
		for(int i = 0; i < vec->size(); i++){
			if(vec->at(i) == l){
				vec->erase(vec->begin() + i);
			}
		}
	}
}

bool EventDispatcher::hasEventListener(EventListener* l, string eventType){
	unordered_map<string, vector<EventListener*>*>::const_iterator find(listeners->find(eventType));
	if(find != listeners->end()){
		vector<EventListener*>* vec = listeners->at(eventType);
		for(int i = 0; i < vec->size(); i++){
			if(vec->at(i) == l){
				return true;
			}
		}
	}
	return false;
}

void EventDispatcher::dispatchEvent(Event *e){
	unordered_map<string, vector<EventListener*>*>::const_iterator find(listeners->find(e->getType()));
	if(find != listeners->end()){
		vector<EventListener*>* vec = listeners->at(e->getType());
		for(int i = 0; i < vec->size(); i++){
			vec->at(i)->handleEvent(e);
		}
	}
}
