//===========================================================
//
//　攻撃力UPアイテムのデコレーター処理部分
// 
//===========================================================

#pragma once
#include "BulletDecorator.h"

class AtkUpDecorator : public BulletDecorator
{
public:
	void Init() override
	{
		GetBaseBullet()->PlusAtk();
	}

	void Uninit() override
	{
		m_PlayerBullet->Uninit();
	}

	void Update() override
	{
		m_PlayerBullet->Update();
		if (!m_PlayerBullet->GetActive())
		{
			m_Active = false;
			m_Remove = true;
		}
	}

	void Draw() override
	{
		m_PlayerBullet->Draw();
	}
};