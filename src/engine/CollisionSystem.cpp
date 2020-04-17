#include "CollisionSystem.h"
#include "GangThug.h"
#include "Creeper.h"
#include "Wolf.h"
#include "Projectile.h"


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
  bool FirstObjectNotFound = false;
  bool SecondObjectNotFound = false;
  for(pair<string,string> collisionPair : objectsCollide){
    if (ObjectsOfEachType.find(collisionPair.first) == ObjectsOfEachType.end() ){
        //std::cout << "first not found\n";
        //First Object Type not Found
    }
    else if( ObjectsOfEachType.find(collisionPair.second) == ObjectsOfEachType.end()  ){
        //std::cout << "second not found\n";
        //Second Object Type Not Found
    }
    else{
      // std::cout<< "UPDATE IS CALLED" <<endl;
      for(DisplayObject* OneObject : *ObjectsOfEachType[collisionPair.first] ){
        //cout << OneObject->id;
        for(DisplayObject* SecondObject : *ObjectsOfEachType[collisionPair.second] ){
          //cout << SecondObject->id;
          if(OneObject != SecondObject){
            if(collidesWith(OneObject, SecondObject)){
                // std::cout << "Detected Collision between ";
                // std::cout << OneObject->id;
                // std::cout << " And ";
                // std::cout << SecondObject->id << endl;
                // std::cout<< OneObject->position.x;
                OneObject->onCollision(SecondObject);
                SecondObject->onCollision(OneObject);

            }
          }
        }
      }
    }
  }

}



//This system watches the game's display tree and is notified whenever a display object is placed onto
//or taken off of the tree. Thus, the collision system always knows what DOs are in the game at any moment automatically.
void CollisionSystem :: handleEvent(Event* e){
  //So specify What event the event is, by "e"
  //And then choose how to handle it.
  //
  if(e->getType() == "addObject"){
    this->handleAddDisplayObject( (GameTreeEvent*) e);
  }
  else if(e->getType() == "removeObject"){
    this->handleRemoveDisplayObject( (GameTreeEvent*) e );
  }
}

//Takes in a GameTreeEvent and uses it to update the lists of objects of each particular type.

void CollisionSystem :: handleAddDisplayObject(GameTreeEvent*e){

  cout << "Add Event Triggered: " <<endl;

  if(ObjectsOfEachType.find(e->getModifiedObject()->type) ==  ObjectsOfEachType.end()){

    cout << "Keytype: ";
    cout << e->modifiedObject->type;
    cout << " : Was added as a new type to the Game EventTree" <<endl;

    vector<DisplayObject*>* vectorOfDisplayObjects;

    vectorOfDisplayObjects = new vector<DisplayObject*>;

    this->ObjectsOfEachType.emplace( e->getModifiedObject()->type , vectorOfDisplayObjects  );
    ObjectsOfEachType[e->getModifiedObject()->type]->push_back(e->getModifiedObject());
    cout << "Added both KeyType and Object" <<endl;
  }
  else{
    cout << e->modifiedObject->type;
    cout <<" Already Exists as a typing." <<endl;
    ObjectsOfEachType[e->getModifiedObject()->type]->push_back(e->getModifiedObject());
    cout << "Object Added to the Game Event Tree" << endl;
  }
}


void CollisionSystem :: handleRemoveDisplayObject(GameTreeEvent*e){
  cout << "handleRemoveDisplayObjectIsCalled"<<endl;
    if(ObjectsOfEachType.find(e->getModifiedObject()->type) ==  ObjectsOfEachType.end()){
      //cout << "Keytype has not been added yet. There is nothing to remove"<<endl;
    }
    else{
      auto iter = ObjectsOfEachType[e->getModifiedObject()->type]->begin(); //Create iterator for forloop, starts with the beginning
      for(iter; iter !=  ObjectsOfEachType[e->getModifiedObject()->type]->end(); iter++ ){
          cout << "Erase Iter" <<endl;
          cout << (*iter)->id <<endl;
          if( (*iter)->id == e->getModifiedObject()->id){
              cout <<"Equal to Target"<<endl;
              ObjectsOfEachType[e->getModifiedObject()->type]->erase(iter--);
          }
      }
    }


}




//This function asks the collision system to start checking for collisions between all pairs
//of DOs of a given type (e.g., player vs platform). The system will begin to check all player objects
//against all platform objects that are in the current scene.
void CollisionSystem :: watchForCollisions(string type1, string type2){
  bool boolOrder1 = false;
  bool boolOrder2 = false;
    pair<string, string> Order1 = make_pair (type1, type2);
    pair<string, string> Order2 = make_pair(type2, type1);
  for(pair<string,string> temp : this->objectsCollide){
    if(temp == Order1 ){
      boolOrder1 = true;
    }
    if(temp == Order2){
      boolOrder2 = true;
    }
  }
  if(boolOrder1 == false && boolOrder2 == false){
    this->objectsCollide.push_back(Order1);
    this->collisionTypeMap.insert({Order1, 0});
    //cout <<"Item added to objectsCollide" <<endl;
  }
}

//returns true iff obj1 hitbox and obj2 hitbox overlap. Uses the following method from DO:
//	SDL_Point* DisplayObject::getGlobalHitbox();
bool CollisionSystem :: collidesWith(DisplayObject* obj1, DisplayObject* obj2){
  SDL_Point* obj1Points = obj1->getGlobalHitbox();
  SDL_Point* obj2Points = obj2->getGlobalHitbox();

  if (obj1->type == "GangThug") {
    SDL_Point* obj1Points = ((GangThug*)obj1)->getGlobalHitbox();
  }
  if (obj2->type == "GangThug") {
    SDL_Point* obj2Points = ((GangThug*)obj2)->getGlobalHitbox();
  }
  if (obj1->type == "Projectile") {
    SDL_Point* obj1Points = ((Projectile*)obj1)->getGlobalHitbox();
  }
  if (obj2->type == "Projectile") {
    SDL_Point* obj2Points = ((Projectile*)obj2)->getGlobalHitbox();
  }
  if (obj1->type == "Wolf") {
    SDL_Point* obj1Points = ((Wolf*)obj1)->getGlobalHitbox();
  }
  if (obj2->type == "Wolf") {
    SDL_Point* obj2Points = ((Wolf*)obj2)->getGlobalHitbox();
  }

  int minX = INT_MAX;
  int maxX = INT_MIN;
  int minY = INT_MAX;
  int maxY = INT_MIN;

  for(int i = 0; i < 4 ; i++){
    if(obj2Points[i].x < minX){
      minX = obj2Points[i].x;
    }
    if(obj2Points[i].x > maxX){
      maxX = obj2Points[i].x;
    }
    if(obj2Points[i].y < minY){
      minY = obj2Points[i].y;
    }
    if(obj2Points[i].y > maxY){
      maxY = obj2Points[i].y;
    }
  }
//  cout << minX << "  " << minY << "  " <<  maxX <<  "  " << maxY << endl;
  if(obj1Points[0].x > minX
    && obj1Points[0].x < maxX
    && obj1Points[0].y > minY
    && obj1Points[0].y < maxY ){
  //    cout << "PointWithinOtherObject" <<endl;
      return true;
    }
  return IntersectChecker(obj1Points, obj2Points);
/*
  cout << "TwoLineCheckerTest";
  SDL_Point temp = {0,0};
  SDL_Point temp2 = {10,0};
  SDL_Point temp3 = {5,5};
  SDL_Point temp4 = {5, 1};
  cout << twoLineChecker(temp, temp2, temp3, temp4);
*/
}
  bool CollisionSystem :: IntersectChecker(SDL_Point* obj1, SDL_Point* obj2){
    //https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    //Supplimented the slides.

    //Lines we need to check.
    // 0,0, width,0 ,  0, height, width height.
    // check [0]&[1], 0&2, 1&3  2&3

    //16 lines of code to check the four lines against four lines.
    if( twoLineChecker(obj1[0], obj1[1], obj2[0],obj2[1]) ){
      return true;
    }
    else if( twoLineChecker(obj1[0], obj1[1], obj2[0],obj2[2])){
      return true;
    }
    else if( twoLineChecker(obj1[0], obj1[1], obj2[1],obj2[3])){
      return true;
    }
    else if( twoLineChecker(obj1[0], obj1[1], obj2[2], obj2[3])){
      return true;
    }
//second set of obj1
    else if( twoLineChecker(obj1[0], obj1[2], obj2[0],obj2[1]) ){
      return true;
    }
    else if( twoLineChecker(obj1[0], obj1[2], obj2[0],obj2[2])){
      return true;
    }
    else if( twoLineChecker(obj1[0], obj1[2], obj2[1],obj2[3])){
      return true;
    }
    else if( twoLineChecker(obj1[0], obj1[2], obj2[2], obj2[3])){
      return true;
    }
//Third Set of obj1

    else if( twoLineChecker(obj1[1], obj1[3], obj2[0],obj2[1]) ){
      //cout <<"131" <<endl;
      return true;
    }
    else if( twoLineChecker(obj1[1], obj1[3], obj2[0],obj2[2])){
    //  cout <<"132" <<endl;
      return true;
    }
    else if( twoLineChecker(obj1[1], obj1[3], obj2[1],obj2[3])){
      //cout <<"133" <<endl;
      return true;
    }
    else if( twoLineChecker(obj1[1], obj1[3], obj2[2], obj2[3])){
      //cout <<"134"<<endl;
      return true;
    }
//Set 4
    else if( twoLineChecker(obj1[2], obj1[3], obj2[0],obj2[1]) ){
      return true;
    }
    else if( twoLineChecker(obj1[2], obj1[3], obj2[0],obj2[2])){
      return true;
    }
    else if( twoLineChecker(obj1[2], obj1[3], obj2[1],obj2[3])){
      return true;
    }
    else if( twoLineChecker(obj1[2], obj1[3], obj2[2], obj2[3])){
      return true;
    }

    return false;
  }

  bool CollisionSystem:: twoLineChecker(SDL_Point o1p1, SDL_Point o1p2, SDL_Point o2p1, SDL_Point o2p2 ){
    int o1 = orientationChecker(o1p1, o1p2, o2p1);
    int o2 = orientationChecker(o1p1, o1p2, o2p2);
    int o3 = orientationChecker(o2p1, o2p2, o1p1);
    int o4 = orientationChecker(o2p1, o2p2, o1p2);
    if(o1 != o2 && o3 != o4){
        return true;
    }
    else{
      return false;
    }
  }


  int CollisionSystem :: orientationChecker(SDL_Point p1, SDL_Point p2, SDL_Point p3){
    //https://www.geeksforgeeks.org/orientation-3-ordered-points/
    float slopeP1P2 = (p2.y - p1.y) *(p3.x - p2.x); // Multiplying instead of dividing removes floating point execption
    float slopeP2P3 = (p3.y - p2.y) * (p2.x - p1.x); //Multiplying instead of dividing removes floatign point execption.
    //Equivalent of cross multiplication dy1/dx1 - dy2/dx2 = ?
    //dy1 / dx1 = ? + dy2/dx2
    //dy1 dx2 - dy2 dx1 = ?*dx1*dx2
    if(slopeP2P3-slopeP1P2 == 0){
      return 0;
    }
    else if(slopeP2P3 - slopeP1P2 > 0){
      return -1;
    }
    else{
      return 1;
    }
     //Return 0 if collinear
     //Return -1 if turning clockwise
     //Return 1 if turning counterclockwise
  }


void CollisionSystem :: setSpecialCollisionType(string type1, string type2, int CollisionStyle){
  pair<string, string> Order1 = make_pair (type1, type2);
  //cout << "setSpecialCollisionType";
  //cout << this->collisionTypeMap.at(Order1)<<endl;
  this->collisionTypeMap[Order1] = CollisionStyle;
  //cout << this->collisionTypeMap.at(Order1)<<endl;

}




//???? There is no way to get how much it moved, as you can only pull the current position.
//All modifications have to be done based on the object's current position.

//Resolves the collision that occurred between d and other
//xDelta1 and yDelta1 are the amount d moved before causing the collision.
//xDelta2 and yDelta2 are the amount other moved before causing the collision.
void CollisionSystem :: resolveCollision(DisplayObject* d, DisplayObject* other, int xDelta1, int yDelta1, int xDelta2, int yDelta2){
  //cout << "COLLISION SYSTEM's resolve Collision is being called."<<endl;
  //cout << "XDELTA: " << xDelta1 << " : YDelta: " << yDelta1 <<endl;
  d->position.x -= xDelta1;
  d->position.y -= yDelta1;
  other->position.x -= xDelta2;
  other->position.y -= yDelta2;
  //Positions have been reset to pre-collision.

  binaryXChecker(d, other, xDelta1/2, xDelta2/2);
  binaryYChecker(d, other, yDelta1/2, yDelta2/2);

}
/*
void CollisionSystem:: binaryXChecker(DisplayObject* d, DisplayObject* other, int xDelta1, int xDelta2){
  cout << "BinaryXCheckerCalled x :" << xDelta1 << " : " << xDelta2 <<endl;
  int ShrinkByX1 = xDelta1/2;
  int ShrinkByX2 = xDelta2/2;
  int maxDeltaX1 = 0;
  int maxDeltaX2 = 0;
  maxDeltaX1 += ShrinkByX1;
  maxDeltaX2 += ShrinkByX2;


  if (ShrinkByX1 < 1 && ShrinkByX1 > -1 ) {
    ShrinkByX1 = 0;
    maxDeltaX1 = 0;
  }
  if (ShrinkByX2 < 1 && ShrinkByX2 > -1 ) {
      ShrinkByX2 = 0;
      maxDeltaX2 = 0;
  }
  if( (ShrinkByX1 < 1) && (ShrinkByX1 > -1) && (ShrinkByX2 < 1)  && (ShrinkByX2 > -1)){
    cout << "finished"<<endl;


    return;
  }else{
      d->position.x += maxDeltaX1;
      other->position.x += maxDeltaX2;

      if( collidesWith(d, other) ){
        d->position.x -= maxDeltaX1;
        other->position.x -= maxDeltaX2;
        ShrinkByX1 = ShrinkByX1/2;
        ShrinkByX2 = ShrinkByX2/2;
        binaryXChecker(d, other, ShrinkByX1, ShrinkByX2);
      }
      else{
        ShrinkByX1 = ShrinkByX1/2;
        ShrinkByX2 = ShrinkByX2/2;
        binaryXChecker(d, other, ShrinkByX1, ShrinkByX2);
      }

  }
}

*/


void CollisionSystem:: binaryXChecker(DisplayObject* d, DisplayObject* other, int xDelta1, int xDelta2){
  //cout << "BinaryXCheckerCalled Y :" << xDelta1 << " : " << xDelta2 <<endl;

  int ShrinkByX1 = xDelta1;
  int ShrinkByX2 = xDelta2;
  int maxDeltaX1 = 0;
  int maxDeltaX2 = 0;
  maxDeltaX1 += ShrinkByX1;
  maxDeltaX2 += ShrinkByX2;

  d->position.x += maxDeltaX1;
  other->position.x += maxDeltaX2;
  if( collidesWith(d, other) ){
    d->position.x -= maxDeltaX1;
    other->position.x -= maxDeltaX2;
    ShrinkByX1 = ShrinkByX1/2;
    ShrinkByX2 = ShrinkByX2/2;
    if( (ShrinkByX1 == 0) && (ShrinkByX2 == 0) ){
      //cout << "finished"<<endl;
      return;
    }
    binaryXChecker(d, other, ShrinkByX1, ShrinkByX2);
  }
  else{
    ShrinkByX1 = ShrinkByX1/2;
    ShrinkByX2 = ShrinkByX2/2;
      if( (ShrinkByX1 == 0) && (ShrinkByX2 == 0) ){
        //cout << "finished"<<endl;
        return;
      }
    binaryXChecker(d, other, ShrinkByX1, ShrinkByX2);
  }
}


void CollisionSystem :: binaryYChecker(DisplayObject* d, DisplayObject* other, int yDelta1, int yDelta2){
  //cout << "BinaryYCheckerCalled Y :" << yDelta1 << " : " << yDelta2 <<endl;

  int ShrinkByY1 = yDelta1;
  int ShrinkByY2 = yDelta2;
  int maxDeltaY1 = 0;
  int maxDeltaY2 = 0;
  maxDeltaY1 += ShrinkByY1;
  maxDeltaY2 += ShrinkByY2;

  d->position.y += maxDeltaY1;
  other->position.y += maxDeltaY2;
  if( collidesWith(d, other) ){
    d->position.y -= maxDeltaY1;
    other->position.y -= maxDeltaY2;
    ShrinkByY1 = ShrinkByY1/2;
    ShrinkByY2 = ShrinkByY2/2;
    if( (ShrinkByY1 == 0) && (ShrinkByY2 == 0) ){
      //cout << "finished"<<endl;
      return;
    }
    binaryYChecker(d, other, ShrinkByY1, ShrinkByY2);
  }
  else{
    ShrinkByY1 = ShrinkByY1/2;
    ShrinkByY2 = ShrinkByY2/2;
      if( (ShrinkByY1 == 0) && (ShrinkByY2 == 0) ){
        //cout << "finished"<<endl;
        return;
      }
    binaryYChecker(d, other, ShrinkByY1, ShrinkByY2);
  }
}
