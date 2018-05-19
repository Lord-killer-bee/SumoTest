#ifndef SIMPLEFIRING_H_INCLUDED
#define SIMPLEFIRING_H_INCLUDED

#include "IWeaponBehaviour.h"

class SimpleFiringBehaviour : public IWeaponBehaviour
{
public:
	SimpleFiringBehaviour();
	~SimpleFiringBehaviour();

	std::list<Bullet*> FireRoundWithBehavior(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction) override;
};

#endif //SIMPLEFIRING_H_INCLUDED