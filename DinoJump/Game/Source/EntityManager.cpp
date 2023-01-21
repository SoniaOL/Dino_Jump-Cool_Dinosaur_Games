#include "EntityManager.h"
#include "Player.h"
#include "FlyEnemy.h"
#include "WalkEnemy.h"

//#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Physics.h"
#include "SceneIntro.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

		for (item = entities.start; item != NULL && ret == true; item = item->next)
		{
			pEntity = item->data;

			if (pEntity->active == false) continue;
			ret = item->data->Awake();
		}
	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::FLYENEMY:
		entity = new FlyEnemy(); 
		break; 

	case EntityType::WALKENEMY:
		entity = new WalkEnemy();
		break;

	case EntityType::ITEM:
		entity = new Item();
		break;

	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update();
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	app->scene->player->lava = false;
	app->scene->player->camg = false;

	PosX = data.child("CAMG").attribute("x").as_int();
	PosY = data.child("CAMG").attribute("y").as_int();

	app->scene->player->CAMG->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("CAMGDETECT").attribute("x").as_int();
	PosY = data.child("CAMGDETECT").attribute("y").as_int();

	app->scene->player->CAMGDetect->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("CAM").attribute("x").as_int();
	PosY = data.child("CAM").attribute("y").as_int();

	app->scene->player->CAM->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("player").attribute("x").as_int();
	PosY = data.child("player").attribute("y").as_int();

	app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);
	
	PosX = data.child("LAVDetect").attribute("x").as_int();
	PosY = data.child("LAVDetect").attribute("y").as_int();

	app->scene->player->LAVDetect->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("Life1").attribute("x").as_int();
	PosY = data.child("Life1").attribute("y").as_int();

	app->scene->player->lifeT1->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("Life2").attribute("x").as_int();
	PosY = data.child("Life2").attribute("y").as_int();

	app->scene->player->lifeT2->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("Life3").attribute("x").as_int();
	PosY = data.child("Life3").attribute("y").as_int();

	app->scene->player->lifeT3->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("Life4").attribute("x").as_int();
	PosY = data.child("Life4").attribute("y").as_int();

	app->scene->player->lifeT4->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("Life5").attribute("x").as_int();
	PosY = data.child("Life5").attribute("y").as_int();

	app->scene->player->lifeT5->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("item4").attribute("x").as_int();
	PosY = data.child("item4").attribute("y").as_int();

	app->scene->player->Coin4->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		PosX = data.child("LAV").attribute("x").as_int();
		PosY = data.child("LAV").attribute("y").as_int();

		app->scene->player->LAV->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		PosX = data.child("item").attribute("x").as_int();
		PosY = data.child("item").attribute("y").as_int();

		app->scene->player->Coin1->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		PosX = data.child("item2").attribute("x").as_int();
		PosY = data.child("item2").attribute("y").as_int();

		app->scene->player->Coin2->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		PosX = data.child("item3").attribute("x").as_int();
		PosY = data.child("item3").attribute("y").as_int();

		app->scene->player->Coin3->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		PosX = data.child("Check1").attribute("x").as_int();
		PosY = data.child("Check1").attribute("y").as_int();

		app->scene->player->CHECK->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		PosX = data.child("Check2").attribute("x").as_int();
		PosY = data.child("Check2").attribute("y").as_int();

		app->scene->player->CHECK1->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

		if (!app->scene->fly->isDead)
		{

			PosX = data.child("FLYENEMY").attribute("x").as_int();
			PosY = data.child("FLYENEMY").attribute("y").as_int();

			app->scene->fly->pbody->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);
		}

		if (app->sceneIntro->reset == true)
		{
			if (app->scene->fly->isDead)
			{
				app->scene->fly->col = true;
			}
		}

		
				if (flyLive == 0)
				{
					if (app->scene->fly->isDead)
					{
						app->scene->fly->col = true;
					}
				}
		


		if (!app->scene->walk->isDead) 
		{
			PosX = data.child("WALKENEMY").attribute("x").as_int();
			PosY = data.child("WALKENEMY").attribute("y").as_int();

			app->scene->walk->position.x = PosX;
			app->scene->walk->position.y = PosY;

			app->scene->walk->pbody->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);
		}

		if (app->sceneIntro->reset == true)
		{
			if (app->scene->walk->isDead)
			{
				app->scene->walk->col = true;
			}
		}

	
				if (walkLive == 0)
				{
					if (app->scene->walk->isDead)
					{
						app->scene->walk->col = true;
					}
				}
		
		
		app->sceneIntro->reset = false;

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");
	pugi::xml_node lav = data.append_child("LAV");
	pugi::xml_node lavDetect = data.append_child("LAVDetect");
	pugi::xml_node cam = data.append_child("CAM");
	pugi::xml_node camg = data.append_child("CAMG");
	pugi::xml_node camgD = data.append_child("CAMGDETECT");
	pugi::xml_node flyEnemy = data.append_child("FLYENEMY");
	pugi::xml_node walkEnemy = data.append_child("WALKENEMY");
	pugi::xml_node Life1 = data.append_child("Life1");
	pugi::xml_node Life2 = data.append_child("Life2");
	pugi::xml_node Life3 = data.append_child("Life3");
	pugi::xml_node Life4 = data.append_child("Life4");
	pugi::xml_node Life5 = data.append_child("Life5");
	pugi::xml_node Item = data.append_child("item");
	pugi::xml_node Item2 = data.append_child("item2");
	pugi::xml_node Item3 = data.append_child("item3");
	pugi::xml_node Item4 = data.append_child("item4");
	pugi::xml_node Check1 = data.append_child("Check1");
	pugi::xml_node Check2 = data.append_child("Check2");


	if (!app->sceneIntro->reset) 
	{
		if (!app->sceneIntro->checkpoint1)
		{
			if (!app->sceneIntro->checkpoint2) 
			{
				play.append_attribute("x") = app->scene->player->position.x;
				play.append_attribute("y") = app->scene->player->position.y;

				lav.append_attribute("x") = app->scene->player->lavaPosX;
				lav.append_attribute("y") = app->scene->player->lavaPosY;

				lavDetect.append_attribute("x") = app->scene->player->DetectPosX;
				lavDetect.append_attribute("y") = app->scene->player->DetectPosY;

				cam.append_attribute("x") = app->scene->player->camPosX;
				cam.append_attribute("y") = app->scene->player->camPosY;

				camg.append_attribute("x") = app->scene->player->DetectPosX;
				camg.append_attribute("y") = app->scene->player->DetectPosY - 15;

				camgD.append_attribute("x") = app->scene->player->camPosX;
				camgD.append_attribute("y") = app->scene->player->camPosY + 50;

				flyEnemy.append_attribute("x") = app->scene->fly->position.x;
				flyEnemy.append_attribute("y") = app->scene->fly->position.y;

				walkEnemy.append_attribute("x") = app->scene->walk->position.x;
				walkEnemy.append_attribute("y") = app->scene->walk->position.y;

				Life1.append_attribute("x") = app->scene->player->lifeT1X;
				Life1.append_attribute("y") = app->scene->player->lifeT1Y;

				Life2.append_attribute("x") = app->scene->player->lifeT2X;
				Life2.append_attribute("y") = app->scene->player->lifeT2Y;

				Life3.append_attribute("x") = app->scene->player->lifeT3X;
				Life3.append_attribute("y") = app->scene->player->lifeT3Y;

				Life4.append_attribute("x") = app->scene->player->lifeT4X;
				Life4.append_attribute("y") = app->scene->player->lifeT4Y;

				Life5.append_attribute("x") = app->scene->player->lifeT5X;
				Life5.append_attribute("y") = app->scene->player->lifeT5Y;

				Item.append_attribute("x") = app->scene->player->CoinX1;
				Item.append_attribute("y") = app->scene->player->CoinY1;

				Item2.append_attribute("x") = app->scene->player->CoinX2;
				Item2.append_attribute("y") = app->scene->player->CoinY2;

				Item3.append_attribute("x") = app->scene->player->CoinX3;
				Item3.append_attribute("y") = app->scene->player->CoinY3;

				Item4.append_attribute("x") = app->scene->player->CoinX4;
				Item4.append_attribute("y") = app->scene->player->CoinY4;

				Check1.append_attribute("x") = app->scene->player->Check1X;
				Check1.append_attribute("y") = app->scene->player->Check1Y;

				Check2.append_attribute("x") = app->scene->player->Check2X;
				Check2.append_attribute("y") = app->scene->player->Check2Y;

				if (!app->scene->fly->isDead) {
					flyLive = 0;
				}

				if (app->scene->fly->isDead) {
					flyLive = 1;
				}

				if (!app->scene->walk->isDead) {
					walkLive = 0;
				}

				if (app->scene->walk->isDead) {
					walkLive = 1;
				}
			}
		}
	}

	if (app->sceneIntro->checkpoint1) 
	{
		app->render->camera.y = -618;

		app->scene->player->lava = false;

		lav.append_attribute("x") = app->scene->player->lavaPosX;
		lav.append_attribute("y") = app->scene->player->lavaPosY;

		if (!app->scene->fly->isDead)
		{
			flyEnemy.append_attribute("x") = app->scene->fly->position.x;
			flyEnemy.append_attribute("y") = app->scene->fly->position.y;
		}

		if (!app->scene->walk->isDead)
		{
			walkEnemy.append_attribute("x") = app->scene->walk->position.x;
			walkEnemy.append_attribute("y") = app->scene->walk->position.y;
		}
		Item.append_attribute("x") = app->scene->player->CoinX1;
		Item.append_attribute("y") = app->scene->player->CoinY1;

		Item2.append_attribute("x") = app->scene->player->CoinX2;
		Item2.append_attribute("y") = app->scene->player->CoinY2;

		Item3.append_attribute("x") = app->scene->player->CoinX3;
		Item3.append_attribute("y") = app->scene->player->CoinY3;

		if (app->scene->player->playerPos == false)
		{
			play.append_attribute("x") = 125;
			play.append_attribute("y") = 895;
		}

		if (app->scene->player->playerPos == true)
		{
			play.append_attribute("x") = app->scene->player->position.x;
			play.append_attribute("y") = app->scene->player->position.y;

			app->scene->player->playerPos = false;
			
		}

		lavDetect.append_attribute("x") = 37;
		lavDetect.append_attribute("y") = 1485;

		cam.append_attribute("x") = 37;
		cam.append_attribute("y") = 1128;

		camg.append_attribute("x") = 37;
		camg.append_attribute("y") = 1500;

		camgD.append_attribute("x") = 37;
		camgD.append_attribute("y") = 1138;

		Life1.append_attribute("x") = 400;
		Life1.append_attribute("y") = 665;

		Life2.append_attribute("x") = 380;
		Life2.append_attribute("y") = 665;

		Life3.append_attribute("x") = 360;
		Life3.append_attribute("y") = 665;

		Life4.append_attribute("x") = 340;
		Life4.append_attribute("y") = 665;

		Life5.append_attribute("x") = 320;
		Life5.append_attribute("y") = 665;

		Item4.append_attribute("x") = 365;
		Item4.append_attribute("y") = 695;

		Check1.append_attribute("x") = app->scene->player->Check1X;
		Check1.append_attribute("y") = app->scene->player->Check1Y;

		Check2.append_attribute("x") = app->scene->player->Check2X;
		Check2.append_attribute("y") = app->scene->player->Check2Y;

		if (!app->scene->fly->isDead) {
			flyLive = 0;
		}

		if (app->scene->fly->isDead) {
			flyLive = 1;
		}

		if (!app->scene->walk->isDead) {
			walkLive = 0;
		}

		if (app->scene->walk->isDead) {
			walkLive = 1;
		}

		app->sceneIntro->checkpoint1 = false;

		app->sceneIntro->bluefire2 = true; 
		
		app->LoadGameRequest();

	}

	if (app->sceneIntro->checkpoint2)
	{
		app->render->camera.y = -618;

		lav.append_attribute("x") = app->scene->player->lavaPosX;
		lav.append_attribute("y") = app->scene->player->lavaPosY;

		if (!app->scene->fly->isDead)
		{
			flyEnemy.append_attribute("x") = app->scene->fly->position.x;
			flyEnemy.append_attribute("y") = app->scene->fly->position.y;
		}

		if (!app->scene->walk->isDead)
		{
			walkEnemy.append_attribute("x") = app->scene->walk->position.x;
			walkEnemy.append_attribute("y") = app->scene->walk->position.y;
		}
		Item.append_attribute("x") = app->scene->player->CoinX1;
		Item.append_attribute("y") = app->scene->player->CoinY1;

		Item2.append_attribute("x") = app->scene->player->CoinX2;
		Item2.append_attribute("y") = app->scene->player->CoinY2;

		Item3.append_attribute("x") = app->scene->player->CoinX3;
		Item3.append_attribute("y") = app->scene->player->CoinY3;

		if (app->scene->player->playerPos == false)
		{
			play.append_attribute("x") = 80;
			play.append_attribute("y") = 1245;
		}

		if (app->scene->player->playerPos == true)
		{
			play.append_attribute("x") = app->scene->player->position.x;
			play.append_attribute("y") = app->scene->player->position.y;

			app->scene->player->playerPos = false;
		}

		lavDetect.append_attribute("x") = 37;
		lavDetect.append_attribute("y") = 1385;

		cam.append_attribute("x") = 37;
		cam.append_attribute("y") = 1028;

		camg.append_attribute("x") = 37;
		camg.append_attribute("y") = 1400;

		camgD.append_attribute("x") = 37;
		camgD.append_attribute("y") = 1038;

		Life1.append_attribute("x") = 400;
		Life1.append_attribute("y") = 665;

		Life2.append_attribute("x") = 380;
		Life2.append_attribute("y") = 665;

		Life3.append_attribute("x") = 360;
		Life3.append_attribute("y") = 665;

		Life4.append_attribute("x") = 340;
		Life4.append_attribute("y") = 665;

		Life5.append_attribute("x") = 320;
		Life5.append_attribute("y") = 665;

		Item4.append_attribute("x") = 365;
		Item4.append_attribute("y") = 695;

		Check1.append_attribute("x") = app->scene->player->Check1X;
		Check1.append_attribute("y") = app->scene->player->Check1Y;

		Check2.append_attribute("x") = app->scene->player->Check2X;
		Check2.append_attribute("y") = app->scene->player->Check2Y;
		
		if (!app->scene->fly->isDead) {
			flyLive = 0;
		}

		if (app->scene->fly->isDead) {
			flyLive = 1;
		}

		if (!app->scene->walk->isDead) {
			walkLive = 0;
		}

		if (app->scene->walk->isDead) {
			walkLive = 1;
		}

		app->sceneIntro->checkpoint2 = false;

		app->sceneIntro->bluefire = true;

		app->LoadGameRequest();

	}

	if (app->sceneIntro->reset) 
	{

		play.append_attribute("x") = 176;
		play.append_attribute("y") = 1745;

		lav.append_attribute("x") = 37;
		lav.append_attribute("y") = 1842;

		lavDetect.append_attribute("x") = 37;
		lavDetect.append_attribute("y") = 1770;

		cam.append_attribute("x") = 37;
		cam.append_attribute("y") = 1412;

		camg.append_attribute("x") = 37;
		camg.append_attribute("y") = 1785;

		camgD.append_attribute("x") = 37;
		camgD.append_attribute("y") = 1462;

		flyEnemy.append_attribute("x") = 348;
		flyEnemy.append_attribute("y") = 1356;

		if (!app->scene->fly->isDead) {
			flyLive = 0;
		}

		if (app->scene->fly->isDead) {
			flyLive = 1;
		}

		if (!app->scene->walk->isDead) {
			walkLive = 0;
		}

		if (app->scene->walk->isDead) {
			walkLive = 1;
		}

		walkEnemy.append_attribute("x") = 228;
		walkEnemy.append_attribute("y") = 1090;
		
		Life1.append_attribute("x") = 400;
		Life1.append_attribute("y") = 950;
		
		Life2.append_attribute("x") = 380;
		Life2.append_attribute("y") = 950;
		
		Life3.append_attribute("x") = 360;
		Life3.append_attribute("y") = 950;
		
		Life4.append_attribute("x") = 340;
		Life4.append_attribute("y") = 950;
		
		Life5.append_attribute("x") = 320;
		Life5.append_attribute("y") = 950;
		
		Item.append_attribute("x") = 300;
		Item.append_attribute("y") = 1600;
		
		Item2.append_attribute("x") = 200;
		Item2.append_attribute("y") = 1400;
		
		Item3.append_attribute("x") = 350;
		Item3.append_attribute("y") = 1150;
		
		Item4.append_attribute("x") = 365;
		Item4.append_attribute("y") = 980;

		Check1.append_attribute("x") = 150;
		Check1.append_attribute("y") = 890;

		Check2.append_attribute("x") = 70;
		Check2.append_attribute("y") = 1250;

		app->render->SaveState(data);
		app->render->LoadState(data);
		app->LoadGameRequest();

		LOG("RESET");
	}

	return true;
}