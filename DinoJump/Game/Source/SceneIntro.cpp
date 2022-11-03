#include "SceneIntro.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "ModuleFadeToBlack.h"

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
	app->render->DrawTexture(img, -150, 700);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		app->fade->FadeToBlack(this, (Module*)app->scene, 90); 
	}



	return true;
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