#include "Tween.h"
#include <iostream>
#include <cmath>

using namespace std;

Tween::Tween(DisplayObject *object) {
    tweenObject = object;
    startTweenEvent = new TweenEvent(TweenEvent::TWEEN_START_EVENT, this);
    partialTweenEvent = new TweenEvent(TweenEvent::TWEEN_PARTIAL_EVENT, this);
    completeTweenEvent = new TweenEvent(TweenEvent::TWEEN_COMPLETE_EVENT, this);
}

Tween::~Tween() {
    for (int i = 0; i < this->params.size(); i++) {
        delete params[i];
    }

    delete startTweenEvent;
    delete partialTweenEvent;
    delete completeTweenEvent;
}

void Tween::animate(TweenableParams fieldToAnimate, double startVal, double endVal, double frames, TweenTransitions transition) {
    params.push_back(new ParamInfo(fieldToAnimate, startVal, endVal, frames, transition));
    if (frames > this->frames)
        this->frames = frames;
}

void Tween::update() {
    for (int i = 0; i < params.size(); i++) {
        ParamInfo* param = params[i];

        if (param->endVal > param->startVal&& param->curVal >= param->endVal) {
            // This Param is done
            continue;
        }
        else if (param->endVal < param->startVal && param->curVal <= param->endVal) {
            // This Param is done
            continue;
        }
        
        switch(param->transition) {
        case TweenTransitions::LINEAR:
            param->curVal += (param->endVal - param->startVal) / param->frames;
            break;
        case TweenTransitions::EASEINCUBIC:
            param->curVal = (param->endVal - param->startVal) * (param->curFrame / param->frames) * (param->curFrame / param->frames) * (param->curFrame / param->frames) + param->startVal;
            break;
        case TweenTransitions::EASEOUTCUBIC:
            param->curVal = (param->endVal - param->startVal) * ((param->curFrame / param->frames - 1) * (param->curFrame / param->frames - 1) * (param->curFrame / param->frames - 1) + 1) + param->startVal;
            break;
        }
        param->curFrame++;
        setValue(param->field, param->curVal);
    }

    this->curFrame++;

    if (this->curFrame == 1)
        this->dispatchEvent(startTweenEvent);
    else if (this->curFrame == this->frames) {
        this->dispatchEvent(completeTweenEvent);
        this->complete = true;
    }
    else
        this->dispatchEvent(partialTweenEvent);
}

bool Tween::isComplete() {
    return complete;
}

// Returns percentage done relative to longest animation in Tween
double Tween::getPercentDone() {
    return curFrame / frames;
}

void Tween::setValue(TweenableParams param, double value){
    switch (param) { 
    case TweenableParams::ALPHA: 
        tweenObject->alpha = value;
        break; 
    case TweenableParams::SCALE_X:
        tweenObject->scaleX = value;
        break; 
    case TweenableParams::SCALE_Y:
        tweenObject->scaleY = value;
        break; 
    case TweenableParams::POS_X:
        tweenObject->position.x = value;
        break; 
    case TweenableParams::POS_Y:
        tweenObject->position.y = value;
        break;
    case TweenableParams::ROTATION:
        tweenObject->rotation = value;
        break;  
    } 
}