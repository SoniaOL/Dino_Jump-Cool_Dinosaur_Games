#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"
#include "GuiButton.h"


struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	bool OnGuiMouseClickEvent(GuiControl* control);

	// Called before quitting
	bool CleanUp();

public:

	bool On = false;
	bool reset = false;
	Player* player;


private:
	const char* imgpath;
	const char* deathpath;
	const char* winpath;
	SDL_Texture* img; 
	SDL_Texture* death; 
	SDL_Texture* win; 

	Animation* currentAnimation = nullptr;
	Animation dinoIntro;

	Animation* currentAnimationdeath = nullptr;
	Animation dinoDeath;

	Animation* currentAnimationwin = nullptr;
	Animation dinoWin;

	const char* audioPathwin;
	const char* audioPathlose;
	int audiolose;
	int audiowin;
	//bool audiob = true; 

	GuiButton* button1;
	GuiButton* button2;
	

	

};

#endif // SCENEINTRO
