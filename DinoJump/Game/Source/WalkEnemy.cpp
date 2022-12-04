#include "WalkEnemy.h"
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

WalkEnemy::WalkEnemy() : Entity(EntityType::WALKENEMY)
{
	name.Create("Enemies");

	idleAnimEnemy.PushBack({ 0,0,24,24 });
	idleAnimEnemy.PushBack({ 24,0,24,24 });
	idleAnimEnemy.PushBack({ 48,0,24,24 });
	idleAnimEnemy.PushBack({ 72,0,24,24 });
	idleAnimEnemy.speed = 0.1f;

	movingAnimEnemy.PushBack({ 96,0,24,24 });
	movingAnimEnemy.PushBack({ 120,0,24,24 });
	movingAnimEnemy.PushBack({ 144,0,24,24 });
	movingAnimEnemy.PushBack({ 168,0,24,24 });
	movingAnimEnemy.PushBack({ 192,0,24,24 });
	movingAnimEnemy.PushBack({ 216,0,24,24 });
	movingAnimEnemy.speed = 0.1f;

}

WalkEnemy::~WalkEnemy() {

}

bool WalkEnemy::Awake() {

	position.x = parameters.child("walk").attribute("x").as_int();
	position.y = parameters.child("walk").attribute("y").as_int();
	texturePath = parameters.child("walk").attribute("texturepath").as_string();

	return true;
}

bool WalkEnemy::Start() {

	pbody = app->physics->CreateCircle(position.x + (8 * 2), position.y + (8 * 2), 8, bodyType::DYNAMIC);
	texture = app->tex->Load(texturePath);
	pbody->listener = this;

	pbody->ctype = ColliderType::ENEMY;
	return true;
}

bool WalkEnemy::Update()
{
	currentAnimation = &idleAnimEnemy;

	SDL_Rect walk = currentAnimation->GetCurrentFrame();

	currentAnimation->Update();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	app->render->DrawTexture(texture, position.x - 12, position.y - 11, &walk, flip);

	return true;
}

bool WalkEnemy::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void WalkEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {


}
