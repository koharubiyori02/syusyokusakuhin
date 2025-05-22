//===========================================================
//
//Å@íPî≠
// 
//===========================================================

#pragma once
#include "..//main.h"
#include "ShootType.h"

class BulletTypeFactory;

class SingleShoot : public ShootType
{
private:
	const int SHOOT_DELAY = 60;//î≠éÀä‘äu

public:

	void Shoot(XMFLOAT3 position, XMFLOAT3 velocity) override;
};
