#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "FlyEnemy.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

public:

	List<Entity*> entities;

	bool SaveState(pugi::xml_node&);
	bool LoadState(pugi::xml_node&);

	float PosX;
	float PosY;
	float Time;
	int flyLive = 0;
	int walkLive = 0;
};

#endif // __ENTITYMANAGER_H__
