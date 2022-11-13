#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"


struct SDL_Texture;

class sceneLogo : public Module
{
public:

	sceneLogo();

	// Destructor
	virtual ~sceneLogo();

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





};

#endif // SCENELOGO
