#ifndef SCENE_H
#define SCENE_H

#include "DisplayObjectContainer.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include <string>
#include <vector>
#include <fstream>


#include "../rapidjson/filereadstream.h"
#include <iostream>


#include <rapidjson/writer.h>
#include "../rapidjson/document.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/filewritestream.h"
#include "../rapidjson/pointer.h"
#include "../rapidjson/allocators.h"
#include "../rapidjson/rapidjson.h"


using namespace std;

class Scene : public DisplayObjectContainer{

public:
	Scene();

	/* Load scene from a file */

	void loadScene(string sceneFilePath);
	void saveScene(string sceneFilePath);
	virtual void update(set<SDL_Scancode> pressedKeys);
	virtual void draw(AffineTransform &at);


	rapidjson:: Document savedoc;

/*
			rapidjson:: Document savedoc;

			savedoc.SetObject();
			rapidjson:: Document::AllocatorType& allocator = savedoc.GetAllocator();
			size_t sz = allocator.Size();
*/

	rapidjson:: StringBuffer objectBuffer;

	/*
	char cbuf[1024];
	rapidjson::MemoryPoolAllocator<> allocator (cbuf, sizeof(cbuf));
	rapidjson:: Document savedoc(&allocator, 256);
*/



	void saveDisplayObject(DisplayObject* at);
//	void saveRGB(DisplayObject* at);
void saveRGB(Sprite* at);

	void saveDisplayObjectContainer(DisplayObjectContainer* at);
	void saveSprite(Sprite* at);
	void saveAnimatedSprite(AnimatedSprite* at);


	void saveAnimation(AnimatedSprite* at);
	void saveAnimation(AnimatedSprite* at ,  rapidjson:: Value& animationSet);

/*
	void saveDisplayObject(DisplayObject* at, string filepath);
	void saveRGB(DisplayObject* at, string filepath);
	void saveDisplayObjectContainer(DisplayObjectContainer* at, string filepath);
	void saveSprite(Sprite* at, string filepath);
	void saveAnimatedSprite(AnimatedSprite* at, string filepath);
	void saveAnimation(AnimatedSprite* at, string animName);
*/





/*
	void saveDisplayObject(DisplayObject &at, string filepath);
	void saveRGB(DisplayObject &at, string filepath);
	void saveDisplayObjectContainer(DisplayObjectContainer &at, string filepath);
	void saveSprite(Sprite &at, string filepath);
	void saveAnimatedSprite(AnimatedSprite &at, string filepath);
	void saveAnimation(AnimatedSprite &at, string animName);
*/
private:

};

#endif
