//===========================================================
//
//�@�������̊��N���X
// 
//===========================================================

#pragma once
#include "..//main.h"

class ShootType
{
private:	

protected:
	int m_ShootDelayCount = 0;//���˃f�B���C�̃J�E���g
	int m_FastShootDelay = 0;//�ŏ��Ɍ��܂ł̃f�B���C�̃J�E���g
	bool m_ShootFlag = false;//���˂ł����Ԃ��ǂ���
	
public:

	virtual void Shoot(XMFLOAT3 position,XMFLOAT3 velocity) = 0;//����

	void SetFlag(const bool flag) { m_ShootFlag = flag; }
};