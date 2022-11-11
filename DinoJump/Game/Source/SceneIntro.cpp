#include "SceneIntro.h"
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

SceneIntro::SceneIntro(bool startEnabled) : Module()
{
	name.Create("sceneIntro");

	dinoIntro.PushBack({ 0,0,450,900 }); 
	dinoIntro.PushBack({ 450,0,450,900 });
	dinoIntro.speed = 0.009; 

	dinoDeath.PushBack({ 0,0,450,900 });
	dinoDeath.PushBack({ 455,0,450,900 });
	dinoDeath.speed = 0.009;
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	imgpath = config.child("imgintro").attribute("texturepath").as_string();
	deathpath = config.child("imgintrodeath").attribute("death").as_string();
	audioPathlose = config.child("audio").attribute("pathlose").as_string(); 
	audioPathwin = config.child("audio").attribute("pathwin").as_string(); 

	app->entityManager->active = false;
	app->map->active = false;

	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	img = app->tex->Load(imgpath); 
	death = app->tex->Load(deathpath); 
	audiolose = app->audio->LoadFx(audioPathlose);
	audiowin = app->audio->LoadFx(audioPathwin);

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	bool ret = true;
	/*app->render->DrawTexture(img, 0, 860);
	app->render->DrawTexture(img2, 0, 860);*/
	currentAnimation = &dinoIntro; 
	currentAnimationdeath = &dinoDeath; 
	SDL_Rect dinoI = currentAnimation->GetCurrentFrame(); 
	SDL_Rect dinoD = currentAnimationdeath->GetCurrentFrame(); 

	currentAnimation->Update();
	currentAnimationdeath->Update();

	if (app->scene->player->Meta) {
		app->audio->PlayFx(audiowin);
		app->render->camera.y = -900;
		app->render->DrawTexture(img, 0, 900, &dinoI);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			app->scene->player->die = false;
			app->scene->player->Meta = false;
			this->active = false;
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			On = true;
			reset = true;

			app->SaveGameRequest();
		}
	}

	if (!app->scene->player->die) {
		app->render->DrawTexture(img, 0, 900, &dinoI);
	   
	}
	if (app->scene->player->die) {
		app->render->camera.y = -900; 
		app->render->DrawTexture(death, 0, 900 , &dinoD);
		if (app->scene->player->audiob == true) {
			app->audio->PlayFx(audiolose);
			app->scene->player->audiob = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
		{
			app->scene->player->die = false;
			app->scene->player->Meta = false;
			this->active = false;
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			On = true;
			reset = true;

			app->SaveGameRequest();
		}
	}
	if (!app->scene->player->Meta)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->scene->player->die = false;
			app->scene->player->Meta = false;
			this->active = false;
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			On = true;
			app->LoadGameRequest();
		}
	}

	return ret;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing scene");

	return true;
}