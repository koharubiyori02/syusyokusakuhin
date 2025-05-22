//===========================================================
//
//　撃ち方の基底クラス
// 
//===========================================================

#pragma once
#include "..//main.h"

class ShootType
{
private:	

protected:
	int m_ShootDelayCount = 0;//発射ディレイのカウント
	int m_FastShootDelay = 0;//最初に撃つまでのディレイのカウント
	bool m_ShootFlag = false;//発射できる状態かどうか
	
public:

	virtual void Shoot(XMFLOAT3 position,XMFLOAT3 velocity) = 0;//発射

	void SetFlag(const bool flag) { m_ShootFlag = flag; }
};