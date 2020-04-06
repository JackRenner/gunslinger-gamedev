#include "MyObject.h"
#include "Game.h"
#include <iostream>
using namespace std;

MyObject::MyObject() : Sprite() {
    this->type = "MyObject";
}

MyObject::MyObject(string id, string filepath) : Sprite(id, filepath){
    this->type = "MyObject";
}

MyObject::MyObject(string id, int red, int green, int blue) : Sprite(id, red, green, blue) {
    this->type = "MyObject";
}

void MyObject::update(set<SDL_Scancode> pressedKeys) {
    Sprite::update(pressedKeys);
}

void MyObject:: savePosition(){
  this->oldX = position.x;
  this->oldY = position.y;
}

void MyObject::draw(AffineTransform &at) {
  Sprite::draw(at);
}


void MyObject:: onCollision(DisplayObject * otherObject){
  cout << "MYOBJECT's onCollision is being called." <<endl;
    Game::instance->ourCollisionSystem->resolveCollision(this, otherObject , this->position.x - oldX, this->position.y-oldY, 0, 0);

}
