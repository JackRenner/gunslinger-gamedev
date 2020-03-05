#ifndef TWEEN_H
#define TWEEN_H

#include "../events/EventDispatcher.h"
#include "../tweens/TweenEvent.h"
#include "../DisplayObject.h"
#include <vector>
#include <string>

class TweenEvent;

enum class TweenableParams { ALPHA, SCALE_X, SCALE_Y, POS_X, POS_Y, ROTATION };
enum class TweenTransitions { LINEAR, EASEINCUBIC, EASEOUTCUBIC, EASEINOUTCUBIC };

struct ParamInfo {
    TweenableParams field;
    double startVal = 0.0;
    double curVal = 0.0;
    double endVal = 0.0;
    double curFrame = 0.0;
    double frames = 0.0;
    TweenTransitions transition;
    ParamInfo(TweenableParams p, double start, double end, double num, TweenTransitions t) {
        field = p;
        startVal = start;
        curVal = start;
        endVal = end;
        curFrame = 1;
        frames = num;
        transition = t;
    }
};

class Tween : public EventDispatcher { 

public:
	Tween(DisplayObject *object);
	~Tween();

    void animate(TweenableParams fieldToAnimate, double startVal, double endVal, double frames, TweenTransitions transition);
    void update();
    bool isComplete();
    double getPercentDone();
    void setValue(TweenableParams param, double value);

    vector<ParamInfo*> params;

    DisplayObject *tweenObject = NULL;

    TweenEvent* startTweenEvent;
    TweenEvent* partialTweenEvent;
    TweenEvent* completeTweenEvent;

private:
    // duration of Tween as measured by longest Param animation
    int frames = 0;
    int curFrame = 0;
    bool complete = false;
	
};

#endif