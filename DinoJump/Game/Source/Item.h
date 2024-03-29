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

	SDL_Texture* texture;
	const char* texturePath;

	//DONE 4: Add a physics to an item


private:

};

#endif // __ITEM_H__