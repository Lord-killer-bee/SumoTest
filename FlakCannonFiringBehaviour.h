#ifndef FLAKCANNON_H_INCLUDED
#define FLAKCANNON_H_INCLUDED

#include "IWeaponBehaviour.h"

class FlakCannonFiringBehaviour : public IWeaponBehaviour
{
public:
	FlakCannonFiringBehaviour();
	~FlakCannonFiringBehaviour();

	std::list<Bullet*> FireRoundWithBehavior(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction) override;
};

#endif //FLAKCANNON_H_INCLUDED