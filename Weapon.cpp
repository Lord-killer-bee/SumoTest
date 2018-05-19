#include "Weapon.h"

Weapon::Weapon(){}

Weapon::~Weapon()
{
	delete weaponBehaviour_;
}

void Weapon::InitializeWeapon(EWeaponType type)
{
	SwitchWeapons(type);
}

std::list<Bullet*> Weapon::FireRound(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction)
{
	return weaponBehaviour_->FireRoundWithBehavior(position, direction);
}

void Weapon::SwitchWeapons(EWeaponType type)
{
	switch (type)
	{
	case SIMPLE_FIRING:
		if(currentWeaponBehaviour_ != SIMPLE_FIRING)
		{		
			weaponBehaviour_ = new SimpleFiringBehaviour();
			currentWeaponBehaviour_= SIMPLE_FIRING;
		}
		break;

	case CONICAL_FIRING:
		if(currentWeaponBehaviour_ != CONICAL_FIRING)
		{	
			weaponBehaviour_ = new ConicalFiringBehaviour();
			currentWeaponBehaviour_= CONICAL_FIRING;
		}
		break;

	case FLAK_CANNON:
		if(currentWeaponBehaviour_ != FLAK_CANNON)
		{	
			weaponBehaviour_ = new FlakCannonFiringBehaviour();
			currentWeaponBehaviour_= FLAK_CANNON;
		}
		break;

	default:
		if(currentWeaponBehaviour_ != SIMPLE_FIRING)
		{
			weaponBehaviour_ = new SimpleFiringBehaviour();
			currentWeaponBehaviour_= SIMPLE_FIRING;
		}
		break;
	}
}