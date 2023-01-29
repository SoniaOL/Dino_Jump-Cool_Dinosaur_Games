#ifndef __SETTING_H__
#define __SETTING_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"
#include "GuiButton.h"


struct SDL_Texture;

class Settings : public Module
{
public:

	Settings();

	// Destructor
	virtual ~Settings();

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



private:

	GuiButton* button6; 
	GuiButton* button7; 


};

#endif // SETTINGS

