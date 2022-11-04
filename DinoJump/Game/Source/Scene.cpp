#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "SceneIntro.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->Load();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());


	this->active = false;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;

	if (col)
	{
		// L07 DONE 3: Create colliders
		// Later you can create a function here to load and create the colliders from the map
		PhysBody* c1 = app->physics->CreateRectangle(34 + (380 / 2), 1765 + (18 / 2), 396, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c1->ctype = ColliderType::PLATFORM;

		PhysBody* c2 = app->physics->CreateRectangle(18 + (18 / 2), 0 + (1800 / 2), 18, 1800, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c2->ctype = ColliderType::UNKNOWN;

		PhysBody* c3 = app->physics->CreateRectangle(414 + (18 / 2), 0 + (1800 / 2), 18, 1800, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c3->ctype = ColliderType::UNKNOWN;

		PhysBody* c4 = app->physics->CreateRectangle(288 + (144 / 2), 1674 + (36 / 2), 144, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c4->ctype = ColliderType::PLATFORM;

		PhysBody* c5 = app->physics->CreateRectangle(180 + (36 / 2), 1728 + (18 / 2), 36, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c5->ctype = ColliderType::PLATFORM;

		PhysBody* c6 = app->physics->CreateRectangle(234 + (54 / 2), 1692 + (18 / 2), 54, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c6->ctype = ColliderType::PLATFORM;

		PhysBody* c7 = app->physics->CreateRectangle(216 + (36 / 2), 1638 + (18 / 2), 36, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c7->ctype = ColliderType::PLATFORM;

		PhysBody* c8 = app->physics->CreateRectangle(72 + (126 / 2), 1602 + (36 / 2), 126, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c8->ctype = ColliderType::PLATFORM;

		PhysBody* c9 = app->physics->CreateRectangle(72 + (56 / 2), 1566 + (36 / 2), 54, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c9->ctype = ColliderType::PLATFORM;

		PhysBody* c10 = app->physics->CreateRectangle(18 + (56 / 2), 1512 + (36 / 2), 54, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c10->ctype = ColliderType::PLATFORM;

		PhysBody* c11 = app->physics->CreateRectangle(108 + (36 / 2), 1494 + (18 / 2), 36, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c11->ctype = ColliderType::PLATFORM;

		PhysBody* c12 = app->physics->CreateRectangle(180 + (36 / 2), 1494 + (18 / 2), 36, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c12->ctype = ColliderType::PLATFORM;

		PhysBody* c13 = app->physics->CreateRectangle(270 + (108 / 2), 1476 + (36 / 2), 108, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c13->ctype = ColliderType::PLATFORM;

		PhysBody* c14 = app->physics->CreateRectangle(18 + (56 / 2), 1512 + (36 / 2), 54, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c14->ctype = ColliderType::PLATFORM;

		PhysBody* c15 = app->physics->CreateRectangle(306 + (72 / 2), 1404 + (36 / 2), 72, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c15->ctype = ColliderType::PLATFORM;

		PhysBody* c16 = app->physics->CreateRectangle(396 + (36 / 2), 1350 + (36 / 2), 36, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c16->ctype = ColliderType::PLATFORM;

		PhysBody* c17 = app->physics->CreateRectangle(72 + (180 / 2), 1350 + (36 / 2), 180, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c17->ctype = ColliderType::PLATFORM;

		PhysBody* c18 = app->physics->CreateRectangle(90 + (18 / 2), 1314 + (18 / 2), 18, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c18->ctype = ColliderType::PLATFORM;

		PhysBody* c19 = app->physics->CreateRectangle(18 + (72 / 2), 1278 + (36 / 2), 72, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c19->ctype = ColliderType::PLATFORM;

		PhysBody* c20 = app->physics->CreateRectangle(144 + (108 / 2), 1260 + (36 / 2), 108, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c20->ctype = ColliderType::PLATFORM;

		PhysBody* c21 = app->physics->CreateRectangle(288 + (18 / 2), 1241 + (18 / 2), 18, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c21->ctype = ColliderType::PLATFORM;

		PhysBody* c22 = app->physics->CreateRectangle(306 + (18 / 2), 1224 + (18 / 2), 18, 18, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c22->ctype = ColliderType::PLATFORM;
		col = false;

		PhysBody* c23 = app->physics->CreateRectangle(342 + (90 / 2), 1188 + (36 / 2), 90, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c23->ctype = ColliderType::PLATFORM;
		col = false;

		PhysBody* c24 = app->physics->CreateRectangle(180 + (90 / 2), 1169 + (36 / 2), 90, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c24->ctype = ColliderType::PLATFORM;
		col = false;

		PhysBody* c25 = app->physics->CreateRectangle(18 + (90 / 2), 1170 + (36 / 2), 90, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c25->ctype = ColliderType::PLATFORM;
		col = false;

		PhysBody* c26 = app->physics->CreateRectangle(143 + (126 / 2), 1098 + (36 / 2), 126, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c26->ctype = ColliderType::PLATFORM;
		col = false;

		PhysBody* c27 = app->physics->CreateRectangle(360 + (72 / 2), 1044 + (36 / 2), 72, 36, bodyType::STATIC);
		// L07 DONE 7: Assign collider type
		c27->ctype = ColliderType::PLATFORM;
		col = false;
	
	}

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	if (app->sceneIntro->On) {
		// Draw map
		app->map->Draw();
	}

	return ret;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
