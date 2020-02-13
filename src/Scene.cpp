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
		if (type == "DisplayObjectContainer") {
			DisplayObjectContainer* new_doc = new DisplayObjectContainer();
			new_doc->id = sceneDoc[it->name]["id"].GetString();
			new_doc->position.x = sceneDoc[it->name]["position"][0].GetInt();
			new_doc->position.y = sceneDoc[it->name]["position"][1].GetInt();
			this->addChild(new_doc);
		}
		else if (type == "Sprite") {
			string id = sceneDoc[it->name]["id"].GetString();
			string filepath = sceneDoc[it->name]["filepath"].GetString();
			Sprite* new_sprite = new Sprite(id, filepath);
			new_sprite->position.x = sceneDoc[it->name]["position"][0].GetInt();
			new_sprite->position.y = sceneDoc[it->name]["position"][1].GetInt();
			this->addChild(new_sprite);
		}
		else if (type == "AnimatedSprite") {
			string id = sceneDoc[it->name]["id"].GetString();
			AnimatedSprite* new_animsprite = new AnimatedSprite(id);
			new_animsprite->position.x = sceneDoc[it->name]["position"][0].GetInt();
			new_animsprite->position.y = sceneDoc[it->name]["position"][1].GetInt();
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

