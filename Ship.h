#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "GameEntity.h"
#include "Weapon.h"

class Graphics;

class Ship : public GameEntity
{
public:
	Ship();
	~Ship();

	void SetControlInput(float acceleration,
		float rotation);

	void Update(System *system);
	void Render(Graphics *graphics) const;

	D3DXVECTOR3 GetForwardVector() const;
	D3DXVECTOR3 GetVelocity() const;

	void DeductLives();
	int GetLives();
	void SetLives(int lives);
	void SetInvulnerability(bool status);

	void Reset();
	Weapon* GetAttatchedWeapon();

private:

	Weapon* CreateWeapon(EWeaponType type);

	Weapon* weapon_;
	int activeColor_, invulnerableColor_, vulnerableColor_;

	float accelerationControl_;
	float rotationControl_;

	D3DXVECTOR3 velocity_;
	D3DXVECTOR3 forward_;
	float rotation_;

	int lives_;
};

#endif // SHIP_H_INCLUDED
