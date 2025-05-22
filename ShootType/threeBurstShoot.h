//===========================================================
//
//　三連射
// 
//===========================================================

#pragma once
#include "..//main.h"
#include "shootType.h"

class BulletTypeFactory;

class ThreeBurstShoot : public ShootType
{
private:
	const int SHOOT_NUM = 30;	//3連射なので3*10フレーム
	const int SHOOT_DELAY = 40; //3点バーストごとの発射間隔

public:

	void Shoot(XMFLOAT3 position, XMFLOAT3 velocity) override;
};
