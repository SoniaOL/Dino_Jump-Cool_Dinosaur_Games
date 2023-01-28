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
	bool checkpoint1 = false;
	bool checkpoint2 = false;
	bool bluefire = false; 
	bool bluefire2 = false; 
	bool init = true; 

	Player* player;

	bool Continue = false;

private:
	const char* imgpath;
	const char* deathpath;
	
	SDL_Texture* img; 

	Animation* currentAnimation = nullptr;
	Animation dinoIntro;

	//Animation* currentAnimationwin = nullptr;
	//Animation dinoWin;

	const char* audioPathintro;

	GuiButton* button1;
	GuiButton* button2;
	GuiButton* button3;
	GuiButton* button4;
	GuiButton* button5;

	bool start = true; 
	bool exit = true; 

};

#endif // SCENEINTRO
