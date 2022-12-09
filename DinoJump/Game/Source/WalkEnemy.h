#ifndef __WALKENEMY_H__
#define __WALKENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
//#include "Item.h"

struct SDL_Texture;

class WalkEnemy : public Entity
{
public:

	WalkEnemy();

	virtual ~WalkEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void Follow();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);


public:
	PhysBody* pbody;
	PhysBody* sensor; 
	PhysBody* Kill;

	int WalkPosY = 0;
	int WalkPosX = 0;
	iPoint p;
	iPoint e;

	iPoint player;
	iPoint enemy;

	bool alive;
	bool kill = false;
	bool col = true;
	bool isDead = false;

private:
	SDL_Texture* texture;
	const char* texturePath;
	SDL_Texture* pathTileTex;

	Animation* currentAnimation = nullptr;
	Animation idleAnimEnemy;
	Animation movingAnimEnemy;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

};

#endif // __WALKENEMY_H__
