#include "Scene.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include <iostream>

using namespace std;

Scene::Scene() : DisplayObjectContainer() {
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

	// struct to hold DOC pointers and JSON object names so the method can set parents later
	vector<DisplayObjectContainer*> entityStore;


	this->id = sceneDoc["scene_id"].GetString();
	// for loop to load entities in and will set them as children of this scene initially
	for (rapidjson::Value::ConstMemberIterator it = sceneDoc.MemberBegin() + 1; it != sceneDoc.MemberEnd(); ++it) {

		const char* entityName = it->name.GetString();
		string type = sceneDoc[entityName]["type"].GetString();

		// DisplayObjectContainers should only be declared if they are intended to be empty containers; this method will not handle constructor overloading for DOCs
		if (type == "DisplayObjectContainer") {
			// Make empty DOC
			DisplayObjectContainer* new_doc = new DisplayObjectContainer();
			new_doc->id = entityName;

			// set transform parameters
			new_doc->position.x = sceneDoc[entityName]["position"][0].GetInt();
			new_doc->position.y = sceneDoc[entityName]["position"][1].GetInt();
			new_doc->pivot.x = sceneDoc[entityName]["pivot"][0].GetInt();
			new_doc->pivot.y = sceneDoc[entityName]["pivot"][1].GetInt();
			new_doc->scaleX = sceneDoc[entityName]["scale"][0].GetDouble();
			new_doc->scaleY = sceneDoc[entityName]["scale"][1].GetDouble();
			new_doc->rotation = sceneDoc[entityName]["rotation"].GetDouble();
			new_doc->alpha = sceneDoc[entityName]["alpha"].GetInt();
			new_doc->visible = sceneDoc[entityName]["visible"].GetBool();

			entityStore.push_back(new_doc);
		}
		// If an entity is declared as a Sprite in the JSON, it must have an image filepath, otherwise use type RGB
		else if (type == "Sprite") {
			// construct new Sprite
			string id = entityName;
			string filepath = sceneDoc[entityName]["filepath"].GetString();
			Sprite* new_sprite = new Sprite(id, filepath);

			// set transform parameters
			new_sprite->width = sceneDoc[entityName]["dimensions"][0].GetInt();
			new_sprite->height = sceneDoc[entityName]["dimensions"][1].GetInt();
			new_sprite->position.x = sceneDoc[entityName]["position"][0].GetInt();
			new_sprite->position.y = sceneDoc[entityName]["position"][1].GetInt();
			new_sprite->pivot.x = sceneDoc[entityName]["pivot"][0].GetInt();
			new_sprite->pivot.y = sceneDoc[entityName]["pivot"][1].GetInt();
			new_sprite->scaleX = sceneDoc[entityName]["scale"][0].GetDouble();
			new_sprite->scaleY = sceneDoc[entityName]["scale"][1].GetDouble();
			new_sprite->rotation = sceneDoc[entityName]["rotation"].GetDouble();
			new_sprite->alpha = sceneDoc[entityName]["alpha"].GetInt();
			new_sprite->visible = sceneDoc[entityName]["visible"].GetBool();

			entityStore.push_back(new_sprite);
		}
		// mainly for testing and basic blocking
		else if (type == "RGB") {
			// construct new Sprite
			string id = entityName;
			int red = sceneDoc[entityName]["rgb"][0].GetInt();
			int green = sceneDoc[entityName]["rgb"][1].GetInt();
			int blue = sceneDoc[entityName]["rgb"][2].GetInt();
			Sprite* new_rgb = new Sprite(id, red, green, blue);

			// set transform parameters
			new_rgb->width = sceneDoc[entityName]["dimensions"][0].GetInt();
			new_rgb->height = sceneDoc[entityName]["dimensions"][1].GetInt();
			new_rgb->position.x = sceneDoc[entityName]["position"][0].GetInt();
			new_rgb->position.y = sceneDoc[entityName]["position"][1].GetInt();
			new_rgb->pivot.x = sceneDoc[entityName]["pivot"][0].GetInt();
			new_rgb->pivot.y = sceneDoc[entityName]["pivot"][1].GetInt();
			new_rgb->scaleX = sceneDoc[entityName]["scale"][0].GetDouble();
			new_rgb->scaleY = sceneDoc[entityName]["scale"][1].GetDouble();
			new_rgb->rotation = sceneDoc[entityName]["rotation"].GetDouble();
			new_rgb->alpha = sceneDoc[entityName]["alpha"].GetInt();
			new_rgb->visible = sceneDoc[entityName]["visible"].GetBool();

			entityStore.push_back(new_rgb);
		}
		else if (type == "AnimatedSprite") {
			// construct new AnimatedSprite
			string id = entityName;
			AnimatedSprite* new_animsprite = new AnimatedSprite(id);

			// loop through all animations
			for (rapidjson::Value::ConstMemberIterator it = sceneDoc[entityName]["animations"].MemberBegin(); it != sceneDoc[entityName]["animations"].MemberEnd(); ++it) {
				const char* animationID = it->name.GetString();

				string basepath = sceneDoc[entityName]["animations"][animationID]["basepath"].GetString();
				string animName = sceneDoc[entityName]["animations"][animationID]["animName"].GetString();
				int numFrames = sceneDoc[entityName]["animations"][animationID]["numFrames"].GetInt();
				int frameRate = sceneDoc[entityName]["animations"][animationID]["frameRate"].GetInt();
				bool loop = sceneDoc[entityName]["animations"][animationID]["loop"].GetBool();
				new_animsprite->addAnimation(basepath, animName, numFrames, frameRate, loop);
			}

			new_animsprite->play(sceneDoc[entityName]["playing"].GetString());
			std::cout << "adding animation" << endl;
			std::cout << sceneDoc[entityName]["playing"].GetString() << endl;
			// set transform parameters
			new_animsprite->width = sceneDoc[entityName]["dimensions"][0].GetInt();
			new_animsprite->height = sceneDoc[entityName]["dimensions"][1].GetInt();
			new_animsprite->position.x = sceneDoc[entityName]["position"][0].GetInt();
			new_animsprite->position.y = sceneDoc[entityName]["position"][1].GetInt();
			new_animsprite->pivot.x = sceneDoc[entityName]["pivot"][0].GetInt();
			new_animsprite->pivot.y = sceneDoc[entityName]["pivot"][1].GetInt();
			new_animsprite->scaleX = sceneDoc[entityName]["scale"][0].GetDouble();
			new_animsprite->scaleY = sceneDoc[entityName]["scale"][1].GetDouble();
			new_animsprite->rotation = sceneDoc[entityName]["rotation"].GetDouble();
			new_animsprite->alpha = sceneDoc[entityName]["alpha"].GetInt();
			new_animsprite->visible = sceneDoc[entityName]["visible"].GetBool();

			entityStore.push_back(new_animsprite);
		}
		else if (type == "Layer") {
			Layer* new_doc = new Layer();
			new_doc->id = entityName;

			// set transform parameters
			new_doc->position.x = sceneDoc[entityName]["position"][0].GetInt();
			new_doc->position.y = sceneDoc[entityName]["position"][1].GetInt();
			new_doc->pivot.x = sceneDoc[entityName]["pivot"][0].GetInt();
			new_doc->pivot.y = sceneDoc[entityName]["pivot"][1].GetInt();
			new_doc->scaleX = sceneDoc[entityName]["scale"][0].GetDouble();
			new_doc->scaleY = sceneDoc[entityName]["scale"][1].GetDouble();
			new_doc->rotation = sceneDoc[entityName]["rotation"].GetDouble();
			new_doc->alpha = sceneDoc[entityName]["alpha"].GetInt();
			new_doc->visible = sceneDoc[entityName]["visible"].GetBool();
			new_doc->scrollSpeedX = sceneDoc[entityName]["scrollSpeedX"].GetDouble();
			new_doc->scrollSpeedY = sceneDoc[entityName]["scrollSpeedY"].GetDouble();

			entityStore.push_back(new_doc);
		}
	}

	// Loop through the temporary store, and set the parent to Scene if key-value is null/not found, otherwise set real parent
	for (int i = 0; i < entityStore.size(); i++) {
		const char* jsonID = entityStore[i]->id.c_str();
		if (sceneDoc[jsonID]["parent"].IsNull()) {
			this->addChild(entityStore[i]);
			entityStore[i]->parent = this;
		}
		else {
			string parent = sceneDoc[jsonID]["parent"].GetString();
			bool found = false;
			for (int j = 0; j < entityStore.size(); j++) {
				if (entityStore[j]->id == parent) {
					entityStore[j]->addChild(entityStore[i]);
					entityStore[i]->parent = entityStore[j];
					found = true;
					break;
				}
			}
			if (!found) {
				this->addChild(entityStore[i]);
				entityStore[i]->parent = this;
			}
		}
	}
}

void Scene::update(set<SDL_Scancode> pressedKeys) {
	DisplayObjectContainer::update(pressedKeys);
}

void Scene::draw(AffineTransform& at) {
	DisplayObjectContainer::draw(at);
}

