#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"


struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro(bool startEnabled);

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

	// Called before quitting
	bool CleanUp();

public:

	bool On = false;
	bool reset = false;
	Player* player;


private:
	const char* imgpath;
	const char* deathpath;
	SDL_Texture* img; 
	SDL_Texture* death; 

	Animation* currentAnimation = nullptr;
	Animation dinoIntro;
	Animation dino2;

	Animation* currentAnimationdeath = nullptr;
	Animation dinoDeath;

	const char* audioPathwin;
	const char* audioPathlose;
	int audio;
	bool audiob = true; 

	

	

};

#endif // SCENEINTRO
