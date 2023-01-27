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

	fireloop.PushBack({ 2,0,21,32 }); 
	fireloop.PushBack({ 26,0,21,32 }); 
	fireloop.PushBack({ 50,0,21,32 }); 
	fireloop.PushBack({ 74,0,21,32 }); 
	fireloop.PushBack({ 98,0,21,32 }); 
	fireloop.PushBack({ 122,0,21,32 }); 
	fireloop.PushBack({ 146,0,21,32 }); 
	fireloop.PushBack({ 170,0,21,32 }); 
	fireloop.speed = 0.2f; 

	//fireloop.PushBack({ 98,32,21,32 });
	//fireloop.PushBack({ 122,32,21,32 });
	//fireloop.PushBack({ 146,32,21,32 });
	//fireloop.PushBack({ 170,32,21,32 });

	blueStart.PushBack({ 2,64,21,32 });
	blueStart.PushBack({ 26,64,21,32 });
	blueStart.PushBack({ 50,64,21,32 });
	blueStart.PushBack({ 74,64,21,32 });
	blueStart.PushBack({ 98,64,21,32 });
	blueStart.PushBack({ 122,64,21,32 });
	blueStart.PushBack({ 146,64,21,32 });
	blueStart.PushBack({ 170,64,21,32 });
	blueStart.speed = 0.2f; 

	

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
	
	item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	item->parameters = config.child("item");


	audioPath = config.child("player").child("audio").attribute("path").as_string();
	pathFire = config.child("player").child("fire").attribute("texturepath").as_string();
	audioPathfire = config.child("player").child("fire").attribute("sound").as_string();

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

	if (init)
	{
		fire = app->tex->Load(pathFire);
		fire2 = app->tex->Load(pathFire);
		init = false;
	}

	audiofire = app->audio->LoadFx(audioPathfire);
	firesound = true;
	firesound2 = true;

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
		/*if (app->scene->fly->isDead) {
			app->scene->fly->col = true;
		}
		if (app->scene->walk->isDead) {
			app->scene->walk->col = true;
		}*/
		app->map->active = false;
		app->sceneIntro->On = false;
	/*	app->sceneIntro->active = true;*/
		app->physics->debug = false;
		//app->SaveGameRequest();
		/*this->active = false;*/
		app->fade->FadeToBlack(this, (Module*)app->sceneWin, 20);
	}

	if (col)
	{
		app->scene->player->die = false;
		app->scene->player->DieCounter = 3;
		app->scene->player->Meta = false;
		app->entityManager->active = true;
		app->map->active = true;
		app->sceneIntro->On = true;

		if (app->scene->player->check1 == true) 
		{
			app->LoadFromFile();
		}		
		
		if (app->scene->player->check2 == true) 
		{
			app->LoadFromFile();
		}

		if (app->scene->player->check1 == false)
		{
			if (app->scene->player->check2 == false)
			{
				app->sceneIntro->bluefire = false;
				app->sceneIntro->bluefire2 = false;
				app->sceneIntro->reset = true;
				app->SaveGameRequest();
			}
		}

		app->audio->PlayMusic(audioPath, 0.0f);
	
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
		/*app->scene->player->check1 = false;
		app->scene->player->check2 = false;*/

		if (!app->scene->fly->kill) {
			app->scene->fly->CleanUp();
		}		
		if (!app->scene->walk->kill) {
			app->scene->walk->CleanUp();
		}
		this->active = false;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		if (player->dieCount != 1) {
			app->sceneIntro->reset = false;
			app->SaveGameRequest();
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

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		app->sceneIntro->checkpoint1 = true;
		app->sceneIntro->reset = false;
		app->SaveGameRequest();
	}	
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_UP) 
	{
		app->LoadGameRequest();
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		app->sceneIntro->checkpoint2 = true;
		app->sceneIntro->reset = false;
		app->SaveGameRequest();
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_UP)
	{
		app->LoadGameRequest();
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

	if (app->sceneIntro->bluefire == false) {
		currentAnimation = &fireloop;
	}
	if (app->sceneIntro->bluefire2 == false) {
		currentAnimation2 = &fireloop;
	}

	if (app->sceneIntro->bluefire == true) {
		currentAnimation = &blueStart;
		if (firesound == true) {
			app->audio->PlayFx(audiofire);
			firesound = false; 
		}
	}

	if (app->sceneIntro->bluefire2 == true) {
		currentAnimation2 = &blueStart;
		if (firesound2 == true) {
			app->audio->PlayFx(audiofire);
			firesound2 = false;
		}

	}

	SDL_Rect fireplace = currentAnimation->GetCurrentFrame(); 
	SDL_Rect fireplace2 = currentAnimation2->GetCurrentFrame(); 
 
	currentAnimation->Update(); 
	currentAnimation2->Update(); 

	app->render->DrawTexture(fire, 59, 1246, &fireplace); 
	app->render->DrawTexture(fire2, 146, 886, &fireplace2); 

	


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

	//const char* coins = app->scene->player->Coins;
	
	//LOG("coins: %c", coins);

	std::string c = std::to_string(app->scene->player->Coins);
	char const* coins = c.c_str();

	app->render->TextDraw(coins, 390, 75, 80, 157, 0, 255, 12);

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
