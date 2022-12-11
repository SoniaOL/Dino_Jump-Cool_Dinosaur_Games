#ifndef __FLYENEMY_H__
#define __FLYENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"

//#include "Item.h"

struct SDL_Texture;

class FlyEnemy : public Entity
{
public:

	FlyEnemy();

	virtual ~FlyEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void Follow();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void EndContact(PhysBody* physA, PhysBody* physB);


public:
	PhysBody* pbody;
	PhysBody* sensor;
	PhysBody* Kill;

	int FlyPosY = 0;
	int FlyPosX = 0;
	int cont = 0;

	iPoint p;
	iPoint e;

	iPoint pos;

	iPoint player;
	iPoint enemy;

	bool alive; 
	bool kill = false;
	bool isDead = false;
	bool col = true;

	bool deadanim = false;
	bool follow = false;

private:
	SDL_Texture* texture;
	SDL_Texture* pathTileTex;
	const char* texturePath;
	const char* deathPath;

	Animation* currentAnimation = nullptr;
	Animation flyAnim;
	Animation deathAnim; 

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	
};

#endif // __FLYENEMY_H__
