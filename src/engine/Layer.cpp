#include "Layer.h"

using namespace std;

Layer::Layer() : DisplayObjectContainer() {
	this->type = "Layer";
}

void Layer::draw(AffineTransform &at) {
	applyTransformations(at);

	at.translate(pivot.x, pivot.y);


	// undo global camera modifiers
	at.translate(gameCamera.zoomPoint.x, gameCamera.zoomPoint.y);
	at.scale(1 / gameCamera.scale, 1 / gameCamera.scale);
	at.translate(gameCamera.x - gameCamera.zoomPoint.x, gameCamera.y - gameCamera.zoomPoint.y);

	// do layer specific modifiers
	at.translate(-scrollSpeedX * gameCamera.x + gameCamera.zoomPoint.x, -scrollSpeedY * gameCamera.y + gameCamera.zoomPoint.y);
	at.scale(gameCamera.scale, gameCamera.scale);
	at.translate(-gameCamera.zoomPoint.x, -gameCamera.zoomPoint.y);

	for (int i = 0; i < children.size(); i++) {
		children[i]->draw(at);
	}

	// undo layer specific modifiers
	at.translate(gameCamera.zoomPoint.x, gameCamera.zoomPoint.y);
	at.scale(1 / gameCamera.scale, 1 / gameCamera.scale);
	at.translate(scrollSpeedX * gameCamera.x - gameCamera.zoomPoint.x, scrollSpeedY * gameCamera.y - gameCamera.zoomPoint.y);

	// redo global camera modifiers
	at.translate(-gameCamera.x + gameCamera.zoomPoint.x, -gameCamera.y + gameCamera.zoomPoint.y);
	at.scale(gameCamera.scale, gameCamera.scale);
	at.translate(-gameCamera.zoomPoint.x, -gameCamera.zoomPoint.y);

	// redo the parent's pivot
	at.translate(-pivot.x, -pivot.y);

	reverseTransformations(at);
}