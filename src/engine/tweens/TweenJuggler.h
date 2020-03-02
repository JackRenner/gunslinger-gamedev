#ifndef TWEENJUGGLER_H
#define TWEENJUGGLER_H

#include "Tween.h"
#include <list>

class TweenJuggler { 

public:
    static TweenJuggler *getInstance();
    void add(Tween *tween);
    void nextFrame();

private:
    TweenJuggler() {};
    static TweenJuggler *instance;
    list<Tween*> tweens;
	
};

#endif