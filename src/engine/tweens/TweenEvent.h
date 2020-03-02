#ifndef TWEENEVENT_H
#define TWEENEVENT_H

#include "../events/Event.h"
#include "Tween.h"

using namespace std;

class Tween;

class TweenEvent : public Event {

public:
	TweenEvent(string type, Tween* tween);

	Tween* getTween();

	// currently only have events for overall Tween progress (not individual params)
	static const string TWEEN_START_EVENT;
	static const string TWEEN_PARTIAL_EVENT;
	static const string TWEEN_COMPLETE_EVENT;

private:
	string eventType = "";
	Tween* source = NULL;
};

#endif