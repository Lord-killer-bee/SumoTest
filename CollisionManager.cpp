#include "CollisionManager.h"

CollisionManager::CollisionManager() : cellCountInARowOrColumn_(4), listOfRegions_(0), collision_(0)
{
	collision_ = new Collision();
	listOfRegions_ = new RegionList();
	SetUpRegions();
}
CollisionManager::~CollisionManager()
{
	delete collision_;
}

Collision* CollisionManager::GetCollision()
{
	return collision_;
}

void CollisionManager::SetUpRegions()
{
	int width = 800, height = 600;

	int j = 0, k = 0;

	listOfRegions_->clear();

	for (int i = 0; i < 16; i++)
	{
		CollisionRegion* region = new CollisionRegion();

		region->regionID_ = i;
		region->minPoint_ = D3DXVECTOR2((float)(width / cellCountInARowOrColumn_) * j, (float)(height / cellCountInARowOrColumn_) * k);
		region->maxPoint_ = region->minPoint_ + D3DXVECTOR2((float)(width / cellCountInARowOrColumn_), (float)(height / cellCountInARowOrColumn_));

		listOfRegions_->insert(std::make_pair(i, region));

		j++;

		if(j > cellCountInARowOrColumn_ - 1)
		{
			j = 0;
			k++;
		}
	}
}

void CollisionManager::AddEntityToGrid(GameEntity* entity)
{
	GameEntity::RegionMap* regionMap = ReturnRegionMapFromTransform(D3DXVECTOR2(entity->GetPosition().x, entity->GetPosition().y) , (float)entity->GetCollider()->radius);//TODO: check if this cast is possible

	for (int i = 0; i < regionMap->count(); i++)
	{
		if(regionMap[i] == 1)
		{
			InsertEntityIntoRegion(i, entity);
		}
	}
	
	entity->SetCollisionRegionMap(regionMap);
}

void CollisionManager::RemoveEntityFromGrid(GameEntity* entity)
{
	GameEntity::RegionMap* regionMap = entity->GetCollisionRegionMap();//TODO: check if this cast is possible

	for (int i = 0; i < regionMap->count(); i++)
	{
		if(regionMap[i] == 1)
		{
			RemoveEntityFromRegion(i, entity);
		}
	}
}

void CollisionManager::UpdateEntitiesInGrid(Game* game)
{
	for (Collision::ColliderList::const_iterator colliderAIt = collision_->colliders_.begin(), end = collision_->colliders_.end();
		colliderAIt != end;
		++colliderAIt)
	{
		GameEntity* tempObject = (*colliderAIt)->entity;

		if(CheckIfRegionChanged(tempObject))
		{
			GameEntity::RegionMap* cachedMap = tempObject->GetCollisionRegionMap();

			GameEntity::RegionMap* currentMap = ReturnRegionMapFromTransform((D3DXVECTOR2)tempObject->GetPosition(), tempObject->GetCollider()->radius);

			for (int i = 0; i < cachedMap->count(); i++)
			{
				if(cachedMap[i] == 0 && currentMap[i] == 1)// entered new region
				{
					InsertEntityIntoRegion(i, tempObject);
				}
				else if(cachedMap[i] == 1 && currentMap[i] == 0)// exited a region
				{
					RemoveEntityFromRegion(i, tempObject);
				}
			}

			tempObject->SetCollisionRegionMap(currentMap);
		}
	}
}

void CollisionManager::CalculateCollisionDetection(Game* game)
{
	// store already checked pairs
	for (RegionList::const_iterator regionIt = listOfRegions_->begin(), end = listOfRegions_->end();
		regionIt != end;
		++regionIt)
	{
		if((*regionIt).second->listOfObjectsInRegion_.size() > 1)
			collision_->DoCollisionOnAListOfEntities((*regionIt).second->listOfObjectsInRegion_, game);
	}
}

GameEntity::RegionMap* CollisionManager::ReturnRegionMapFromTransform(D3DXVECTOR2 position, int radius)
{
	std::bitset<16>* tempMap = new std::bitset<16>();

	int column = position.x / 200;
	int row = position.y / 150;
	int index = column + row * cellCountInARowOrColumn_;

	tempMap[index] = 1;

	int rightBound, leftBound, topBound, bottomBound;
	bool rightSet, leftSet, topSet, bottomSet;

	rightBound = (position.x  + radius) / 200;
	leftBound = (position.x  - radius) / 200;
	topBound = (position.y  - radius) / 150;
	bottomBound = (position.y  + radius) / 150;

	if(column < rightBound && column != cellCountInARowOrColumn_ - 1)
		tempMap[index + 1] = 1; rightSet = true;
	if(column > leftBound && column != 0)
		tempMap[index - 1] == 1; leftSet = true;
	if(row < bottomBound && row != cellCountInARowOrColumn_ - 1)
		tempMap[index + cellCountInARowOrColumn_] = 1; bottomSet = true;
	if(row > topBound && row != 0)
		tempMap[index - cellCountInARowOrColumn_] = 1; topSet = true;

	if(rightSet && bottomSet)
		tempMap[index + 1 + cellCountInARowOrColumn_] = 1;
	if(rightSet && topSet)
		tempMap[index + 1 - cellCountInARowOrColumn_] = 1;
	if(leftSet && bottomSet)
		tempMap[index - 1 + cellCountInARowOrColumn_] = 1;
	if(leftSet && topSet)
		tempMap[index - 1 - cellCountInARowOrColumn_] = 1;

	return tempMap;
}

void CollisionManager::InsertEntityIntoRegion(int regionID, GameEntity* entity)
{
	RegionList::iterator regionIt = listOfRegions_->find(regionID);

	(*regionIt).second->listOfObjectsInRegion_.push_back(entity);
}

void CollisionManager::RemoveEntityFromRegion(int regionID, GameEntity* entity)
{
	RegionList::iterator regionIt = listOfRegions_->find(regionID);

	(*regionIt).second->listOfObjectsInRegion_.remove(entity);
}

bool CollisionManager::CheckIfRegionChanged(GameEntity* entity)
{
	GameEntity::RegionMap* cachedMap = entity->GetCollisionRegionMap();

	GameEntity::RegionMap* currentMap = ReturnRegionMapFromTransform((D3DXVECTOR2)entity->GetPosition(), entity->GetCollider()->radius);

	for (int i = 0; i < cachedMap->count(); i++)
	{
		if(cachedMap[i] != currentMap[i])
			return true;
	}

	return false;
}