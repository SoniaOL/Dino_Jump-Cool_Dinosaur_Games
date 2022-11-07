#include "EntityManager.h"
#include "Player.h"

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

	//case EntityType::ITEM:
	//	entity = new Item();
	//	break;

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

	PosX = data.child("player").attribute("x").as_int();
	PosY = data.child("player").attribute("y").as_int();

	app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("CAM").attribute("x").as_int();
	PosY = data.child("CAM").attribute("y").as_int();

	app->scene->player->CAM->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("LAVDetect").attribute("x").as_int();
	PosY = data.child("LAVDetect").attribute("y").as_int();

	app->scene->player->LAVDetect->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("LAV").attribute("x").as_int();
	PosY = data.child("LAV").attribute("y").as_int();

	app->scene->player->LAV->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("CAMG").attribute("x").as_int();
	PosY = data.child("CAMG").attribute("y").as_int();

	app->scene->player->CAMG->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

	PosX = data.child("CAMGDETECT").attribute("x").as_int();
	PosY = data.child("CAMGDETECT").attribute("y").as_int();

	app->scene->player->CAMGDetect->body->SetTransform({ PIXEL_TO_METERS(PosX), PIXEL_TO_METERS(PosY) }, 0);

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


	if (!app->sceneIntro->reset) {
		
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
	}

	if (app->sceneIntro->reset) {
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

		app->render->SaveState(data);
		app->render->LoadState(data);
		app->LoadGameRequest();

		LOG("RESET");

		app->sceneIntro->reset = false;
	}

	return true;
}