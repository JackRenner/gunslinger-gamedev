#ifndef TWEEN_H
#define TWEEN_H

#include "EventDispatcher.h"
#include "DisplayObject.h"
#include <vector>
#include <string>

enum TweenableParams {ALPHA, SCALE_X, SCALE_Y, POS_X, POS_Y, ROTATION};
enum TweenTransitions {NONE,EASEINCUBIC,EASEOUTCUBIC,EASEINOUTCUBIC};

struct ParamInfo{
    TweenableParams field;
    double startVal;
    double endVal;
    double frames;
    TweenTransitions transition;
    ParamInfo(){};
    ParamInfo(TweenableParams p,double start,double end,double num,TweenTransitions t){
        field=p;
        startVal=start;
        endVal=end;
        frames=num;
        transition=t;
    }
};

class Tween : public EventDispatcher { 

public:
    Tween();
	Tween(DisplayObject *object);
	~Tween();

    void animate(TweenableParams fieldToAnimate, double startVal, double endVal, double frames, TweenTransitions transition);
    void update();
    bool isComplete();
    void setValue(TweenableParams param, double value);

    // std::unordered_map<TweenableParams, ParamInfo> params;
    vector<ParamInfo> params;

    DisplayObject *tweenObject;

private:

	
};

#endif