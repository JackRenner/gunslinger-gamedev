#ifndef TWEENJUGGLER_H
#define TWEENJUGGLER_H

#include "Tween.h"
#include <vector>

class TweenJuggler { 

public:

    void add(Tween *tween);
    void nextFrame();
    static TweenJuggler *getInstance();

private:
    TweenJuggler();
    static TweenJuggler *instance;
    vector<Tween*> tweens;
	
};

#endif