#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "SceneIntro.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	idleAnim.PushBack({ 0,0,24,24 });
	idleAnim.PushBack({ 24,0,24,24 });
	idleAnim.PushBack({ 48,0,24,24 });
	idleAnim.PushBack({ 72,0,24,24 });
	idleAnim.speed = 0.1f;

	movingAnim.PushBack({ 96,0,24,24 });
	movingAnim.PushBack({ 120,0,24,24 });
	movingAnim.PushBack({ 144,0,24,24 });
	movingAnim.PushBack({ 168,0,24,24 });
	movingAnim.PushBack({ 192,0,24,24 });
	movingAnim.PushBack({ 216,0,24,24 });
	movingAnim.speed = 0.1f;
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	audioPath = parameters.child("audioJump").attribute("path").as_string();
	audioCoin = parameters.child("audioCoin").attribute("path").as_string();
	audioPathSlide = parameters.child("audioSlide").attribute("path").as_string();

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.child("play").attribute("x").as_int();
	position.y = parameters.child("play").attribute("y").as_int();
	texturePath = parameters.child("play").attribute("texturepath").as_string();

	lavaPosX = parameters.child("LAV").attribute("x").as_int();
	lavaPosY = parameters.child("LAV").attribute("y").as_int();

	camPosX = parameters.child("CAM").attribute("x").as_int();
	camPosY = parameters.child("CAM").attribute("y").as_int();

	camPosGroundX = parameters.child("CAMG").attribute("x").as_int();
	camPosGroundY = parameters.child("CAMG").attribute("y").as_int();

	DetectPosX = parameters.child("LAVDetect").attribute("x").as_int();
	DetectPosY = parameters.child("LAVDetect").attribute("y").as_int();

	texturePathLava = parameters.child("LAV").attribute("texturepath").as_string(); 

	texturePathMeta = parameters.child("Meta").attribute("texturepath").as_string();

	texturePathHearth = parameters.child("gui").attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	pickCoinFxId = app->audio->LoadFx(audioCoin);
	audio = app->audio->LoadFx(audioPath);
	audioSlide = app->audio->LoadFx(audioPathSlide);

	return true;
}

bool Player::Update()
{

	if (col) {

		//initilize textures
		texture = app->tex->Load(texturePath);
		textureLava = app->tex->Load(texturePathLava); 
		textureMeta = app->tex->Load(texturePathMeta);
		textureHearth = app->tex->Load(texturePathHearth); 

		// L07 DONE 5: Add physics to the player - initialize physics body
		pbody = app->physics->CreateCircle(position.x + (8 * 2), position.y + (8 * 2), 8, bodyType::DYNAMIC);

		// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
		pbody->listener = this;

		// L07 DONE 7: Assign collider type
		pbody->ctype = ColliderType::PLAYER;

		//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
		/*pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");*/

		pbody->GetPosition(PlayerPosX, PlayerPosY);

		CAM = app->physics->CreateRectangleSensor(camPosX, camPosY, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		CAM->ctype = ColliderType::CAM;

		CAMG = app->physics->CreateRectangleSensor(DetectPosX, DetectPosY - 15, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		CAMG->ctype = ColliderType::CAMG;

		LAVDetect = app->physics->CreateRectangleSensor(DetectPosX, DetectPosY, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		LAVDetect->ctype = ColliderType::LAVADETECT;
		
		CAMGDetect = app->physics->CreateRectangleSensor(camPosX, camPosY + 50, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		CAMGDetect->ctype = ColliderType::CAMGDETECT;

		LAV = app->physics->CreateRectangleSensor(lavaPosX, lavaPosY, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		LAV->ctype = ColliderType::LAVA;

		META = app->physics->CreateRectangleSensor(229, 719, 50, 90, STATIC);
		// L07 DONE 7: Assign collider type
		META->ctype = ColliderType::META;

		CHECK = app->physics->CreateRectangleSensor(229, 900, 50, 90, STATIC);
		// L07 DONE 7: Assign collider type
		CHECK->ctype = ColliderType::CHECK;

		lifeT1 = app->physics->CreateRectangleSensor(lifeT1X, lifeT1Y, 20, 20, KINEMATIC);
		lifeT1->ctype = ColliderType::UNKNOWN;

		lifeT2 = app->physics->CreateRectangleSensor(lifeT2X, lifeT2Y, 20, 20, KINEMATIC);
		lifeT2->ctype = ColliderType::UNKNOWN;

		lifeT3 = app->physics->CreateRectangleSensor(lifeT3X, lifeT3Y, 20, 20, KINEMATIC);
		lifeT3->ctype = ColliderType::UNKNOWN;

		lifeT4 = app->physics->CreateRectangleSensor(lifeT4X, lifeT4Y, 20, 20, KINEMATIC);
		lifeT4->ctype = ColliderType::UNKNOWN;

		lifeT5 = app->physics->CreateRectangleSensor(lifeT5X, lifeT5Y, 20, 20, KINEMATIC);
		lifeT5->ctype = ColliderType::UNKNOWN;


		/*life1 = life();
		life1.Life = app->physics->CreateRectangleSensor(229, 1200, 20, 20, STATIC);
		life1.Life->ctype = ColliderType::LIFE;
		life1.id = 0;*/

		//LIFES.emplace_back(life1);

		/*lifeP2 = app->physics->CreateRectangleSensor(229, 1100, 20, 20, STATIC);
		lifeP2->ctype = ColliderType::LIFE;*/

		//life2 = life();
		//life2.Life = app->physics->CreateRectangleSensor(229, 1100, 20, 20, STATIC);
		//life2.Life->ctype = ColliderType::LIFE;
		//life2.id = 1;

		//LIFES.emplace_back(life2);

		/*life3 = life();
		life3.Life = app->physics->CreateRectangleSensor(229, 1400, 20, 20, STATIC);
		life3.Life->ctype = ColliderType::LIFE;
		life3.id = 2;*/

		//LIFES.emplace_back(life3);

		lifeP1 = app->physics->CreateRectangleSensor(lifep1X, lifep1Y, 20, 20, STATIC);
		lifeP1->ctype = ColliderType::LIFE;

		lifeP3 = app->physics->CreateRectangleSensor(lifep3X, lifep3Y, 20, 20, STATIC);
		lifeP3->ctype = ColliderType::LIFE;

		if (init)
		{
			app->sceneIntro->reset = true;

			app->SaveGameRequest();

			init = false;
		}

		col = false;
	}

	if (lives == true) 
	{
		lifeP1->body->SetActive(true);
		lifeP3->body->SetActive(true);

		lives = false;
	}

	if (DieCounter <= 0) 
	{
		die = true;
		audiob = true;
	}

	//CAMERA TOP
	if (app->render->camera.y >= -618) {
		lava = false;
	}

	if (position.y < lavaPosY) {
		dieCount = 0;
	}
	else {
		dieCount = 1;
	}

	// L07 DONE 5: Add physics to the player - updated player position using physics

	currentAnimation = &idleAnim;

	int speed = 5;

	LAV->body->SetLinearVelocity(b2Vec2(0, -0.3));

	if (isjumping) {
		vel.y = vel.y + (time / 20);
	}

	if (slideSlow) {
		if (Right) {
			vel.x = vel.x - (timeS / 20);
			if (vel.x <= 0) {
				vel.x = 0;
				slideSlow = false;
			}
		}
		if (Left) {
			vel.x = vel.x + (timeS / 20);
			if (vel.x >= 0) {
				vel.x = 0;
				slideSlow = false;
			}
		}
	}
	if (!slideSlow) {
		vel.x = 0;
		Left = false;
		Right = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (jump)
		{
			app->audio->PlayFx(audio);
			LOG("JUMPCOUNTER: %d", jumpCounter);
			time = 0;
			isjumping = true;
			vel.x = pbody->body->GetLinearVelocity().x;
			vel.y = -5;
			//pbody->body->ApplyForce(b2Vec2(0, METERS_TO_PIXELS(-100)), pbody->body->GetWorldCenter(), true);
		}

		if (isjumping) {
			if (jumpCounter == 1)
			{
				jump = false;
			}
			jumpCounter++;
		}

		//pbody->body->ApplyForce(b2Vec2(METERS_TO_PIXELS(0), METERS_TO_PIXELS(-2000.0f)), pbody->body->GetWorldCenter(), true);
		//position.y -= 1;
	}

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (GodMode) 
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel = b2Vec2(0, -10);
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
			vel = b2Vec2(0, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel = b2Vec2(0, 10);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
			vel = b2Vec2(0, 0);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		flip = SDL_FLIP_HORIZONTAL;
		currentAnimation = &movingAnim;
		if (!slideSlow) {
			vel = b2Vec2(-speed, vel.y);
		}
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			if (slideCounter != 2)
			{
				app->audio->PlayFx(audioSlide);
				slideSlow = true;
				Left = true;
				timeS = 0;
				vel.x += -2.5f;
				//pbody->body->ApplyForce(b2Vec2(METERS_TO_PIXELS(-30), 0), pbody->body->GetWorldCenter(), true);
				if (isjumping) {
					slideCounter += 0.5f;
				}
			}
			else {

				slide = false;
			}
		}

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		flip = SDL_FLIP_NONE;
		currentAnimation = &movingAnim;
		if (!slideSlow) {
			vel = b2Vec2(speed, vel.y);
		}
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			if (slideCounter != 2)
			{
				app->audio->PlayFx(audioSlide);
				slideSlow = true;
				Right = true;
				timeS = 0;
				vel.x += 2.5f;
				//pbody->body->ApplyForce(b2Vec2(METERS_TO_PIXELS(30), 0), pbody->body->GetWorldCenter(), true);
				if (isjumping) {
					slideCounter++;
				}
			}
			else {

				slide = false;
			}
		}
	}

	if (lava) 
	{
			app->render->camera.y += 4;
			CAM->body->SetLinearVelocity(b2Vec2(0, -4.8));
			LAVDetect->body->SetLinearVelocity(b2Vec2(0, -4.8));
			CAMG->body->SetLinearVelocity(b2Vec2(0, -4.8));
			CAMGDetect->body->SetLinearVelocity(b2Vec2(0, -4.8));
			lifeT1->body->SetLinearVelocity(b2Vec2(0, -4.8));
			lifeT2->body->SetLinearVelocity(b2Vec2(0, -4.8));
			lifeT3->body->SetLinearVelocity(b2Vec2(0, -4.8));
			lifeT4->body->SetLinearVelocity(b2Vec2(0, -4.8));
			lifeT5->body->SetLinearVelocity(b2Vec2(0, -4.8));
	}

	if(!lava)
	{
		if (!camg)
		{
			CAM->body->SetLinearVelocity(b2Vec2(0, 0));
			LAVDetect->body->SetLinearVelocity(b2Vec2(0, 0));
			CAMG->body->SetLinearVelocity(b2Vec2(0, 0));
			CAMGDetect->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT1->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT2->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT3->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT4->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT5->body->SetLinearVelocity(b2Vec2(0, 0));
		}
	}

	if (camg) 
	{
		app->render->camera.y -= 4;
		CAM->body->SetLinearVelocity(b2Vec2(0, 4.8));
		LAVDetect->body->SetLinearVelocity(b2Vec2(0, 4.8));
		CAMG->body->SetLinearVelocity(b2Vec2(0, 4.8));
		CAMGDetect->body->SetLinearVelocity(b2Vec2(0, 4.8));
		lifeT1->body->SetLinearVelocity(b2Vec2(0, 4.8));
		lifeT2->body->SetLinearVelocity(b2Vec2(0, 4.8));
		lifeT3->body->SetLinearVelocity(b2Vec2(0, 4.8));
		lifeT4->body->SetLinearVelocity(b2Vec2(0, 4.8));
		lifeT5->body->SetLinearVelocity(b2Vec2(0, 4.8));
	}

	if (!camg) 
	{
		if (!lava)
		{
			CAM->body->SetLinearVelocity(b2Vec2(0, 0));
			LAVDetect->body->SetLinearVelocity(b2Vec2(0, 0));
			CAMG->body->SetLinearVelocity(b2Vec2(0, 0));
			CAMGDetect->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT1->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT2->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT3->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT4->body->SetLinearVelocity(b2Vec2(0, 0));
			lifeT5->body->SetLinearVelocity(b2Vec2(0, 0));
		}
	}


	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels

	SDL_Rect dino = currentAnimation->GetCurrentFrame();

	currentAnimation->Update();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	camPosX = METERS_TO_PIXELS(CAM->body->GetTransform().p.x);
	camPosY = METERS_TO_PIXELS(CAM->body->GetTransform().p.y);

	lavaPosX = METERS_TO_PIXELS(LAV->body->GetTransform().p.x);
	lavaPosY = METERS_TO_PIXELS(LAV->body->GetTransform().p.y);

	DetectPosX = METERS_TO_PIXELS(LAVDetect->body->GetTransform().p.x);
	DetectPosY = METERS_TO_PIXELS(LAVDetect->body->GetTransform().p.y);
	
	lifeT1X = METERS_TO_PIXELS(lifeT1->body->GetTransform().p.x);
	lifeT1Y = METERS_TO_PIXELS(lifeT1->body->GetTransform().p.y);	
	
	lifeT2X = METERS_TO_PIXELS(lifeT2->body->GetTransform().p.x);
	lifeT2Y = METERS_TO_PIXELS(lifeT2->body->GetTransform().p.y);
	
	lifeT3X = METERS_TO_PIXELS(lifeT3->body->GetTransform().p.x);
	lifeT3Y = METERS_TO_PIXELS(lifeT3->body->GetTransform().p.y);

	lifeT4X = METERS_TO_PIXELS(lifeT4->body->GetTransform().p.x);
	lifeT4Y = METERS_TO_PIXELS(lifeT4->body->GetTransform().p.y);
	
	lifeT5X = METERS_TO_PIXELS(lifeT5->body->GetTransform().p.x);
	lifeT5Y = METERS_TO_PIXELS(lifeT5->body->GetTransform().p.y);

	SDL_Rect gui;
	gui.x = 128; 
	gui.y = 128; 
	gui.w = 16; 
	gui.h = 17; 

	app->render->DrawTexture(texture, position.x - 12, position.y - 11, &dino, flip);
	app->render->DrawTexture(textureLava, lavaPosX - 35, lavaPosY - 5);
	app->render->DrawTexture(textureMeta, 130, 643);

	if (lifeP1->body->IsActive() == true)
	{
		app->render->DrawTexture(textureHearth, lifep1X - 8, lifep1Y - 8, &gui);
	}
	if (lifeP3->body->IsActive() == true)
	{
		app->render->DrawTexture(textureHearth, lifep3X - 8, lifep3Y - 8, &gui);
	}

	if (DieCounter >= 1)
	{
		app->render->DrawTexture(textureHearth, lifeT1X - 8, lifeT1Y - 8, &gui);
	}	
	if (DieCounter >= 2)
	{
		app->render->DrawTexture(textureHearth, lifeT2X - 8, lifeT2Y - 8, &gui);
	}
	if (DieCounter >= 3)
	{
		app->render->DrawTexture(textureHearth, lifeT3X - 8, lifeT3Y - 8, &gui);
	}	
	if (DieCounter >= 4)
	{
		app->render->DrawTexture(textureHearth, lifeT4X - 8, lifeT4Y - 8, &gui);
	}
	if (DieCounter >= 5)
	{
		app->render->DrawTexture(textureHearth, lifeT5X - 8, lifeT5Y - 8, &gui);
	}

	time++;
	timeS++;

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) 
{

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			app->audio->PlayFx(pickCoinFxId);
			if (physB->body->IsActive() == true)
			{
				Coins++;
			}
			physB->body->SetActive(false);
			break;
	case ColliderType::PLATFORM:
		jump = true;
		isjumping = false;
		time = 0;
		vel.y = -GRAVITY_Y;
		jumpCounter = 0;
		slideCounter = 0;
		slide = true;
		break;
	case ColliderType::META:
		Meta = true;
		audiow = true;
		app->scene->fly->kill = true;
		app->scene->fly->deadanim = true;
		app->scene->walk->kill = true;
		app->scene->walk->deadanim = true;
		break;
	case ColliderType::CHECK:
		app->SaveGameRequest();
		app->sceneIntro->reset = false;
		LOG("Check");
		break;
	case ColliderType::CAM:
		lava = true;
		break;
	case ColliderType::CAMG:
		camg = true;
		break;
	case ColliderType::LAVA:
		if (!GodMode)
		{
			if (app->scene->player->die == false) {
				DieCounter--;
				/*audiob = true;*/
				app->LoadGameRequest();
				app->sceneIntro->reset = false;
			}
		}
		break;
	case ColliderType::ENEMY:
		if (!GodMode)
		{
			if (app->scene->player->die == false) {
				DieCounter--;
				/*	audiob = true;*/
				app->LoadGameRequest();
				app->sceneIntro->reset = false;
			}
		}
		break;
	case ColliderType::KILLFLY:
		app->scene->fly->kill = true;
		app->scene->fly->deadanim = true;
		break;
	case ColliderType::KILLWALK:
		app->scene->walk->kill = true;
		app->scene->walk->deadanim = true;
		break;
	case ColliderType::LAVADETECT:
		lava = false;
		break;
	case ColliderType::CAMGDETECT:
		camg = false;
		break;
	case ColliderType::LIFE:

		if (physB->body->IsActive() == true)
		{
			DieCounter++;
		}
		physB->body->SetActive(false);
			break;
	case ColliderType::UNKNOWN:
		break;
		}
}
