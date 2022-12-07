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
	int FlyPosY = 0;
	int FlyPosX = 0;

	iPoint p;
	iPoint e;

	iPoint player;
	iPoint enemy;

private:
	SDL_Texture* texture;
	SDL_Texture* pathTileTex;
	const char* texturePath;

	Animation* currentAnimation = nullptr;
	Animation flyAnim;

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	
};

#endif // __FLYENEMY_H__
