#ifndef CONICALFIRING_H_INCLUDED
#define CONICALFIRING_H_INCLUDED

#include "IWeaponBehaviour.h"

class ConicalFiringBehaviour : public IWeaponBehaviour
{
public:
	ConicalFiringBehaviour();
	~ConicalFiringBehaviour();

	std::list<Bullet*> FireRoundWithBehavior(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction) override;
};

#endif //CONICALFIRING_H_INCLUDED