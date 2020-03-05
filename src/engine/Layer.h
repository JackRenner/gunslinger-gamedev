#ifndef LAYER_H
#define LAYER_H

#include "DisplayObjectContainer.h"

using namespace std;

class Layer : public DisplayObjectContainer {

public:

	Layer();

	void draw(AffineTransform &at) override;

	double scrollSpeedX = 1.0;
	double scrollSpeedY = 1.0;

private:


};

#endif