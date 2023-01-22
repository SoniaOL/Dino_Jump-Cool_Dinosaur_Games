#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"


struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin();

	// Destructor
	virtual ~SceneWin();

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

	bool init = true;


private:
	const char* winpath;
	SDL_Texture* win;

	Animation* currentAnimationwin = nullptr;
	Animation dinoWin;

	const char* audioPathwin;
	int audiowin;

};

#endif // SCENEWIN

