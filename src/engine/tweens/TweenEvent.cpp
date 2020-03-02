#include "TweenEvent.h"

using namespace std;

const string TweenEvent::TWEEN_START_EVENT = "tween_start";
const string TweenEvent::TWEEN_PARTIAL_EVENT = "tween_partial";
const string TweenEvent::TWEEN_COMPLETE_EVENT = "tween_complete";

TweenEvent::TweenEvent(string type, Tween* tween) : Event(type, tween) {

}

Tween* TweenEvent::getTween() {
	return this->source;
}