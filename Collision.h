#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <d3dx9math.h>
#include <list>

class GameEntity;
class Game;
class Collider;

class Collision
{
public:
	Collision();
	~Collision();

	friend class CollisionManager;

	Collider *CreateCollider(GameEntity *entity);
	void DestroyCollider(Collider *collider);

	void UpdateColliderPosition(Collider *collider, const D3DXVECTOR3 &position);
	void UpdateColliderRadius(Collider *collider, float radius);
	void EnableCollider(Collider *collider);
	void DisableCollider(Collider *collider);

	void DoCollisions(Game *game) const;
	void DoCollisionOnAListOfEntities(std::list<GameEntity* > entities, Game* game) const;

private:

	typedef std::list<Collider *> ColliderList;

	static bool CollisionTest(Collider *a, Collider *b);

	ColliderList colliders_;

};

#endif // COLLISION_H_INCLUDED
