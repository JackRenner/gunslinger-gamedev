#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include "Scene.h"

using namespace std;

Scene::Scene(): DisplayObjectContainer(){

}

void Scene::loadScene(string sceneFilePath){
    //1. Parse JSON file to array
    //2. iterate through array and determine which constructors to use for object in scene
    //3. construct each child and modify its properties based on info in JSON object
    //4. iterate through *possible* children of each object and construct / modify data
    //5. add children to parent and then parents to scene

    

}

void Scene::update(set<SDL_Scancode> pressedKeys) {
    DisplayObjectContainer::update(pressedKeys);
}

void Scene::draw(AffineTransform &at) {
    DisplayObjectContainer::draw(at);
}