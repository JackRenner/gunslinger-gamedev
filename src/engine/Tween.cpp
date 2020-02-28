#include "Tween.h"
#include <iostream>
#include <cmath>

using namespace std;

Tween::Tween(){

}

Tween::Tween(DisplayObject *object){
    tweenObject = object;
}

Tween::~Tween(){

}

void Tween::animate(TweenableParams fieldToAnimate, double startVal, double endVal, double frames, TweenTransitions transition){
    params.push_back(new ParamInfo(fieldToAnimate,startVal,endVal,frames,transition));
}

void Tween::update(){
    cout << "UPDATE" << endl;
    for(ParamInfo *param : params){
        // if (abs(param->endVal-param->curVal) < (param->endVal-param->startVal)/param->frames ){
        //     cout << "DONE!" << endl;
        //     continue;
        // }
        if (param->endVal > param->startVal && param->curVal >= param->endVal){
            cout << "DONE!" << endl;
            continue;
        }
        else if (param->endVal < param->startVal && param->curVal <= param->endVal){
            cout << "DONE!" << endl;
            continue;
        }

        cout << "Frame: " << param->curFrame << " Value: " << param->curVal << endl;

        switch(param->transition){
        case LINEAR:
            param->curVal += (param->endVal-param->startVal)/param->frames;
            break;
        case EASEINCUBIC:
            param->curVal = (param->endVal-param->startVal) * (param->curFrame / param->frames) * (param->curFrame / param->frames) * (param->curFrame / param->frames) + param->startVal;
            break;
        case EASEOUTCUBIC:
            param->curVal = (param->endVal-param->startVal) * ((param->curFrame / param->frames-1) * (param->curFrame / param->frames-1) * (param->curFrame / param->frames-1) + 1) + param->startVal;
            break;
        }
        param->curFrame++;
        setValue(param->field, param->curVal);
    }
}

bool Tween::isComplete(){

}

void Tween::setValue(TweenableParams param, double value){
    switch (param) { 
    case ALPHA: 
        tweenObject->alpha = value;
        break; 
    case SCALE_X: 
        tweenObject->scaleX = value;
        break; 
    case SCALE_Y: 
        tweenObject->scaleY = value;
        break; 
    case POS_X: 
        tweenObject->position.x = value;
        break; 
    case POS_Y: 
        tweenObject->position.y = value;
        break;
    case ROTATION: 
        tweenObject->rotation = value;
        break;  
    } 
}


// double easeInCubic(double curFrame, double startVal, double endVal, double frames) {
//     double range = endVal - startVal;
//     return range * (curFrame /= frames) * curFrame * curFrame + startVal;
// }

// double easeOutCubic(double curFrame, double startVal, double endVal, double frames) {
//     double range = endVal - startVal;
//     return range * ((curFrame = curFrame / frames-1) * curFrame * curFrame + 1) + startVal;
// }
// double easeInOutCubic(double curFrame, double startVal, double endVal, double frames) {
//     double range = endVal - startVal;
//     if ((curFrame /= frames / 2) < 1) {
//       return endVal / 2 * curFrame * curFrame * curFrame + startVal;
//     } else {
//       return endVal / 2 * ((curFrame -= 2) * curFrame * curFrame + 2) + startVal;
//     }
// }