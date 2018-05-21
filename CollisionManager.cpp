#include "CollisionManager.h"

CollisionManager::CollisionManager() : cellCountInARowOrColumn_(4)
{
	collision_ = new Collision();
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

	listOfRegions_.clear();

	for (int i = 0; i < cellCountInARowOrColumn_ * cellCountInARowOrColumn_; i++)
	{
		CollisionRegion* region = new CollisionRegion();

		region->regionID_ = i;
		region->minPoint_ = D3DXVECTOR2((width / cellCountInARowOrColumn_) * j, (height / cellCountInARowOrColumn_) * k);
		region->maxPoint_ = region->minPoint_ + D3DXVECTOR2(width / cellCountInARowOrColumn_, height / cellCountInARowOrColumn_);

		listOfRegions_.insert(std::pair<int, CollisionRegion*>(i, region));

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
	int regionID = ReturnRegionIDFromLocation((D3DXVECTOR2)entity->GetPosition());//TODO: check if this cast is possible
	InsertEntityIntoRegion(regionID, entity);
}

void CollisionManager::RemoveEntityFromGrid(GameEntity* entity)
{
	int regionID = ReturnRegionIDFromLocation((D3DXVECTOR2)entity->GetPosition());//TODO: check if this cast is possible
	RemoveEntityFromRegion(regionID, entity);
}

void CollisionManager::UpdateEntitiesInGrid()
{

}

void CollisionManager::CalculateCollisionDetection(Game* game)
{

}

int CollisionManager::ReturnRegionIDFromLocation(D3DXVECTOR2 position)
{
	int row = position.x / 200;
	int column = position.y / 150;

	return row + column * cellCountInARowOrColumn_;
}

void CollisionManager::InsertEntityIntoRegion(int regionID, GameEntity* entity)
{
	std::map<int, CollisionRegion*>::iterator regionIt = listOfRegions_.find(regionID);

	(*regionIt).second->listOfObjectsInRegion_.push_back(entity);
}

void CollisionManager::RemoveEntityFromRegion(int regionID, GameEntity* entity)
{
	std::map<int, CollisionRegion*>::iterator regionIt = listOfRegions_.find(regionID);

	(*regionIt).second->listOfObjectsInRegion_.remove(entity);
}

bool CollisionManager::CheckIfRegionChanged(GameEntity* entity)
{
	return false;
}