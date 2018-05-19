#include "SimpleFiringBehaviour.h"

SimpleFiringBehaviour::SimpleFiringBehaviour(){}

SimpleFiringBehaviour::~SimpleFiringBehaviour(){}

std::list<Bullet*> SimpleFiringBehaviour::FireRoundWithBehavior (const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction)
{
	std::list<Bullet*> bullets;
	Bullet* bullet = new Bullet(position, direction);

	bullets.push_back(bullet);
	return bullets;
}
