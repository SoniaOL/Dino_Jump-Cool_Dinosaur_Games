#include "SceneWin.h"
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

SceneWin::SceneWin() : Module()
{
	name.Create("sceneIntro");

	dinoWin.PushBack({ 0,0,450,900 });
	dinoWin.PushBack({ 453,0,450,900 });
	dinoWin.speed = 0.009;
}

// Destructor
SceneWin::~SceneWin()
{}

// Called before render is available
bool SceneWin::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	winpath = config.child("imgintrodeath").attribute("win").as_string();
	audioPathwin = config.child("audio").attribute("pathwin").as_string();

	return ret;
}

// Called before the first frame
bool SceneWin::Start()
{
	if (init) {
		win = app->tex->Load(winpath);
		audiowin = app->audio->LoadFx(audioPathwin);
		init = false;
	}

	return true;
}

// Called each loop iteration
bool SceneWin::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneWin::Update(float dt)
{
	bool ret = true;

	currentAnimationwin = &dinoWin;
	SDL_Rect dinoW = currentAnimationwin->GetCurrentFrame();
	currentAnimationwin->Update();

	if (app->scene->player->Meta) {
		if (app->scene->player->audiow == true) {
			app->audio->PlayFx(audiowin);
			app->scene->player->audiow = false;
		}
		app->render->camera.y = -900;
		app->render->DrawTexture(win, 0, 900, &dinoW);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			app->scene->player->Meta = false;
			app->scene->player->lives = true;
			app->scene->col = true;
			app->scene->player->Coins = 0;
			/*app->scene->player->check1 = false;
			app->scene->player->check2 = false;*/
			app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 40);
		}
	}

	return ret;
}

// Called each loop iteration
bool SceneWin::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneWin::CleanUp()
{
	//app->tex->UnLoad(win);
	LOG("Freeing scene");

	return true;
}