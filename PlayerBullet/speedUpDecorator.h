//===========================================================
//
//�@�X�s�[�hUP�A�C�e���̃f�R���[�^�[��������
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
		GetBaseBullet()->PlusSpeed();//�e���𑁂�����
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
