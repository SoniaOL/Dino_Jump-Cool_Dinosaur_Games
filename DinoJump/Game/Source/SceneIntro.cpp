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
#include "GuiManager.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "SceneCredits.h"
#include "Setting.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("sceneIntro");

	dinoIntro.PushBack({ 0,0,450,900 }); 
	dinoIntro.PushBack({ 450,0,450,900 });
	dinoIntro.speed = 0.009; 

	//dinoDeath.PushBack({ 0,0,450,900 });
	//dinoDeath.PushBack({ 455,0,450,900 });
	//dinoDeath.speed = 0.009;

	//dinoWin.PushBack({ 0,0,450,900 });
	//dinoWin.PushBack({ 453,0,450,900 });
	//dinoWin.speed = 0.009;
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
	audioPathintro = config.child("audio").attribute("pathintro").as_string();


	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;
	app->scenecredit->active = false;
	app->sceneLose->active = false; 
	app->sceneWin->active = false; 

	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{
	start = true; 

	volume = 20; 

	uint w, h;
	app->win->GetWindowSize(w, h);

	if (this->active == true)
	{
		if (init) {
			img = app->tex->Load(imgpath);
			button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", { (int)w / 2 - 85 / 2,700,85,20 }, this);
			button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { (int)w / 2 - 125 / 2,730,125,20 }, this);
			button3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { (int)w / 2 - 125 / 2,760,125,20 }, this);
			button4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Exit", { (int)w / 2 - 60 / 2,790,60,20 }, this);
			button5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Credits", { (int)w / 2 - 113 / 2,820,113,20 }, this);
			init = false;
		}
	}
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
	if (start == true) {
		app->audio->PlayMusic(audioPathintro, 0.0f);
		start = false; 
	}
	bool ret = true;
	/*app->render->DrawTexture(img, 0, 860);
	app->render->DrawTexture(img2, 0, 860);*/
	currentAnimation = &dinoIntro; 
	/*currentAnimationdeath = &dinoDeath; */
	/*currentAnimationwin = &dinoWin; */
	SDL_Rect dinoI = currentAnimation->GetCurrentFrame(); 
	//SDL_Rect dinoD = currentAnimationdeath->GetCurrentFrame();
	//SDL_Rect dinoW = currentAnimationwin->GetCurrentFrame();

	currentAnimation->Update();
	//currentAnimationdeath->Update();
	//currentAnimationwin->Update();

	if (!app->scene->player->die) {
		app->render->DrawTexture(img, 0, 900, &dinoI);
	}

	//if (app->scene->player->Meta) {
	//	if (app->scene->player->audiow == true) {
	//		app->audio->PlayFx(audiowin);
	//		app->scene->player->audiow = false;
	//	}
	//	app->render->camera.y = -900;
	//	app->render->DrawTexture(win, 0, 900, &dinoW);

	//	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//	{
	//		app->scene->player->Meta = false;
	//	}
	//}

	/*if (!app->scene->player->die) {
		app->render->DrawTexture(img, 0, 900, &dinoI);
	   
	}*/

	//if (app->scene->player->die) {
	//	app->render->camera.y = -900; 
	//	app->render->DrawTexture(death, 0, 900 , &dinoD);
	//	if (app->scene->player->audiob == true) {
	//		app->audio->PlayFx(audiolose);
	//		app->scene->player->audiob = false;
	//	}
	//	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	//	{
	//		app->scene->player->die = false;
	//		app->scene->player->DieCounter = 0;
	//		app->scene->player->Meta = false;
	//		this->active = false;
	//		app->scene->active = true;
	//		app->entityManager->active = true;
	//		app->map->active = true;
	//		On = true;
	//		reset = true;

	//		app->SaveGameRequest();
	//	}
	//}
	if (!app->scene->player->Meta)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->scene->player->die = false;
			app->scene->player->DieCounter = 3;
			app->scene->player->Meta = false;
			this->active = false;
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			On = true;
			if (app->scene->fly->isDead) {
				app->scene->fly->col = true;

			}
			if (app->scene->walk->isDead) {
				app->scene->walk->col = true;
			}
			//app->LoadGameRequest();
		}
	}

	//if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) 
	//{
	//	app->scenecredit->col = true;
	//	app->fade->FadeToBlack(this, (Module*)app->scenecredit, 20);
	//}
	app->guiManager->Draw();


	return ret;
}
 
// Called each loop iteration
bool SceneIntro::PostUpdate()
{

	/*app->render->TextDraw("START", 170, 300, 255, 157, 0, 255, 25);*/
	bool ret = exit;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	return ret;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		app->scene->col = true;
		Continue = false;
		app->scene->item->restart = true;
		app->scene->player->Coins = 0;
		app->scene->player->lives = true;
		app->scene->player->DieCounter = 3;
		app->scene->player->check1 = false;
		app->scene->player->check2 = false;
		app->scene->timer->Start();
		app->scene->ts = 0;
		reset = true;
		app->sceneIntro->bluefire = false;
		app->sceneIntro->bluefire2 = false;
		app->fade->FadeToBlack(this, (Module*)app->scene, 20);
		app->settings->active = false;

		break;
	case 2:
		LOG("Button 2 click");

		if (app->scene->player->check1) {

			app->scene->col = true;
			Continue = true;
			app->scene->player->Coins = 0;
			app->scene->player->lives = true;
			app->scene->player->DieCounter = 3;
			app->sceneIntro->bluefire = false;
			app->sceneIntro->bluefire2 = true;

			app->scene->player->CheckOut1 = false;
			app->scene->player->CheckOut2 = false;
			app->fade->FadeToBlack(this, (Module*)app->scene, 20);
			app->scene->player->CheckOut1 = false;
			app->scene->player->CheckOut2 = false;
		}
		if (app->scene->player->check2) {

			app->scene->col = true;
			Continue = true;
			app->scene->player->Coins = 0;
			app->scene->player->lives = true;
			app->scene->player->DieCounter = 3;
			app->sceneIntro->bluefire = true;
			app->sceneIntro->bluefire2 = false;

			app->scene->player->CheckOut1 = false;
			app->scene->player->CheckOut2 = false;
			app->fade->FadeToBlack(this, (Module*)app->scene, 20);
			app->scene->player->CheckOut1 = false;
			app->scene->player->CheckOut2 = false;
		}

		break;
	case 3:
		LOG("Button 3 click");
		app->settings->active = true;
		button = true; 
		break;
	case 4:
		LOG("Button 4 click");
		exit = false;
		break;

	case 5:
		LOG("Button 5 click");
		app->scenecredit->col = true;
		app->fade->FadeToBlack(this, (Module*)app->scenecredit, 20);
		break;

	case 6:
		volume--;
		maxvolume = volume * SDL_MIX_MAXVOLUME / 60;
		app->audio->ChangeMusicVolume(maxvolume);
	
     case 7:
	     volume++;
		 maxvolume = volume * SDL_MIX_MAXVOLUME / 60;
		 app->audio->ChangeMusicVolume(maxvolume);
	}


	return true;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	//app->tex->UnLoad(img);
	app->audio->PauseMusic(); 
	//app->tex->UnLoad(death);
	/*app->tex->UnLoad(win);*/

	LOG("Freeing scene");

	return true;
}