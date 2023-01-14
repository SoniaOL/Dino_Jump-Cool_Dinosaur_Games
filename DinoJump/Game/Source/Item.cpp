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
		texture = app->tex->Load(texturePath);

		// L07 DONE 4: Add a physics to an item - initialize the physics body
		Coin1 = app->physics->CreateRectangleSensor(CoinX1, CoinY1, 16, 16, bodyType::KINEMATIC);

		// L07 DONE 7: Assign collider type
		Coin1->ctype = ColliderType::ITEM;

		Coin2 = app->physics->CreateRectangleSensor(CoinX2, CoinY2, 16, 16, bodyType::KINEMATIC);
		Coin2->ctype = ColliderType::ITEM;
		
		Coin3 = app->physics->CreateRectangleSensor(CoinX3, CoinY3, 16, 16, bodyType::KINEMATIC);
		Coin3->ctype = ColliderType::ITEM;
		
		col = false;
	}

	if (restart) {

		Coin1->body->SetActive(true);
		Coin2->body->SetActive(true);
		Coin3->body->SetActive(true);

		restart = false;
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;*/

	if (Coin1->body->IsActive() == true)
	{
		app->render->DrawTexture(texture, CoinX1 - 16, CoinY1 - 16);
	}
	if (Coin2->body->IsActive() == true)
	{
		app->render->DrawTexture(texture, CoinX2 - 16, CoinY2 - 16);
	}
	if (Coin3->body->IsActive() == true)
	{
		app->render->DrawTexture(texture, CoinX3 - 16, CoinY3 - 16);
	}

	return true;
}

bool Item::CleanUp()
{
	return true;
}