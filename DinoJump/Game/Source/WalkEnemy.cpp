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
#include "PathFinding.h"

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

bool WalkEnemy::Start() 
{
	texture = app->tex->Load(texturePath);
	pathTileTex = app->tex->Load("Assets/Maps/MapMetadata.png");

	return true;
}

bool WalkEnemy::Update()
{
	if (col)
	{
		pbody = app->physics->CreateCircle(228, 1057, 8, bodyType::DYNAMIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::LAVA;

		sensor = app->physics->CreateRectangleSensor(position.x + (8 * 2), position.y + (8 * 2), 60, 60, bodyType::KINEMATIC);
		sensor->listener = this;
		sensor->ctype = ColliderType::SENSOR;

		Kill = app->physics->CreateRectangleSensor(position.x, position.y, 15, 30, bodyType::KINEMATIC);
		Kill->ctype = ColliderType::KILLWALK;

		pbody->GetPosition(WalkPosX, WalkPosY);

		alive = true;
		isDead = false;
		kill = false;

		col = false;
	}

	if (!isDead)
	{
		app->scene->player->pbody->GetPosition(p.x, p.y);
		pbody->GetPosition(e.x, e.y);

		enemy = app->map->WorldToMap(e.x, e.y);

		player = app->map->WorldToMap(p.x, p.y);

		currentAnimation = &idleAnimEnemy;

		SDL_Rect walk = currentAnimation->GetCurrentFrame();

		currentAnimation->Update();

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

		if (alive == true) {
			app->render->DrawTexture(texture, position.x - 12, position.y - 11, &walk, flip);
		}

		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			SDL_Rect rect = { 0,0,36,18 };

			app->render->DrawTexture(pathTileTex, pos.x, pos.y, &rect);

		}

		if (e.y + 12 >= app->scene->player->lavaPosY)
		{
			alive = false;
			pbody->body->GetWorld()->DestroyBody(pbody->body);
			sensor->body->GetWorld()->DestroyBody(sensor->body);
			Kill->body->GetWorld()->DestroyBody(Kill->body);
			isDead = true;
		}

		if (kill)
		{
			alive = false;
			pbody->body->GetWorld()->DestroyBody(pbody->body);
			sensor->body->GetWorld()->DestroyBody(sensor->body);
			Kill->body->GetWorld()->DestroyBody(Kill->body);
			isDead = true;
		}

		b2Vec2 vec = { (float32)PIXEL_TO_METERS(position.x), (float32)PIXEL_TO_METERS(position.y) };
		b2Vec2 vec2 = { (float32)PIXEL_TO_METERS(position.x), (float32)PIXEL_TO_METERS(position.y - 0.25) };

		sensor->body->SetTransform(vec, 0);
		Kill->body->SetTransform(vec2, 0);
	}

	return true;
}

bool WalkEnemy::CleanUp()
{
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	sensor->body->GetWorld()->DestroyBody(sensor->body);
	Kill->body->GetWorld()->DestroyBody(Kill->body);

	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void WalkEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		app->pathfinding->CreatePath(enemy, player);
		Follow();
		break;
	}
}

void WalkEnemy::Follow()
{
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);

		float32 speed = 1.5f;

		if (e.y < pos.y) {
			pbody->body->SetLinearVelocity({ 0, speed });
		}

		if (e.x < pos.x) {
			pbody->body->SetLinearVelocity({ speed, 0 });
		}

		if (e.y > pos.y) {
			pbody->body->SetLinearVelocity({ 0, -speed });
		}

		if (e.x > pos.x) {
			pbody->body->SetLinearVelocity({ -speed, 0 });
		}
	}
}

