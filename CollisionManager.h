#ifndef COLLISIONMANAGER_H_INCLUDED
#define COLLISIONMANAGER_H_INCLUDED

#include <d3dx9math.h>
#include "Collision.h"
#include "Collider.h"
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
	void UpdateEntitiesInGrid(Game* game);
	void CalculateCollisionDetection(Game* game);

private:
	void SetUpRegions();
	GameEntity::RegionMap* ReturnRegionMapFromTransform(D3DXVECTOR2 position, int radius);
	void InsertEntityIntoRegion(int regionID, GameEntity* entity);
	void RemoveEntityFromRegion(int regionID, GameEntity* entity);
	bool CheckIfRegionChanged(GameEntity* entity);

	int cellCountInARowOrColumn_;
	
	typedef std::map<int, CollisionRegion*> RegionList;
	RegionList* listOfRegions_;

	Collision* collision_;
};

#endif