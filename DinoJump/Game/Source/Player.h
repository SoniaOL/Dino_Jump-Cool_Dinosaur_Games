#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
#include "FlyEnemy.h"
#include "Timer.h"
//#include "Item.h"

#include <vector>

struct SDL_Texture;

class life
{
public:
	int id; 
	// Convert geometry to pixels to draw w/ SDL
	PhysBody* Life;
};

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
	void EndContact(PhysBody* physA, PhysBody* physB);


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
	bool camg = false; 
	bool slideSlow = false;
	bool init = true;
	bool stop = false;

	int PlayerPosY = 0;
	int PlayerPosX = 0;

	int lavaPosX = 0;
	int lavaPosY = 0;	
	
	int camPosX = 0;
	int camPosY = 0;

	int camPosGroundX = 0;
	int camPosGroundY = 0;

	int DetectPosX = 0;
	int DetectPosY = 0;

	int dieCount = 0;

	PhysBody* CAM;
	PhysBody* CAMG;
	PhysBody* LAVDetect;
	PhysBody* LAV;
	PhysBody* META;
	PhysBody* CAMGDetect;
	PhysBody* TOP;

	PhysBody* lifeP1;
	PhysBody* lifeP2;
	PhysBody* lifeP3;

	PhysBody* lifeT1;
	PhysBody* lifeT2;
	PhysBody* lifeT3;
	PhysBody* lifeT4;
	PhysBody* lifeT5;

	life life1{};
	life life2{};
	life life3{};

	std::vector<life> LIFES{};

	bool col = true;
	bool die = false;
	int DieCounter = 3;
	int Coins = 0;
	bool audiob = true;
	bool audiow = true; 
	bool Right = false;
	bool Left = false;
	bool GodMode = false;
	bool lives = true;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	FlyEnemy* fly; 

	int lifep3Y = 1458;
	int lifep3X = 360;

	int lifep1Y = 1085;
	int lifep1X = 100;

	int lifeT1Y = 950;
	int lifeT1X = 400;

	int lifeT2Y = 950;
	int lifeT2X = 380;

	int lifeT3Y = 950;
	int lifeT3X = 360;
	
	int lifeT4Y = 950;
	int lifeT4X = 340;
	
	int lifeT5Y = 950;
	int lifeT5X = 320;


	PhysBody* Coin1;
	PhysBody* Coin2;
	PhysBody* Coin3;
	PhysBody* Coin4;

	int CoinX1 = 300;
	int CoinY1 = 1600;

	int CoinX2 = 200;
	int CoinY2 = 1400;

	int CoinX3 = 350;
	int CoinY3 = 1150;

	int CoinX4 = 365;
	int CoinY4 = 980;


	PhysBody* CHECK;
	PhysBody* CHECK1;

	int Check1X = 150;
	int Check1Y = 890;	
	
	int Check2X = 70;
	int Check2Y = 1250;

	bool check1;
	bool check2;
	bool CheckOut1 = true;
	bool CheckOut2 = true;
	bool playerPos = false;
	bool MetaSpawn = false;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	SDL_Texture* textureLava; 
	const char* texturePathLava;

	SDL_Texture* textureMeta;
	const char* texturePathMeta;

	SDL_Texture* textureHearth; 
	const char* texturePathHearth; 

	SDL_Texture* textureCoin;

	const char* audioPath;
	const char* audioCoin;
	const char* audioPathSlide;
	const char* audioheart; 
	const char* audiohit; 
	int hitaudio; 
	int heartsound;
	int audio;
	int pickCoinFxId;
	int audioSlide;

	/*int pickCoinFxId;*/

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation movingAnim;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	/*Item* item; */

};

#endif // __PLAYER_H__