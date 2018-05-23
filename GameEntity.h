#ifndef GAMEENTITY_H_INCLUDED
#define GAMEENTITY_H_INCLUDED

#include <d3dx9math.h>
#include <bitset>

class System;
class Graphics;
class Collision;
class Collider;

class GameEntity
{
public:
	GameEntity();
	virtual ~GameEntity();

	virtual void Update(System *system);
	virtual void Render(Graphics *graphics) const;

	D3DXVECTOR3 GetPosition() const;
	void SetPosition(const D3DXVECTOR3 &position);

	Collider* GetCollider();

	typedef std::bitset<16> RegionMap;
	RegionMap* GetCollisionRegionMap();
	void SetCollisionRegionMap(RegionMap* map);

	void EnableCollisions(Collision *collisionSystem, float radius);
	void DisableCollisions();

private:

	bool HasValidCollider() const;
	void DestroyCollider();

	RegionMap* regionMap_;

	D3DXVECTOR3 position_;
	Collision *collisionSystem_;
	Collider *collider_;

};

#endif // GAMEENTITY_H_INCLUDED
