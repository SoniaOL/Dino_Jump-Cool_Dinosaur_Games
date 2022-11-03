#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

ModuleFadeToBlack::ModuleFadeToBlack() : Module()
{
	
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Start(pugi::xml_node& config)
{
	LOG("Preparing Fade Screen");

	width = config.child("resolution").attribute("width").as_int(); //get from config 
	height = config.child("resolution").attribute("height").as_int();; //get from config 
	scale = config.child("resolution").attribute("scale").as_int();; //get from config 

	screenRect = { 0, 0, width * scale, height * scale };

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::Update()
{
	bool ret = true; 
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return ret;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			// TODO 1: Enable / disable the modules received when FadeToBlacks() gets called
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return ret;
}

bool ModuleFadeToBlack::PostUpdate()
{
	bool ret = true;
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return ret;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return ret;
}

bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if(currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		// TODO 1: We need to keep track of the modules received in FadeToBlack(...)
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}