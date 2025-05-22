//===========================================================
//
//　スピードUPアイテムのデコレーター処理部分
// 
//===========================================================

#pragma once
#include "..//manager.h"
#include "..//Scenes//scene.h"
#include "..//player.h"
#include "BulletDecorator.h"

class SpeedUpDecorator : public BulletDecorator
{
public:

	void Init() override
	{
		Scene* scene = Manager::GetScene();
		Player* player = scene->GetGameObject<Player>();
		GetBaseBullet()->PlusSpeed();//弾速を早くする
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
