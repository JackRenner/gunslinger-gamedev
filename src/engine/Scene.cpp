#include "Scene.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include <iostream>

using namespace std;

Scene::Scene() {
	this->type = "Scene";
}

void Scene::loadScene(string sceneFilePath) {

	rapidjson::Document sceneDoc;
	const char* sceneFile = sceneFilePath.c_str();
	FILE* fp = fopen(sceneFile, "rb");
	if (fp == NULL) {
		exit(-1);
	}

	char buffer[2048];
	rapidjson::FileReadStream frs(fp, buffer, sizeof(buffer));

	sceneDoc.ParseStream(frs);

	fclose(fp);

	for (rapidjson::Value::ConstMemberIterator it = sceneDoc.MemberBegin(); it != sceneDoc.MemberEnd(); ++it) {

		string type = sceneDoc[it->name]["type"].GetString();

		// DisplayObjectContainers should only be declared if they are intended to be empty containers; this method will not handle constructor overloading for DOCs
		if (type == "DisplayObjectContainer") {
			// Make empty DOC
			DisplayObjectContainer* new_doc = new DisplayObjectContainer();

			// set transform parameters
			new_doc->position.x = sceneDoc[it->name]["position"][0].GetInt();
			new_doc->position.y = sceneDoc[it->name]["position"][1].GetInt();
			new_doc->pivot.x = sceneDoc[it->name]["pivot"][0].GetInt();
			new_doc->pivot.y = sceneDoc[it->name]["pivot"][1].GetInt();
			new_doc->scaleX = sceneDoc[it->name]["scale"][0].GetDouble();
			new_doc->scaleY = sceneDoc[it->name]["scale"][1].GetDouble();
			new_doc->rotation = sceneDoc[it->name]["rotation"].GetDouble();
			new_doc->alpha = sceneDoc[it->name]["alpha"].GetInt();
			new_doc->visible = sceneDoc[it->name]["visible"].GetBool();

			this->addChild(new_doc);
		}
		// As of now Sprites only support textures from filepaths; will eventually add overloading for RGB
		else if (type == "Sprite") {
			// construct new Sprite
			string id = sceneDoc[it->name]["id"].GetString();
			string filepath = sceneDoc[it->name]["filepath"].GetString();
			Sprite* new_sprite = new Sprite(id, filepath);

			// set transform parameters
			new_sprite->position.x = sceneDoc[it->name]["position"][0].GetInt();
			new_sprite->position.y = sceneDoc[it->name]["position"][1].GetInt();
			new_sprite->pivot.x = sceneDoc[it->name]["pivot"][0].GetInt();
			new_sprite->pivot.y = sceneDoc[it->name]["pivot"][1].GetInt();
			new_sprite->scaleX = sceneDoc[it->name]["scale"][0].GetDouble();
			new_sprite->scaleY = sceneDoc[it->name]["scale"][1].GetDouble();
			new_sprite->rotation = sceneDoc[it->name]["rotation"].GetDouble();
			new_sprite->alpha = sceneDoc[it->name]["alpha"].GetInt();
			new_sprite->visible = sceneDoc[it->name]["visible"].GetBool();

			this->addChild(new_sprite);
		}
		else if (type == "AnimatedSprite") {
			// construct new AnimatedSprite
			string id = sceneDoc[it->name]["id"].GetString();
			AnimatedSprite* new_animsprite = new AnimatedSprite(id);

			string basepath = sceneDoc[it->name]["animation1"]["basepath"].GetString();
			string animName = sceneDoc[it->name]["animation1"]["animName"].GetString();
			int numFrames = sceneDoc[it->name]["animation1"]["numFrames"].GetInt();
			int frameRate = sceneDoc[it->name]["animation1"]["frameRate"].GetInt();
			bool loop = sceneDoc[it->name]["animation1"]["loop"].GetBool();
			new_animsprite->addAnimation(basepath, animName, numFrames, frameRate, loop);

			new_animsprite->play(sceneDoc[it->name]["playing"].GetString());

			// set transform parameters
			new_animsprite->position.x = sceneDoc[it->name]["position"][0].GetInt();
			new_animsprite->position.y = sceneDoc[it->name]["position"][1].GetInt();
			new_animsprite->pivot.x = sceneDoc[it->name]["pivot"][0].GetInt();
			new_animsprite->pivot.y = sceneDoc[it->name]["pivot"][1].GetInt();
			new_animsprite->scaleX = sceneDoc[it->name]["scale"][0].GetDouble();
			new_animsprite->scaleY = sceneDoc[it->name]["scale"][1].GetDouble();
			new_animsprite->rotation = sceneDoc[it->name]["rotation"].GetDouble();
			new_animsprite->alpha = sceneDoc[it->name]["alpha"].GetInt();
			new_animsprite->visible = sceneDoc[it->name]["visible"].GetBool();

			this->addChild(new_animsprite);
		}
	}
}

void Scene::update(set<SDL_Scancode> pressedKeys) {
	DisplayObjectContainer::update(pressedKeys);
}

void Scene::draw(AffineTransform& at) {
	DisplayObjectContainer::draw(at);
}

