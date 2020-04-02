#include "CollisionSystem.h"


CollisionSystem :: CollisionSystem(){
}
CollisionSystem :: ~CollisionSystem(){
}

//checks collisions between pairs of DOs where the corresponding types have been requested
//to be checked (via a single call to watchForCollisions) below.
void CollisionSystem:: update(){
// Take the list of collision types and the display tree and check each object against each other.
// Only check types within the list.
//




}



//This system watches the game's display tree and is notified whenever a display object is placed onto
//or taken off of the tree. Thus, the collision system always knows what DOs are in the game at any moment automatically.
void CollisionSystem :: handleEvent(Event* e){
  //So specify What event the event is, by "e"
  //And then choose how to handle it.
  //
  if(e->getType() == "addObject"){




  }
  if(e->getType() == "removeObject"){


  }


}



//This function asks the collision system to start checking for collisions between all pairs
//of DOs of a given type (e.g., player vs platform). The system will begin to check all player objects
//against all platform objects that are in the current scene.
void CollisionSystem :: watchForCollisions(string type1, string type2){
  bool order1 = false;
  bool order2 = false;
    pair<string, string> Order1 = make_pair (type1, type2);
    pair<string, string> Order2 = make_pair(type2, type1);
  for(pair<string,string> temp : this->objectsCollide){

  }
/*
  pair<string, string> Order1 ( type1, type2);
  pair<string, string> Order2 ( type2, type1);
  if( collisionMap.count(Order1) == 0){
    collisionMap.insert(Order1, true);
  }
  else{
    collisionMap[Order1] = true;
  }
  if(collisionMap.count(Order2) == 0){
    collisionMap.insert(Order2, true);
  }
  else{
    collisionMap[Order2] = true;
  }
  */
}

//returns true iff obj1 hitbox and obj2 hitbox overlap. Uses the following method from DO:
//	SDL_Point* DisplayObject::getGlobalHitbox();
bool CollisionSystem :: collidesWith(DisplayObject* obj1, DisplayObject* obj2){


/*
    AffineTransform* obj1Affine = obj1->getGlobalTransform();
    AffineTransform* obj2Affine = obj2->getGlobalTransform();
*/

}

//Resolves the collision that occurred between d and other
//xDelta1 and yDelta1 are the amount d moved before causing the collision.
//xDelta2 and yDelta2 are the amount other moved before causing the collision.
void CollisionSystem :: resolveCollision(DisplayObject* d, DisplayObject* other, int xDelta1, int yDelta1, int xDelta2, int yDelta2){



}
