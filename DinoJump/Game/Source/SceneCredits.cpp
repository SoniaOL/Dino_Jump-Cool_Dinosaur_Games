#include "SceneCredits.h"
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

SceneCredit::SceneCredit() : Module()
{
	name.Create("sceneIntro");
}

// Destructor
SceneCredit::~SceneCredit()
{}

// Called before render is available
bool SceneCredit::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	creditpath = config.child("credits").attribute("path").as_string();
	creditsong = config.child("credits").attribute("audio").as_string();

	return ret;
}

// Called before the first frame
bool SceneCredit::Start()
{
	credit = app->tex->Load(creditpath);
	col = true; 
	return true;
}

// Called each loop iteration
bool SceneCredit::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneCredit::Update(float dt)
{
	bool ret = true;

	if (col == true)
	{
		app->audio->PlayMusic(creditsong, 0.0f);
		
		col = false;
	}

	app->render->DrawTexture(credit, 0, 900);

		if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 40);
		}

	return ret;
}

// Called each loop iteration
bool SceneCredit::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
	{

		ret = false;
	}

	return ret;
}

// Called before quitting
bool SceneCredit::CleanUp()
{
	//app->tex->UnLoad(credit);
	app->audio->PauseMusic();

	return true;
}