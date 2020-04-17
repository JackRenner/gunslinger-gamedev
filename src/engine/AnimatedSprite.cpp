#include "AnimatedSprite.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Controls.h"

using namespace std;

AnimatedSprite::AnimatedSprite() : Sprite() {
    this->type = "AnimatedSprite";
}

AnimatedSprite::AnimatedSprite(string id) : Sprite(id, 0, 0, 0) {
    this->type = "AnimatedSprite";
}

AnimatedSprite::~AnimatedSprite() {
    for (Animation* an : animations) {
        for (int i = 0; i < an->numFrames; i++) {// this needs to be an iterator loop
            if(an->frames[i]->image != NULL) SDL_FreeSurface(an->frames[i]->image);
	        if(an->frames[i]->texture != NULL) SDL_DestroyTexture(an->frames[i]->texture);
            delete an->frames[i];
        }
        delete an->frames;
        delete an;
    }
}

vector<string> AnimatedSprite::parseXML(string spriteSheet, string xml){
    ifstream myFile;
    myFile.open(xml);
    string line;
    vector<string> ssframes;
    if(myFile.is_open()){
        while (getline(myFile, line)){
            if(line.find("x=") != string::npos){
                ssframes.push_back(line);
            }
        }
        myFile.close();
    } else {
        cout << "Unable to open " << xml << endl;
    }
    return ssframes;
}

void AnimatedSprite::addAnimation(string basepath, string animName, int numFrames, int frameRate, bool loop) {
    Animation* anim = new Animation();
    anim->animName = animName;
    anim->numFrames = numFrames;
    anim->frameRate = frameRate;
    anim->loop = loop;
    anim->curFrame = 0;
    anim->fromSheet = false;
    anim->frames = new Frame*[numFrames]; // new frame pointer array of size numFrames;
    for (int i = 0; i < numFrames; i++ ) {
        Frame* f = new Frame();
        string path = basepath + animName + "_" + to_string(i+1) + ".png";
        f->image = IMG_Load(path.c_str());
        f->texture = SDL_CreateTextureFromSurface(Game::renderer, f->image);
        anim->frames[i] = f;
    }
    animations.push_back(anim);
}

void AnimatedSprite::addAnimation(string spriteSheet, string xml, string animName, int numFrames, int frameRate, bool loop){
    vector<string> ssframes = parseXML(spriteSheet, xml);
    Animation* anim = new Animation();
    anim->animName = animName;
    anim->numFrames = numFrames;
    anim->frameRate = frameRate;
    anim->loop = loop;
    anim->curFrame = 0;
    anim->frames = new Frame*[numFrames];
    anim->fromSheet = true;
    anim->rects = new SDL_Rect*[numFrames];
    for(int i = 0; i < numFrames; i++){
        Frame* f = new Frame();
        f->image = IMG_Load(spriteSheet.c_str());
        f->texture = SDL_CreateTextureFromSurface(Game::renderer, f->image);
        anim->frames[i] = f;

        int x, y, w, h;
        string img = ssframes[i];
        try{
            int xIn = img.find("x=") + 3;
            int xOut = img.find("\"", xIn);
            string xSub = img.substr(xIn, xOut - xIn);
            x = stoi(xSub);
            int yIn = img.find("y=") + 3;
            int yOut = img.find("\"", yIn);
            string ySub = img.substr(yIn, yOut - yIn);
            y = stoi(ySub);
            int wIn = img.find("w=") + 3;
            int wOut = img.find("\"", wIn);
            string wSub = img.substr(wIn, wOut - wIn);
            w = stoi(wSub);
            int hIn = img.find("h=") + 3;
            int hOut = img.find("\"", hIn);
            string hSub = img.substr(hIn, hOut - hIn);
            h = stoi(hSub);
        } catch(exception e){
            cout << "xml formatting in " << xml << " is off" << endl;
        }

        SDL_Rect* r = new SDL_Rect();
        r->h = h;
        r->y = y;
        r->w = w;
        r->x = x;
        anim->rects[i] = r;
    }
    animations.push_back(anim);
}

Animation* AnimatedSprite::getAnimation(string animName) {
    for (int i = 0; i < animations.size(); i++) {
        if (animations[i]->animName == animName) {
            return animations[i];
        }
    }
    return NULL;
}

void AnimatedSprite::play(string animName) {
    Animation* anim = getAnimation(animName);
    if (anim != NULL) {
        this->current = anim;
        this->current->curFrame = 0;
        frameCount = 0;
        playing = true;
    }
}

void AnimatedSprite::replay() {
    if (this->current != NULL) {
        current->curFrame = 0;
        frameCount = 0;
        playing = true;
    }
}

void AnimatedSprite::stop() {
    this->playing = false;
}

void AnimatedSprite::whenDoneRemove(string animName){
    Animation* anim = getAnimation(animName);
    if(anim != NULL) {
        anim->whenDoneRemove = true;
    }
}

void AnimatedSprite::update(set<SDL_Scancode> pressedKeys) {
    Sprite::update(pressedKeys);
    if (playing) {
        frameCount++;
        if (frameCount % current->frameRate == 0) {
            // increment local frame counter
            current->curFrame++;
            // check for array out of bounds
            if (current->curFrame == current->numFrames) {
                // reset the animation
                current->curFrame = 0;
                // check for looping
                if (!current->loop) {
                    stop();
                    if(current->whenDoneRemove){
                        this->removeThis();
                        removed = true;
                    }
                }
            }
            if(!removed){
                DisplayObject::setTexture(current->frames[current->curFrame]->texture);
                if(current->fromSheet) {
                    DisplayObject::setSourceRect(current->rects[current->curFrame]);
                } else {
                    DisplayObject::setSourceRect(NULL);
                }
            }
        }

    }
    
}

void AnimatedSprite::draw(AffineTransform &at) {
    Sprite::draw(at);
}