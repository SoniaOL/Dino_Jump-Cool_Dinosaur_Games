#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Item::Start() {

	return true;
}

bool Item::Update()
{

	if (col) 
	{
		//initilize textures
		
		col = false;
	}

	if (restart) {

		

		//restart = false;
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;*/

	

	return true;
}

bool Item::CleanUp()
{
	return true;
}