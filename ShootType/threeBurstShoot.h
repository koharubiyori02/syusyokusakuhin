//===========================================================
//
//�@�O�A��
// 
//===========================================================

#pragma once
#include "..//main.h"
#include "shootType.h"

class BulletTypeFactory;

class ThreeBurstShoot : public ShootType
{
private:
	const int SHOOT_NUM = 30;	//3�A�˂Ȃ̂�3*10�t���[��
	const int SHOOT_DELAY = 40; //3�_�o�[�X�g���Ƃ̔��ˊԊu

public:

	void Shoot(XMFLOAT3 position, XMFLOAT3 velocity) override;
};
