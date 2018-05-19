#include "ConicalFiringBehaviour.h"
#include "Maths.h"

ConicalFiringBehaviour::ConicalFiringBehaviour(){}

ConicalFiringBehaviour::~ConicalFiringBehaviour(){}

std::list<Bullet*> ConicalFiringBehaviour::FireRoundWithBehavior (const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction)
{
	std::list<Bullet*> bullets;

	float angle2 = Maths::DegreesToRadians(30.0f);
	float angle3 = -angle2;

	Bullet* bullet1 = new Bullet(position, direction);

	D3DXVECTOR3 rightDirection = D3DXVECTOR3(direction.x * cos(angle2) + direction.y * sin(angle2), -direction.x * sin(angle2) + direction.y * cos(angle2), direction.z);
	Bullet* bullet2 = new Bullet(position, rightDirection);

	D3DXVECTOR3 leftDirection = D3DXVECTOR3(direction.x * cos(angle3) + direction.y * sin(angle3), -direction.x * sin(angle3) + direction.y * cos(angle3), direction.z);
	Bullet* bullet3 = new Bullet(position, leftDirection);

	bullets.push_back(bullet1);
	bullets.push_back(bullet2);
	bullets.push_back(bullet3);
	return bullets;
}
