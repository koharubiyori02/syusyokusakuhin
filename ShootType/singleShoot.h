//===========================================================
//
//�@�P��
// 
//===========================================================

#pragma once
#include "..//main.h"
#include "ShootType.h"

class BulletTypeFactory;

class SingleShoot : public ShootType
{
private:
	const int SHOOT_DELAY = 60;//���ˊԊu

public:

	void Shoot(XMFLOAT3 position, XMFLOAT3 velocity) override;
};
