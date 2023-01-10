#include "sceneLogo.h"
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

sceneLogo::sceneLogo() : Module()
{
	name.Create("sceneLogo");

}

// Destructor
sceneLogo::~sceneLogo()
{}

// Called before render is available
bool sceneLogo::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	imgpath = config.child("imglogo").attribute("texturepath").as_string();

	return ret;
}

// Called before the first frame
bool sceneLogo::Start()
{

	img = app->tex->Load(imgpath);
	app->sceneIntro->active = false; 

	return true;
}

// Called each loop iteration
bool sceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool sceneLogo::Update(float dt)
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 900);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		/*app->sceneIntro->active = active;
		app->scenelogo->active = false;*/

		app->fade->FadeToBlack(this, (Module*)app->sceneIntro, 90);
	}
	
	return ret;
}

// Called each loop iteration
bool sceneLogo::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool sceneLogo::CleanUp()
{
	app->tex->UnLoad(img); 
	LOG("Freeing scene");

	return true;
}