//===========================================================
//
//�@�A�C�e���̃f�R���[�^�[��������
// 
//===========================================================

#pragma once
#include "..//bullet.h"

class BulletDecorator : public Bullet
{
protected:
	Bullet* m_PlayerBullet;//�e

public:

	void SetBullet(Bullet* bullet)
	{
		m_PlayerBullet = bullet;
	}

	void Uninit() override
	{
		m_PlayerBullet->Uninit();
	}

	void Update() override 
	{
		m_PlayerBullet->Update();

		if (m_PlayerBullet->GetDestroy())
			SetDestroy();
	}

	void Draw() override 
	{
		m_PlayerBullet->Draw();
	}

	void SetParentBullet(Bullet* bullet)
	{
		m_PlayerBullet = bullet;
	}

	void ResetDecorator()//�v�[���ɖ߂������̏�����
	{
		m_Active = false;
		m_Remove = false;

		BulletDecorator* decorator = dynamic_cast<BulletDecorator*>(m_PlayerBullet);
		if (decorator)
		{
			decorator->ResetDecorator();
		}
		else
		{
			m_PlayerBullet->SetActive(false);
			m_PlayerBullet->SetRemove(false);
			m_PlayerBullet->SetPenetrate(false);
		}

		/*m_PlayerBullet->SetActive(false);
		m_PlayerBullet->SetRemove(false);*/
	}

	Bullet* GetBaseBullet() override { return m_PlayerBullet->GetBaseBullet(); }//��{�̒e���擾
};