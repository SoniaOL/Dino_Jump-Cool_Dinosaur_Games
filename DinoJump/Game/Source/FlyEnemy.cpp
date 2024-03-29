#include "FlyEnemy.h"
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

FlyEnemy::FlyEnemy() : Entity(EntityType::FLYENEMY)
{
	name.Create("Enemies");

	flyAnim.PushBack({ 9,30,37,33 });
	flyAnim.PushBack({ 51,30,37,33 });
	flyAnim.PushBack({ 95,30,37,33 });
	flyAnim.PushBack({ 140,30,37,33 });
	flyAnim.PushBack({ 184,30,37,33 });
	flyAnim.PushBack({ 227,30,37,33 });
	flyAnim.PushBack({ 271,30,37,33 });
	flyAnim.speed = 0.2f;

	deathAnim.PushBack({ 10,84,33,91 });
	deathAnim.PushBack({ 54,84,33,91 });
	deathAnim.PushBack({ 98,84,33,91 });
	deathAnim.PushBack({ 142,84,33,91 });
	deathAnim.PushBack({ 186,84,33,91 });
	deathAnim.PushBack({ 229,84,33,91 });
	deathAnim.speed = 0.2f; 


}

FlyEnemy::~FlyEnemy() {

}

bool FlyEnemy::Awake() {

	position.x = parameters.child("fly").attribute("x").as_int();
	position.y = parameters.child("fly").attribute("y").as_int();
	texturePath = parameters.child("fly").attribute("texturepath").as_string();
	audioPath = parameters.child("audio").attribute("flyhurt").as_string(); 

	return true;
}

bool FlyEnemy::Start() {

	texture = app->tex->Load(texturePath);
	audio = app->audio->LoadFx(audioPath);

	pos.x = position.x + (7);
	pos.y = position.y + (10);

	//pbody = app->physics->CreateCircle(pos.x, pos.y, 12, bodyType::DYNAMIC);
	//pbody->body->SetGravityScale(0);

	//pbody->listener = this;

	//pbody->ctype = ColliderType::LAVA;

	//sensor = app->physics->CreateRectangleSensor(position.x + (7), position.y + (7), 200, 200, bodyType::KINEMATIC);
	//sensor->listener = this;
	//sensor->ctype = ColliderType::SENSOR;	
	//
	//Kill = app->physics->CreateRectangleSensor(position.x, position.y, 15, 30, bodyType::KINEMATIC);
	//Kill->ctype = ColliderType::KILL;

	//pbody->GetPosition(FlyPosX, FlyPosY);

	pathTileTex = app->tex->Load("Assets/Maps/MapMetadata.png");
	
	/*alive = true; */

	return true;
}

bool FlyEnemy::Update()
{
	if (col) 
	{
		pbody = app->physics->CreateCircle(pos.x, pos.y, 9, bodyType::DYNAMIC);
		pbody->body->SetGravityScale(0);
		pbody->listener = this;

		pbody->ctype = ColliderType::ENEMY;

		sensor = app->physics->CreateRectangleSensor(position.x + (7), position.y + (7), 200, 200, bodyType::KINEMATIC);
		sensor->listener = this;
		sensor->ctype = ColliderType::SENSOR;

		Kill = app->physics->CreateRectangleSensor(position.x, position.y, 15, 30, bodyType::KINEMATIC);
		Kill->ctype = ColliderType::KILLFLY;

		pbody->GetPosition(FlyPosX, FlyPosY);

		alive = true;
		kill = false;
		isDead = false;

		deadanim = false;
		col = false;

		currentAnimation = &deathAnim;
		currentAnimation->Reset();
		 
	}

	if (deadanim == true) {

		if (currentAnimation->current_frame < 5) {
			currentAnimation = &deathAnim;
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

		currentAnimation = &flyAnim;

		SDL_Rect fly = currentAnimation->GetCurrentFrame();

		currentAnimation->Update();

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

		if (alive == true) {
			app->render->DrawTexture(texture, position.x - 19, position.y - 19, &fly, flip);
		}

		if (app->physics->debug)
		{
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				SDL_Rect rect = { 0,0,18,18 };

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
				deadanim = true; 
				
				isDead = true;
		}

		if (kill) 
		{
			alive = false;
			pbody->body->GetWorld()->DestroyBody(pbody->body);
			sensor->body->GetWorld()->DestroyBody(sensor->body);
			Kill->body->GetWorld()->DestroyBody(Kill->body);
			app->audio->PlayFx(audio);
			app->render->name;
			deadanim = true;
			isDead = true;
		}

		if (follow) {

			app->pathfinding->CreatePath(enemy, player);

			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

			if (path->At(1) != nullptr)
			{
				iPoint pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);

				float32 speed = 0.05;

				if (e.y < pos.y) {
					pbody->body->SetLinearVelocity({ 0, speed * 20 });
					//LOG("POS Y -");
				}

				if (e.x < pos.x) {
					pbody->body->ApplyForceToCenter({ speed, 0 }, true);
					//LOG("POS X +");
				}

				if (e.y > pos.y) {
					pbody->body->ApplyForceToCenter({ 0, -speed }, true);
					//LOG("POS Y +");
				}

				if (e.x > pos.x) {
					pbody->body->ApplyForceToCenter({ -speed, 0 }, true);
					//LOG("POS X -");
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

bool FlyEnemy::CleanUp()
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
void FlyEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{

	case ColliderType::PLAYER:
		//app->pathfinding->CreatePath(enemy, player);
		follow = true;
		//Follow();
		break;
/*	case ColliderType::LAVA:
		LOG("ELIMINATE");
		alive = false;
		CleanUp();
		break;
	*/}

}

void FlyEnemy::EndContact(PhysBody* physA, PhysBody* physB) {

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

void FlyEnemy::Follow() 
{
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);

		float32 speed = 1.5f;

		if (e.y < pos.y) {
			pbody->body->SetLinearVelocity({0, speed });
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
