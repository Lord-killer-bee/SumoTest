#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include <d3dx9math.h>
#include <list>
#include "Bullet.h"
#include "IWeaponBehaviour.h"
#include "SimpleFiringBehaviour.h"
#include "ConicalFiringBehaviour.h"
#include "FlakCannonFiringBehaviour.h"

class Weapon
{
public:
	Weapon();
	~Weapon();

	void InitializeWeapon(EWeaponType type);

	std::list<Bullet*> FireRound(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction);
	void SwitchWeapons(EWeaponType type);

private:

	EWeaponType currentWeaponBehaviour_;
	IWeaponBehaviour* weaponBehaviour_;
};

#endif // WEAPON_H_INCLUDED