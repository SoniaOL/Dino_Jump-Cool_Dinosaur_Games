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
	movingAnimEnemy.speed = 0.2f;

	deathAnimEnemy.PushBack({ 336,0,24,24 });
	deathAnimEnemy.PushBack({ 360,0,24,24 });
	deathAnimEnemy.PushBack({ 384,0,24,24 });
	deathAnimEnemy.speed = 0.1f; 

}

WalkEnemy::~WalkEnemy() {

}

bool WalkEnemy::Awake() {

	position.x = parameters.child("walk").attribute("x").as_int();
	position.y = parameters.child("walk").attribute("y").as_int();
	texturePath = parameters.child("walk").attribute("texturepath").as_string();
	audioPath = parameters.child("audio").attribute("walkhurt").as_string();

	return true;
}

bool WalkEnemy::Start() 
{
	texture = app->tex->Load(texturePath);
	pathTileTex = app->tex->Load("Assets/Maps/MapMetadata.png");
	audio = app->audio->LoadFx(audioPath);

	currentAnimation = &idleAnimEnemy;

	return true;
}

bool WalkEnemy::Update()
{
	if (col)
	{
		pbody = app->physics->CreateCircle(position.x, position.y, 7.5, bodyType::DYNAMIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::ENEMY;
		pbody->body->SetGravityScale(20.0f);

		LOG("PosX: %d", position.x + (8 * 2));
		LOG("PosY: %d", position.y + (8 * 2));

		sensor = app->physics->CreateRectangleSensor(position.x + (8 * 2), position.y + (8 * 2), 250, 100, bodyType::KINEMATIC);
		sensor->listener = this;
		sensor->ctype = ColliderType::SENSOR;

		Kill = app->physics->CreateRectangleSensor(position.x, position.y + (20), 10, 20, bodyType::KINEMATIC);
		Kill->ctype = ColliderType::KILLWALK;

		pbody->GetPosition(WalkPosX, WalkPosY);

		alive = true;
		isDead = false;
		kill = false;

		col = false;
		deadanim = false;
		currentAnimation = &deathAnimEnemy;
		currentAnimation->Reset();
	}

	if (deadanim == true) {

		if (currentAnimation->current_frame < 2) {
			currentAnimation = &deathAnimEnemy;
			SDL_Rect die = currentAnimation->GetCurrentFrame();
			currentAnimation->Update();
			app->render->DrawTexture(texture, position.x - 19, position.y - 19, &die, flip);
		}
	}

	if (!isDead)
	{
		app->scene->player->pbody->GetPosition(p.x, p.y);
		pbody->GetPosition(e.x, e.y);

		enemy = app->map->WorldToMap(e.x, e.y);

		player = app->map->WorldToMap(p.x, p.y);

		SDL_Rect walk = currentAnimation->GetCurrentFrame();

		currentAnimation = &idleAnimEnemy;

		if (pbody->body->GetLinearVelocity().x <= -0.1f) {

			flip = SDL_FLIP_HORIZONTAL;
			currentAnimation = &movingAnimEnemy;
		}

		if (pbody->body->GetLinearVelocity().x >= 0.1f) {

			flip = SDL_FLIP_NONE;
			currentAnimation = &movingAnimEnemy;
		}

		currentAnimation->Update();

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

		if (alive == true) {
			app->render->DrawTexture(texture, position.x - 12, position.y - 11, &walk, flip);
		}

		if (app->physics->debug)
		{
			const DynArray<iPoint>* path = app->pathfinding2->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			SDL_Rect rect = { 0,0,36,18 };

			app->render->DrawTexture(pathTileTex, pos.x, pos.y, &rect);

			}
		}

		if (e.y + 12 >= app->scene->player->lavaPosY)
		{
			alive = false;
			pbody->body->GetWorld()->DestroyBody(pbody->body);
			sensor->body->GetWorld()->DestroyBody(sensor->body);
			Kill->body->GetWorld()->DestroyBody(Kill->body);
			app->audio->PlayFx(audio);
			isDead = true;
			deadanim = true; 
		}

		if (kill)
		{
			alive = false;
			pbody->body->GetWorld()->DestroyBody(pbody->body);
			sensor->body->GetWorld()->DestroyBody(sensor->body);
			Kill->body->GetWorld()->DestroyBody(Kill->body);
			app->audio->PlayFx(audio);
			isDead = true;
			deadanim = true; 
		}

		if (follow) {
			app->pathfinding2->CreatePath(enemy, player);

			const DynArray<iPoint>* path = app->pathfinding2->GetLastPath();

			if (path->At(1) != nullptr)
			{
			iPoint pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);


				float32 speed = 0.6f;

				/*if (e.y < pos.y) {
					pbody->body->SetLinearVelocity({ 0, speed });
				}*/

				if (e.x < pos.x) {
					//LOG("-");
					pbody->body->SetLinearVelocity({ speed, 5.0f });
				}

				//if (e.y > pos.y) {
				//	//pbody->body->ApplyForce({0, -4}, {(float32)position.x, (float32)position.y}, true);
				//	pbody->body->SetLinearVelocity({ 0, -speed });
				//}

				if (e.x > pos.x) {
					//LOG("+");
					pbody->body->SetLinearVelocity({ -speed, 5.0f });
				}
			}
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
	if (app->scene->player->active == false) {
		if (isDead == false) {
			pbody->body->GetWorld()->DestroyBody(pbody->body);
			sensor->body->GetWorld()->DestroyBody(sensor->body);
			Kill->body->GetWorld()->DestroyBody(Kill->body);
		}
	}
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void WalkEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		//app->pathfinding2->CreatePath(enemy, player);
		if (app->pathfinding2->IsWalkable(player) && app->pathfinding2->IsWalkable(enemy))
		{
			follow = true;
		}
		//Follow();
		break;
	}
}

void WalkEnemy::EndContact(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		follow = false;
		pbody->body->SetLinearVelocity({ 0, 0 });
		sensor->body->SetLinearVelocity({ 0, 0 });
		Kill->body->SetLinearVelocity({ 0, 0 });
		break;
	}
}

void WalkEnemy::Follow()
{
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		iPoint epos = app->map->MapToWorld(e.x, e.y + 4);
		

		LOG("e.y: %d", epos.y);
		LOG("pos.y: %d", pos.y);

		float32 speed = 1.5f;

		if (epos.y < pos.y) {
			pbody->body->SetLinearVelocity({ 0, speed });
		}

		if (epos.x < pos.x) {
			pbody->body->SetLinearVelocity({ speed, 0 });
	
		}

		if (epos.y > pos.y) {
			//pbody->body->ApplyForce({0, -4}, {(float32)position.x, (float32)position.y}, true);
			pbody->body->SetLinearVelocity({ 0, -speed });
		}

		if (epos.x > pos.x) {
			pbody->body->SetLinearVelocity({ -speed, 0 });
	
		}
	}
}

