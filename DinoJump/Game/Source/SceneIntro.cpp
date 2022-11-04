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
	app->render->DrawTexture(img, 0, 860);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		this->active = false; 
		app->scene->active = true;
		app->entityManager->active = true; 
		app->map->active = true;
		On = true;
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