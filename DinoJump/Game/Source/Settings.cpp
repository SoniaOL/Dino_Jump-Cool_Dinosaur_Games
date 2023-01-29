#include "Setting.h"
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
#include "GuiManager.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "SceneCredits.h"
#include "SceneIntro.h"

Settings::Settings() : Module()
{
	name.Create("sceneIntro");


}

// Destructor
Settings::~Settings()
{}

// Called before render is available
bool Settings::Awake(pugi::xml_node& config)
{
	LOG("Loading Settings");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool Settings::Start()
{

	uint w, h;
	app->win->GetWindowSize(w, h);
	if (app->sceneIntro->button == true) {
		button6 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "-", { 135, 280, 20,20 }, (Module*)app->sceneIntro);
		button7 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "+", { 235, 280, 20,20 }, (Module*)app->sceneIntro);
	}
	return true; 
}

// Called each loop iteration
bool Settings::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Settings::Update(float dt)
{
	bool ret = true; 
	app->render->DrawRectangle({ 10, 1140, 425, 420 }, 255, 165, 0, 230, true); 
	return ret;
}

// Called each loop iteration
bool Settings::PostUpdate()
{

	app->render->TextDraw("Settings", 135, 280, 60, 60, 60, 255, 30);

	app->render->TextDraw("Music:", 30, 400, 60, 60, 60, 255, 25);
	app->render->TextDraw("Fx:", 30, 450, 60, 60, 60, 255, 25);
	app->render->TextDraw("Fullscreen:", 30, 500, 60, 60, 60, 255, 25);
	app->render->TextDraw("Vsync:", 30, 550, 60, 60, 60, 255, 25);

	return true;
}

bool Settings::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	
	return true;
}

// Called before quitting
bool Settings::CleanUp()
{

	//app->audio->PauseMusic();

	LOG("Freeing scene");

	return true;
}