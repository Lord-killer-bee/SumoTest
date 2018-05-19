#include "FlakCannonFiringBehaviour.h"
#include "Maths.h"

FlakCannonFiringBehaviour::FlakCannonFiringBehaviour(){}

FlakCannonFiringBehaviour::~FlakCannonFiringBehaviour(){}

std::list<Bullet*> FlakCannonFiringBehaviour::FireRoundWithBehavior (const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction)
{
	std::list<Bullet*> bullets;

	float angle2 = Maths::DegreesToRadians(90.0f);
	float angle3 = Maths::DegreesToRadians(180.0f);
	float angle4 = -angle2; 

	Bullet* bullet1 = new Bullet(position, direction);

	D3DXVECTOR3 rightDirection = D3DXVECTOR3(direction.x * cos(angle2) + direction.y * sin(angle2), -direction.x * sin(angle2) + direction.y * cos(angle2), direction.z);
	Bullet* bullet2 = new Bullet(position, rightDirection);

	D3DXVECTOR3 bottomDirection = D3DXVECTOR3(direction.x * cos(angle3) + direction.y * sin(angle3), -direction.x * sin(angle3) + direction.y * cos(angle3), direction.z);
	Bullet* bullet3 = new Bullet(position, bottomDirection);

	D3DXVECTOR3 leftDirection = D3DXVECTOR3(direction.x * cos(angle4) + direction.y * sin(angle4), -direction.x * sin(angle4) + direction.y * cos(angle4), direction.z);
	Bullet* bullet4 = new Bullet(position, leftDirection);

	bullets.push_back(bullet1);
	bullets.push_back(bullet2);
	bullets.push_back(bullet3);
	bullets.push_back(bullet4);
	return bullets;
}
