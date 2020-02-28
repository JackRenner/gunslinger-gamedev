#include "Scene.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace rapidjson;




Scene::Scene() : DisplayObjectContainer() {
	this->type = "Scene";
	this->id = "Default_Scene_id";
	this->savedoc.SetObject();
}




void Scene:: saveScene(string sceneFilePath){



		//This resets the savedoc with an empty doc so we no longer have old code in the save.
		Document().Swap(this->savedoc).SetObject(); // minimize and recreate allocator
		Document(kObjectType).Swap(this->savedoc); // after merging #369


		char writebuffer[10000];
		rapidjson:: Document::AllocatorType& allocator = this->savedoc.GetAllocator();
		size_t sz = allocator.Size();
/*
*/
		string tempString = this->id;
//		const char * cstr = tempString.c_str();
		Value id;
		id.SetString(tempString.c_str(),  static_cast<SizeType> (tempString.size()), allocator);

		this->savedoc.AddMember("scene_id", id, allocator);



		cout << this -> children.size()<<endl;
		for(int i = 0; i < this->numChildren(); i++){
			cout << i << endl;





			if(this->getChild(i)-> type == "DisplayObjectContainer" &&  this->getChild(i)->isRGB == false ){
				saveDisplayObjectContainer(dynamic_cast<DisplayObjectContainer*>(this->getChild(i)));
			}else if(this->getChild(i)->isRGB && this->getChild(i)->type == "Sprite" ){
				saveRGB(dynamic_cast<Sprite*>(this->getChild(i)));
			}
			else if(this->getChild(i)-> type == "Sprite"){
				saveSprite(dynamic_cast<Sprite*>(this->getChild(i)));
			}
			else if(this->getChild(i)-> type == "AnimatedSprite"){
				saveAnimatedSprite(dynamic_cast<AnimatedSprite*> (this->getChild(i)));
			}
			else if(this->getChild(i)-> type == "DisplayObject"){
					saveDisplayObject(this->getChild(i));
			}



		}




		const char* sceneFile = sceneFilePath.c_str();
		FILE*fp = fopen(sceneFile, "w");
		rapidjson:: FileWriteStream writestream(fp, writebuffer, sizeof(writebuffer));
		PrettyWriter<rapidjson:: FileWriteStream> writer(writestream);
		writestream.Flush();//Erase the buffer
		this->savedoc.Accept(writer);
		fclose(fp);

	}

	void Scene :: saveDisplayObject(DisplayObject* at){
		cout <<"DO";
	}
	void Scene :: saveRGB(Sprite* at){
		cout <<"RGB";
				rapidjson:: Document::AllocatorType& allocator = this->savedoc.GetAllocator();
				size_t sz = allocator.Size();
				string tempString = at->id;
				string tempString2;
				Value id (kObjectType);
				id.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);


		/*
		//This was singlehandedly the most horrifying workaround I have ever written in my Life.
		//And it was actually an easy fix. I am keeping this here in case something breaks again.
				std:: ostringstream TheMChoiWorkAround; //What am I doing with my life.
				//How did I resort to this.
				TheMChoiWorkAround << at->id << endl; //If assigned directly to cstr
				//THen the temporary object created will be destroyed before it can be fully formed.
				//So I need to assign it to something else first.
				 string tempString2 = TheMChoiWorkAround.str();
				 cout << "WHAT DID I DO?";
				 cout <<tempString2;
				const char * cstr = tempString2.c_str();

					cout<< cstr <<endl;
				//cout <<cstr<<endl;
					Value id (kObjectType);
		//		id.SetString(TheMChoiWorkAround.str());

				id.SetString(cstr, static_cast<SizeType> (tempString2.size() - 2), allocator);
		*/


				Value converter (kObjectType);
				Value converter2 (kObjectType);
				Value Properties(kObjectType);
				Value TempArray(kArrayType);
				int tempInt;
				int tempInt2;
				int tempInt3;
				double tempDouble;
				double tempDouble2;
				bool tempBool;

				tempString = "RGB";
				converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
				Properties.AddMember("type", converter, allocator);


		//RGB
				Value TempArray0(kArrayType);
				tempInt = at->red;
				tempInt2 = at->green;
				tempInt3 = at->blue;

				TempArray0.PushBack(Value().SetInt(tempInt), allocator);
				TempArray0.PushBack(Value().SetInt(tempInt2), allocator);
				TempArray0.PushBack(Value().SetInt(tempInt3), allocator);
				Properties.AddMember("rgb", TempArray0, allocator);

		//dimensions - Is this the width and the height?
		//Or what we get from the getWidth and getHeight functions?
		// Regardless of which, It should be a relatively easy fix.

				tempInt= (at->width);
				tempInt2 = (at->height);

				TempArray.PushBack(Value().SetInt(tempInt), allocator);
				TempArray.PushBack(Value().SetInt(tempInt2), allocator);
				Properties.AddMember("dimensions", TempArray, allocator );

				//Position;
							Value TempArray2(kArrayType); //I got frustrated trying to reset the TempArray
							tempInt = (at->position.x);
							tempInt2 = (at->position.y);
							TempArray2.PushBack(Value().SetInt(tempInt), allocator);
							TempArray2.PushBack(Value().SetInt(tempInt2), allocator);
							Properties.AddMember("position", TempArray2, allocator);
				//pivot
						Value TempArray3(kArrayType);
						tempInt = (at->pivot.x);
						tempInt2 = (at->pivot.y);
						TempArray3.PushBack(Value().SetInt(tempInt), allocator);
						TempArray3.PushBack(Value().SetInt(tempInt2), allocator);
						Properties.AddMember("pivot", TempArray3, allocator);
		//scale
				Value TempArray4(kArrayType);
				tempDouble = (at->scaleX);
				tempDouble2 = (at->scaleY);
				TempArray4.PushBack(Value().SetDouble(tempDouble), allocator);
				TempArray4.PushBack(Value().SetDouble(tempDouble2), allocator);
				Properties.AddMember("scale", TempArray4, allocator);
		//Rotation
				tempDouble = (at->rotation);
				Properties.AddMember("rotation", tempDouble, allocator);
		//Alpha
				tempInt = (at->alpha);
				Properties.AddMember("alpha", tempInt, allocator);
		//visible
				tempBool = (at->visible);
				Properties.AddMember("visible", tempBool, allocator);
		//parent
				tempString = (at->parent->id);
				if(tempString == this->id){
					//tempString = "I'm a Dad";
					//Don't know how to set this to null without breaking this
					Properties.AddMember("parent", Value(), allocator); //Vlank Value is null;
				}
				else{
					converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
					Properties.AddMember("parent", converter, allocator);
				}
		//
		//
		//
				this->savedoc.AddMember(id, Properties, allocator);

				for(int i = 0; i < at->numChildren(); i++){
				if(at->getChild(i)-> type == "DisplayObjectContainer" &&  at->getChild(i)->isRGB == false ){
				      saveDisplayObjectContainer(dynamic_cast<DisplayObjectContainer*>(at->getChild(i)));
				    }else if(at->getChild(i)->isRGB && at->getChild(i)->type == "Sprite" ){
				      saveRGB(dynamic_cast<Sprite*>(at->getChild(i)));
				    }
				    else if(at->getChild(i)-> type == "Sprite"){
				      saveSprite(dynamic_cast<Sprite*>(at->getChild(i)));
				    }
				    else if(at->getChild(i)-> type == "AnimatedSprite"){
				      saveAnimatedSprite(dynamic_cast<AnimatedSprite*> (at->getChild(i)));
				    }
				    else if(at->getChild(i)-> type == "DisplayObject"){
				        saveDisplayObject(at->getChild(i));
				    }
				  }

	}
	void Scene :: saveDisplayObjectContainer(DisplayObjectContainer* at){
		cout <<"DOC";
		rapidjson:: Document::AllocatorType& allocator = this->savedoc.GetAllocator();
		size_t sz = allocator.Size();

		string tempString = at->id;
		string tempString2;
		Value id (kObjectType);
		id.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);

		Value converter (kObjectType);
		Value converter2 (kObjectType);
		Value Properties(kObjectType);
		Value TempArray(kArrayType);
		int tempInt;
		int tempInt2;
		double tempDouble;
		double tempDouble2;
		bool tempBool;


//Filetype
		tempString = at->type;
		converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
		Properties.AddMember("type", converter, allocator);

/*
//dimensions

		tempInt= (at->width);
		tempInt2 = (at->height);

		TempArray.PushBack(Value().SetInt(tempInt), allocator);
		TempArray.PushBack(Value().SetInt(tempInt2), allocator);
		Properties.AddMember("dimensions", TempArray, allocator );
*/
//Position;
			Value TempArray2(kArrayType); //I got frustrated trying to reset the TempArray
			tempInt = (at->position.x);
			tempInt2 = (at->position.y);
			TempArray2.PushBack(Value().SetInt(tempInt), allocator);
			TempArray2.PushBack(Value().SetInt(tempInt2), allocator);
			Properties.AddMember("position", TempArray2, allocator);
//pivot
		Value TempArray3(kArrayType);
		tempInt = (at->pivot.x);
		tempInt2 = (at->pivot.y);
		TempArray3.PushBack(Value().SetInt(tempInt), allocator);
		TempArray3.PushBack(Value().SetInt(tempInt2), allocator);
		Properties.AddMember("pivot", TempArray3, allocator);
//scale
		Value TempArray4(kArrayType);
		tempDouble = (at->scaleX);
		tempDouble2 = (at->scaleY);
		TempArray4.PushBack(Value().SetDouble(tempDouble), allocator);
		TempArray4.PushBack(Value().SetDouble(tempDouble2), allocator);
		Properties.AddMember("scale", TempArray4, allocator);
//Rotation
		tempDouble = (at->rotation);
		Properties.AddMember("rotation", tempDouble, allocator);
//Alpha
		tempInt = (at->alpha);
		Properties.AddMember("alpha", tempInt, allocator);
//visible
		tempBool = (at->visible);
		Properties.AddMember("visible", tempBool, allocator);
//parent
		tempString = (at->parent->id);
		if(tempString == this->id){
			//tempString = "I'm a Dad";
			//Don't know how to set this to null without breaking this
			Properties.AddMember("parent", Value(), allocator); //Vlank Value is null;
		}
		else{
			converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
			Properties.AddMember("parent", converter, allocator);
		}

		this->savedoc.AddMember(id, Properties, allocator);

		for(int i = 0; i < at->numChildren(); i++){
		if(at->getChild(i)-> type == "DisplayObjectContainer" &&  at->getChild(i)->isRGB == false ){
					saveDisplayObjectContainer(dynamic_cast<DisplayObjectContainer*>(at->getChild(i)));
				}else if(at->getChild(i)->isRGB && at->getChild(i)->type == "Sprite" ){
					saveRGB(dynamic_cast<Sprite*>(at->getChild(i)));
				}
				else if(at->getChild(i)-> type == "Sprite"){
					saveSprite(dynamic_cast<Sprite*>(at->getChild(i)));
				}
				else if(at->getChild(i)-> type == "AnimatedSprite"){
					saveAnimatedSprite(dynamic_cast<AnimatedSprite*> (at->getChild(i)));
				}
				else if(at->getChild(i)-> type == "DisplayObject"){
						saveDisplayObject(at->getChild(i));
				}
			}

	}


	void Scene :: saveSprite(Sprite* at){

		rapidjson:: Document::AllocatorType& allocator = this->savedoc.GetAllocator();
		size_t sz = allocator.Size();


		string tempString = at->id;
		string tempString2;
		Value id (kObjectType);
		id.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);




		Value converter (kObjectType);
		Value converter2 (kObjectType);
		Value Properties(kObjectType);
		Value TempArray(kArrayType);
		int tempInt;
		int tempInt2;
		double tempDouble;
		double tempDouble2;
		bool tempBool;

		tempString = at->type;
		converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
		Properties.AddMember("type", converter, allocator);


//filepath
		tempString = (at->imgPath);
		converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
		Properties.AddMember("filepath", converter, allocator);

//dimensions - Is this the width and the height?
//Or what we get from the getWidth and getHeight functions?
// Regardless of which, It should be a relatively easy fix.

		tempInt= (at->width);
		tempInt2 = (at->height);

		TempArray.PushBack(Value().SetInt(tempInt), allocator);
		TempArray.PushBack(Value().SetInt(tempInt2), allocator);
		Properties.AddMember("dimensions", TempArray, allocator );

//Position;
			Value TempArray2(kArrayType); //I got frustrated trying to reset the TempArray
			tempInt = (at->position.x);
			tempInt2 = (at->position.y);
			TempArray2.PushBack(Value().SetInt(tempInt), allocator);
			TempArray2.PushBack(Value().SetInt(tempInt2), allocator);
			Properties.AddMember("position", TempArray2, allocator);
//pivot
		Value TempArray3(kArrayType);
		tempInt = (at->pivot.x);
		tempInt2 = (at->pivot.y);
		TempArray3.PushBack(Value().SetInt(tempInt), allocator);
		TempArray3.PushBack(Value().SetInt(tempInt2), allocator);
		Properties.AddMember("pivot", TempArray3, allocator);
//scale
		Value TempArray4(kArrayType);
		tempDouble = (at->scaleX);
		tempDouble2 = (at->scaleY);
		TempArray4.PushBack(Value().SetDouble(tempDouble), allocator);
		TempArray4.PushBack(Value().SetDouble(tempDouble2), allocator);
		Properties.AddMember("scale", TempArray4, allocator);
//Rotation
		tempDouble = (at->rotation);
		Properties.AddMember("rotation", tempDouble, allocator);
//Alpha
		tempInt = (at->alpha);
		Properties.AddMember("alpha", tempInt, allocator);
//visible
		tempBool = (at->visible);
		Properties.AddMember("visible", tempBool, allocator);
//parent
		tempString = (at->parent->id);
		if(tempString == this->id){
			//tempString = "I'm a Dad";
			//Don't know how to set this to null without breaking this
			Properties.AddMember("parent", Value(), allocator); //Vlank Value is null;
		}
		else{
			converter.SetString(tempString.c_str(), static_cast<SizeType> (tempString.size()), allocator);
			Properties.AddMember("parent", converter, allocator);
		}
//
//
//
		this->savedoc.AddMember(id, Properties, allocator);

		for(int i = 0; i < at->numChildren(); i++){
		if(at->getChild(i)-> type == "DisplayObjectContainer" &&  at->getChild(i)->isRGB == false ){
					saveDisplayObjectContainer(dynamic_cast<DisplayObjectContainer*>(at->getChild(i)));
				}else if(at->getChild(i)->isRGB && at->getChild(i)->type == "Sprite" ){
					saveRGB(dynamic_cast<Sprite*>(at->getChild(i)));
				}
				else if(at->getChild(i)-> type == "Sprite"){
					saveSprite(dynamic_cast<Sprite*>(at->getChild(i)));
				}
				else if(at->getChild(i)-> type == "AnimatedSprite"){
					saveAnimatedSprite(dynamic_cast<AnimatedSprite*> (at->getChild(i)));
				}
				else if(at->getChild(i)-> type == "DisplayObject"){
						saveDisplayObject(at->getChild(i));
				}
			}

	}
	void Scene :: saveAnimatedSprite(AnimatedSprite* at){
		cout <<"ANimSprite";
	}
	void Scene :: saveAnimation(AnimatedSprite* at){}



	/*
	//This was singlehandedly the most horrifying workaround I have ever written in my Life.
	//And it was actually an easy fix. I am keeping this here in case something breaks again.
			std:: ostringstream TheMChoiWorkAround; //What am I doing with my life.
			//How did I resort to this.
			TheMChoiWorkAround << at->id << endl; //If assigned directly to cstr
			//THen the temporary object created will be destroyed before it can be fully formed.
			//So I need to assign it to something else first.
			 string tempString2 = TheMChoiWorkAround.str();
			 cout << "WHAT DID I DO?";
			 cout <<tempString2;
			const char * cstr = tempString2.c_str();

				cout<< cstr <<endl;
			//cout <<cstr<<endl;
				Value id (kObjectType);
	//		id.SetString(TheMChoiWorkAround.str());

			id.SetString(cstr, static_cast<SizeType> (tempString2.size() - 2), allocator);
	*/




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
