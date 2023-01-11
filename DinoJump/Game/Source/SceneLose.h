#ifndef __SCENELOSE_H__
#define __SCENELOSE_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"


struct SDL_Texture;

class SceneLose : public Module
{
public:

	SceneLose();

	// Destructor
	virtual ~SceneLose();

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



private:
	SDL_Texture* death;

	const char* deathpath;

	Animation* currentAnimationdeath = nullptr;
	Animation dinoDeath;

	const char* audioPathlose;
	int audiolose;

};

#endif // SCENELOGO
