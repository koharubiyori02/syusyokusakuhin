//===========================================================
//
//　貫通アイテムのデコレーター処理部分
// 
//===========================================================

#pragma once
#include "BulletDecorator.h"
#include "..//Particle//particleEmitterBullet.h"

class PenetrateDecorator : public BulletDecorator
{
public:
	void Init() override
	{
		GetBaseBullet()->SetPenetrate(true);
	}

	void Uninit() override
	{
		m_PlayerBullet->Uninit();
		for (auto p : m_Component)
		{
			p->Uninit();
		}
	}

	void Update() override
	{
		m_Position = GetBaseBullet()->GetPosition();

		m_PlayerBullet->Update();
		if (!m_PlayerBullet->GetActive())
		{
			GetComponent<ParticleEmitterBullet>()->ResetParticle();
			m_Active = false;
			m_Remove = true;
		}
		else
		{
			for (auto p : m_Component)
			{
				p->Update();
			}
		}
	}

	void Draw() override
	{
		m_PlayerBullet->Draw();
		for (auto p : m_Component)
		{
			p->Draw();
		}
	}
};