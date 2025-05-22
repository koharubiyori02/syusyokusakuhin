#include "..//manager.h"
#include "..//BulletEnemy.h"
#include "ThreeBurstShoot.h"
#include "../bulletManager.h"

void ThreeBurstShoot::Shoot(XMFLOAT3 position, XMFLOAT3 velocity)
{
	if (m_ShootFlag)//�e��łĂ��ԂȂ�
	{
		if (m_ShootDelayCount % 10 == 0)//10�t���[�����ƂɌ���
		{
			Scene* scene = Manager::GetScene();
			BulletEnemy* bullet = BulletManager::AccessBulletEnemy();
			bullet->SetPosition(XMFLOAT3(position.x, position.y, position.z));
			bullet->SetVelocity(velocity);

			scene->AddNoInit(bullet);
		}

		m_ShootDelayCount++;

		if (m_ShootDelayCount == SHOOT_NUM)//3��ł�����
		{
			m_ShootDelayCount = false;
			m_ShootDelayCount = 0;
			m_ShootFlag = false;
		}
	}
	else
	{
		m_FastShootDelay++;
	}

	if (m_FastShootDelay >= SHOOT_DELAY)//����̔��˃f�B���C���I�������
	{
		m_FastShootDelay = 0;
		m_ShootFlag = true;
	}
}
