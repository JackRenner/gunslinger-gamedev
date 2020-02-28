#include "TweenJuggler.h"

TweenJuggler::TweenJuggler(){

}



void TweenJuggler::add(Tween * tween){
    tweens.push_back(tween);
}

void TweenJuggler::nextFrame(){
    for(Tween* tween : tweens){
        tween->update();
    }
}

TweenJuggler* TweenJuggler::getInstance(){
    if (instance==NULL){
        instance = new TweenJuggler;
    }
    return instance;
}