#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "DisplayObject.h"
#include "DisplayObjectContainer.h"
#include "Sprite.h"
#include "./events/EventDispatcher.h"
#include "./events/Event.h"
#include "map"
#include "vector"
#include "set"
#include "utility"
#include "./events/GameTreeEvent.h"
#include "string"
#include "iostream"
#include <climits>
using namespace std;

class CollisionSystem : public EventListener{

public:

	CollisionSystem();
	~CollisionSystem();

	//checks collisions between pairs of DOs where the corresponding types have been requested
	//to be checked (via a single call to watchForCollisions) below.
	void update();



	//This system watches the game's display tree and is notified whenever a display object is placed onto
	//or taken off of the tree. Thus, the collision system always knows what DOs are in the game at any moment automatically.
	void handleEvent(Event* e);
	//THis  works with DisplayObject Container. We need ot modify the add and remove child
	//Add a line that has calls dispatchEvent.
	//
	//Add this to Game.addEventListener within the Game file.

	//So I need to call notifySubtree event all the way up the ladder
//

//Event categories. THis



	//This function asks the collision system to start checking for collisions between all pairs
	//of DOs of a given type (e.g., player vs platform). The system will begin to check all player objects
	//against all platform objects that are in the current scene.
	void watchForCollisions(string type1, string type2);


	//returns true iff obj1 hitbox and obj2 hitbox overlap. Uses the following method from DO:
	//	SDL_Point* DisplayObject::getGlobalHitbox();
	bool collidesWith(DisplayObject* obj1, DisplayObject* obj2);

	//Resolves the collision that occurred between d and other
	//xDelta1 and yDelta1 are the amount d moved before causing the collision.
	//xDelta2 and yDelta2 are the amount other moved before causing the collision.
	void resolveCollision(DisplayObject* d, DisplayObject* other, int xDelta1, int yDelta1, int xDelta2, int yDelta2);
	//Move these the difference between the two.




	void handleAddDisplayObject(GameTreeEvent* e);
	void handleRemoveDisplayObject(GameTreeEvent* e);
	
	double distance(SDL_Point &p1, SDL_Point &p2);
	double HeronFormulaOfTriangle(SDL_Point &p1, SDL_Point &p2, SDL_Point &p3);
	double areaOfFourTriangles(SDL_Point &p1, SDL_Point &p2, SDL_Point &p3, SDL_Point &p4, SDL_Point &p5);




	//DisplayObjectContainer* ObjectTree;
	std:: map<string, vector<DisplayObject*>*> ObjectsOfEachType;

	std:: map< pair <string, string>, int > collisionTypeMap; //Eventually, replace objectsCollide with this to optimize
	//But for the time being, I think that it may be a good idea to set it up this way.

	vector< pair<string, string> > objectsCollide;

	//vector<string> CollisionIds;
	bool IntersectChecker(SDL_Point* obj1, SDL_Point* obj2);
	bool twoLineChecker(SDL_Point o1p1, SDL_Point o1p2, SDL_Point o2p1, SDL_Point o2p2 );
	int orientationChecker(SDL_Point p1, SDL_Point p2, SDL_Point p3);

	void setSpecialCollisionType(string type1, string type2, int);
	void binaryXChecker(DisplayObject* d, DisplayObject* other, int xDelta1, int xDelta2 );
	void binaryYChecker(DisplayObject* d, DisplayObject* other, int yDelta1, int yDelta2);

//In the player Object Class onCollision


private:


};

#endif
