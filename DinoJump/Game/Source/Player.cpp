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
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

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

		col = false;
	}

	// L07 DONE 5: Add physics to the player - updated player position using physics

	currentAnimation = &idleAnim;

	int speed = 10; 
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y); 

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//position.y -= 1;
		velocity = b2Vec2(0, -10);
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//position.y += 1;
		velocity = b2Vec2(0, 10);
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		//position.x -= 1;
		flip = SDL_FLIP_HORIZONTAL;
		currentAnimation = &movingAnim;
		velocity = b2Vec2(-5, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		//position.x += 1;
		flip = SDL_FLIP_NONE;
		currentAnimation = &movingAnim;
		velocity = b2Vec2(5, -GRAVITY_Y);


	}
	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(velocity);

	//Update player position in pixels

	SDL_Rect dino = currentAnimation->GetCurrentFrame();

	currentAnimation->Update();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 24 / 2;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 24 / 2;

	app->render->DrawTexture(texture, position.x , position.y, &dino, flip);

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
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
	


}
