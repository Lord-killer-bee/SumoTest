#ifndef COLLISIONMANAGER_H_INCLUDED
#define COLLISIONMANAGER_H_INCLUDED

#include <d3dx9math.h>
#include "Collision.h"
#include "GameEntity.h"
#include <list>
#include <map>

class GameEntity;

class CollisionManager
{
public:
	struct CollisionRegion
	{
		int regionID_;
		D3DXVECTOR2 minPoint_, maxPoint_;
		
		std::list<GameEntity*> listOfObjectsInRegion_;
	};

	CollisionManager();
	~CollisionManager();

	Collision* GetCollision();

	void AddEntityToGrid(GameEntity* entity);// call when object is created
	void RemoveEntityFromGrid(GameEntity* entity);// call when object is destroyed
	void UpdateEntitiesInGrid();
	void CalculateCollisionDetection(Game* game);

private:
	void SetUpRegions();
	int ReturnRegionIDFromLocation(D3DXVECTOR2 position);
	void InsertEntityIntoRegion(int regionID, GameEntity* entity);
	void RemoveEntityFromRegion(int regionID, GameEntity* entity);
	bool CheckIfRegionChanged(GameEntity* entity);

	int cellCountInARowOrColumn_;
	std::map<int, CollisionRegion*> listOfRegions_;

	Collision* collision_;
};

#endif