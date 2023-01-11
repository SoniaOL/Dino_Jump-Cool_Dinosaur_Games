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
#include "PathFinding.h"
#include "ModuleFadeToBlack.h"


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
	//for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}

	

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	fly = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLYENEMY); 
	fly->parameters = config.child("enemies"); 

	walk = (WalkEnemy*)app->entityManager->CreateEntity(EntityType::WALKENEMY);
	walk->parameters = config.child("enemies");


	audioPath = config.child("player").child("audio").attribute("path").as_string();
	/*audio = app->audio->LoadFx(audioPath);*/

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	// L03: DONE: Load map
	// L03: DONE: Load map
	bool retLoad = app->map->Load();

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		int wW, hW;
		uchar* dataW = NULL;

		bool retWalkMapW = app->map->CreateWalkabilityMap2(wW, hW, &dataW);
		if (retWalkMapW) app->pathfinding2->SetMap(wW, hW, dataW);

		RELEASE_ARRAY(dataW);

	}

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());


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

	if (player->die) {
		player->LAV->body->SetLinearVelocity(b2Vec2(0 ,0));
		player->camg = false;
		player->lava = false; 
		player->GodMode = false;
		countGod = 0;
		app->entityManager->active = false;
		if (app->scene->fly->isDead) {
			app->scene->fly->col = true;
		}
		if (app->scene->walk->isDead) {
			app->scene->walk->col = true;
		}
		app->map->active = false;
		app->sceneIntro->On = false;
		/*app->sceneIntro->active = true;*/
		app->physics->debug = false;
	/*	this->active = false;*/
		app->fade->FadeToBlack(this, (Module*)app->sceneLose, 40);
	}

	if (player->Meta) {
		player->LAV->body->SetLinearVelocity(b2Vec2(0, 0));
		player->camg = false;
		player->lava = false;
		player->GodMode = false;
		countGod = 0;
		app->entityManager->active = false;
		if (app->scene->fly->isDead) {
			app->scene->fly->col = true;
		}
		if (app->scene->walk->isDead) {
			app->scene->walk->col = true;
		}
		app->map->active = false;
		app->sceneIntro->On = false;
		app->sceneIntro->active = true;
		app->physics->debug = false;
		app->sceneIntro->reset = true;
		app->SaveGameRequest();
		this->active = false;
	}

	if (col)
	{
		app->scene->player->die = false;
		app->scene->player->DieCounter = 3;
		app->scene->player->Meta = false;
		app->entityManager->active = true;
		app->map->active = true;
		app->sceneIntro->On = true;
		app->sceneIntro->reset = true;

		app->SaveGameRequest();

		app->audio->PlayMusic(audioPath);

	
		col = false;
	}

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		player->GodMode = false;
		countGod = 0;
		app->sceneIntro->reset = true;
		app->SaveGameRequest();
	}

	if ((app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) || (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)) {
		player->LAV->body->SetLinearVelocity(b2Vec2(0, 0));
		player->camg = false;
		player->lava = false;
		player->GodMode = false;
		countGod = 0;
		app->entityManager->active = false;
		app->map->active = false;
		app->sceneIntro->On = false;
		app->sceneIntro->active = true;
		app->physics->debug = false;
		app->sceneIntro->reset = true;

		if (!app->scene->fly->kill) {
			app->scene->fly->CleanUp();
		}		
		if (!app->scene->walk->kill) {
			app->scene->walk->CleanUp();
		}
		app->scene->fly->col = true;
		app->scene->walk->col = true;
		app->SaveGameRequest();
		this->active = false;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		if (player->dieCount != 1) {
			app->SaveGameRequest();
			app->sceneIntro->reset = false;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
		app->sceneIntro->reset = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) 
	{
		switch (count)
		{
		case 0:
			app->physics->debug = true;
			count++;
			break;
		case 1:
			app->physics->debug = false;
			count = 0;
			break;
		default:
			break;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

		switch (countGod)
		{
		case 0:
			player->GodMode = true;
			countGod++;
			break;
		case 1:

			player->GodMode = false;
			countGod = 0;

			if (player->dieCount == 1) {
				player->DieCounter--;
				player->audiob = true;
				app->LoadGameRequest();
				app->sceneIntro->reset = false;
			}
			break;
		default:
			break;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {

		change = !change;

		if (change) {
			app->physics->fps = 30.0f;
		}
		else {
			app->physics->fps = 60.0f;
		}

	}

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

	// L08: DONE 3: Test World to map method
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
		mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);


	//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	if (originSelected == true)
	//	{
	//		app->pathfinding->CreatePath(origin, mouseTile);
	//		originSelected = false;
	//	}
	//	else
	//	{
	//		origin = mouseTile;
	//		originSelected = true;
	//		app->pathfinding->ClearLastPath();
	//	}
	//}

	/*const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}*/

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);

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
	app->audio->PauseMusic(); 
	return true;
}
