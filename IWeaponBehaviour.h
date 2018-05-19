#ifndef IWEAPONBEHAVIOUR_H_INCLUDED
#define IWEAPONBEHAVIOUR_H_INCLUDED

#include <d3dx9math.h>
#include <list>
#include "Bullet.h"

/*class SimpleFiringBehaviour;
class ConicalFiringBehaviour;
class FlakCannonFiringBehaviour;*/

enum EWeaponType
{
	SIMPLE_FIRING,
	CONICAL_FIRING,
	FLAK_CANNON
};

class IWeaponBehaviour

{
public:
	virtual std::list<Bullet*> FireRoundWithBehavior(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction) = 0;
};

#endif // IWEAPONBEHAVIOUR_H_INCLUDED