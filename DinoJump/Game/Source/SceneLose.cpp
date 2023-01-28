#include "sceneLose.h"
#include "sceneIntro.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "ModuleFadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"
#include "Scene.h"
#include "Defs.h"
#include "Log.h"

SceneLose::SceneLose() : Module()
{
	name.Create("sceneIntro");

	dinoDeath.PushBack({ 0,0,450,900 });
	dinoDeath.PushBack({ 455,0,450,900 });
	dinoDeath.speed = 0.009;

	

}

// Destructor
SceneLose::~SceneLose()
{}

// Called before render is available
bool SceneLose::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	deathpath = config.child("imgintrodeath").attribute("death").as_string();
	audioPathlose = config.child("audio").attribute("pathlose").as_string();

	return ret;
}

// Called before the first frame
bool SceneLose::Start()
{
	death = app->tex->Load(deathpath);
	audiolose = app->audio->LoadFx(audioPathlose);

	return true;
}

// Called each loop iteration
bool SceneLose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLose::Update(float dt)
{
  	bool ret = true;

	currentAnimationdeath = &dinoDeath;

	SDL_Rect dinoD = currentAnimationdeath->GetCurrentFrame();

	currentAnimationdeath->Update();

	if (app->scene->player->die) {
		app->render->camera.y = -900;
		app->render->DrawTexture(death, 0, 900, &dinoD);
		if (app->scene->player->audiob == true) {
			app->audio->PlayFx(audiolose);
			app->scene->player->audiob = false;
		}

		app->scene->timer->Start();
		app->scene->ts = 0;

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			app->scene->player->die = false;
			app->scene->item->restart = true;
			app->scene->player->lives = true;
			app->scene->player->Coins = 0;
			app->scene->player->DieCounter = 3;
			app->scene->player->Meta = false;
			this->active = false;
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			app->sceneIntro->On = true;
			app->sceneIntro->reset = true;
			app->scene->player->check1 = false;
			app->scene->player->check2 = false;
			app->sceneIntro->bluefire = false; 
			app->scene->col = true; 

			app->SaveGameRequest();
		}
	}

	//if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

	//	/*app->sceneIntro->active = active;
	//	app->scenelogo->active = false;*/

	//	app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 90);
	//}

	return ret;
}

// Called each loop iteration
bool SceneLose::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneLose::CleanUp()
{
	LOG("Freeing scene");
	//app->tex->UnLoad(death);
	return true;
}