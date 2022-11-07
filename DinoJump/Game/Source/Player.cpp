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

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.child("play").attribute("x").as_int();
	position.y = parameters.child("play").attribute("y").as_int();
	texturePath = parameters.child("play").attribute("texturepath").as_string();

	lavaPosX = parameters.child("LAV").attribute("x").as_int();
	lavaPosY = parameters.child("LAV").attribute("y").as_int();

	camPosX = parameters.child("CAM").attribute("x").as_int();
	camPosY = parameters.child("CAM").attribute("y").as_int();

	DetectPosX = parameters.child("LAVDetect").attribute("x").as_int();
	DetectPosY = parameters.child("LAVDetect").attribute("y").as_int();

	return true;
}

bool Player::Start() {

	return true;
}

bool Player::Update()
{

	if (col) {

		//initilize textures
		texture = app->tex->Load(texturePath);

		// L07 DONE 5: Add physics to the player - initialize physics body
		pbody = app->physics->CreateCircle(position.x + (10 * 2), position.y + (10 * 2), 10, bodyType::DYNAMIC);

		// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
		pbody->listener = this;

		// L07 DONE 7: Assign collider type
		pbody->ctype = ColliderType::PLAYER;

		//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
		pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

		pbody->GetPosition(PlayerPosX, PlayerPosY);

		CAM = app->physics->CreateRectangleSensor(camPosX, camPosY, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		CAM->ctype = ColliderType::CAM;

		LAVDetect = app->physics->CreateRectangleSensor(DetectPosX, DetectPosY, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		LAVDetect->ctype = ColliderType::LAVADETECT;
		
		LAV = app->physics->CreateRectangleSensor(lavaPosX, lavaPosY, 2000, 10, KINEMATIC);
		// L07 DONE 7: Assign collider type
		LAV->ctype = ColliderType::LAVA;

		if (init)
		{
			app->sceneIntro->reset = true;

			app->SaveGameRequest();

			init = false;
		}

		col = false;
	}

	// L07 DONE 5: Add physics to the player - updated player position using physics

	currentAnimation = &idleAnim;

	int speed = 5;

	LAV->body->SetLinearVelocity(b2Vec2(0, -0.3));

	if (isjumping) {
		vel.y = vel.y + (time / 5);
	}

	if (slideSlow) {
		vel.x = vel.x - (timeS / 5);
		if (vel.x <= 0) {
			vel.x = 0;
			slideSlow = false;
		}
	}
	if (!slideSlow) {
		vel.x = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (jump)
		{
			LOG("JUMPCOUNTER: %d", jumpCounter);
			time = 0;
			isjumping = true;
			vel.x = pbody->body->GetLinearVelocity().x;
			vel.y = -10;
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

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		flip = SDL_FLIP_HORIZONTAL;
		currentAnimation = &movingAnim;
		vel = b2Vec2(-speed, vel.y);
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			if (slideCounter != 2)
			{
				slideSlow = true;
				timeS = 0;
				vel.x += -50;
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
		vel = b2Vec2(speed, vel.y);
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			if (slideCounter != 2)
			{
				slideSlow = true;
				timeS = 0;
				vel.x += 50;
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

	if (lava) {
		app->render->camera.y += 1;
		CAM->body->SetLinearVelocity(b2Vec2(0, -1.2));
		LAVDetect->body->SetLinearVelocity(b2Vec2(0, -1.2));
	}
	if (!lava) {
		CAM->body->SetLinearVelocity(b2Vec2(0, 0));
		LAVDetect->body->SetLinearVelocity(b2Vec2(0, 0));
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

	app->render->DrawTexture(texture, position.x - 12, position.y - 12, &dino, flip);

	time++;
	timeS++;

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
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
		LOG("Collision META");
		break;
	case ColliderType::CAM:
		lava = true;
		LOG("Collision CAM");
		break;
	case ColliderType::LAVA:
		die = true;
		LOG("Collision LAVA");
		break;
	case ColliderType::LAVADETECT:
		lava = false;
		LOG("Collision DETECT");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}
