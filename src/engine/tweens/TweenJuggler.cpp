#include "TweenJuggler.h"

TweenJuggler* TweenJuggler::instance = NULL;

TweenJuggler* TweenJuggler::getInstance() {
    if(instance==NULL){
        instance = new TweenJuggler();
    }
    return instance;
}

void TweenJuggler::add(Tween * tween) {
    tweens.push_back(tween);
}

void TweenJuggler::nextFrame() {
    for (list<Tween*>::iterator it = tweens.begin(); it != tweens.end(); ++it) {
        (*it)->update();

        if ((*it)->isComplete()) {
            delete (*it);
            it = tweens.erase(it);
            if (it == tweens.end())
                break;
        }
    }
}
