#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	int width = 16;
	int height = 16;
	float time = 0;
	float timeS = 0;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	int jumpCounter = 0;
	int slideCounter = 0;
	bool slide = false;
	bool jump = false;
	bool isjumping = false;
	bool Meta = false;
	bool lava = false;
	bool slideSlow = false;
	bool init = true;

	int PlayerPosY = 0;
	int PlayerPosX = 0;

	int lavaPosX = 0;
	int lavaPosY = 0;	
	
	int camPosX = 0;
	int camPosY = 0;

	int DetectPosX = 0;
	int DetectPosY = 0;

	PhysBody* CAM;
	PhysBody* LAVDetect;
	PhysBody* LAV;

	bool col = true;
	bool die = false;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	int pickCoinFxId;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation movingAnim;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

};

#endif // __PLAYER_H__