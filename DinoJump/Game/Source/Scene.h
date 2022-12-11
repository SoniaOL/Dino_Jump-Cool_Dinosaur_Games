#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "FlyEnemy.h"
#include "WalkEnemy.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	FlyEnemy* fly;
	WalkEnemy* walk;
	bool col = true;
	int count = 0;
	int countGod = 0;
	SDL_Texture* mouseTileTex = nullptr;
	bool originSelected = false;
	bool change = false;
	iPoint origin;

private:
	SDL_Texture* img;

	const char* audioPath;
	int audio; 

};

#endif // __SCENE_H__