#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	bool isPicked = false;
	bool col = true;
	bool restart = true;

	//DONE 4: Add a physics to an item
	PhysBody* Coin1;
	PhysBody* Coin2;
	PhysBody* Coin3;

	int CoinX1 = 300;
	int CoinY1 = 1600;	
	
	int CoinX2 = 200;
	int CoinY2 = 1400;
	
	int CoinX3 = 350;
	int CoinY3 = 1150;

private:

	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __ITEM_H__