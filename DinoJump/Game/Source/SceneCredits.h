#ifndef __SCENECREDITS_H__
#define __SCENECREDITS_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"


struct SDL_Texture;

class SceneCredit : public Module
{
public:

	SceneCredit();

	// Destructor
	virtual ~SceneCredit();

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

	// Called before quitting
	bool CleanUp();

public:
	bool col = true;

private:
	const char* creditpath;
	SDL_Texture* credit;

	const char* creditsong;
	int audiocredit;
};

#endif

