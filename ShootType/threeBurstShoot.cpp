#include "..//manager.h"
#include "..//BulletEnemy.h"
#include "ThreeBurstShoot.h"
#include "../bulletManager.h"

void ThreeBurstShoot::Shoot(XMFLOAT3 position, XMFLOAT3 velocity)
{
	if (m_ShootFlag)//弾を打てる状態なら
	{
		if (m_ShootDelayCount % 10 == 0)//10フレームごとに撃つ
		{
			Scene* scene = Manager::GetScene();
			BulletEnemy* bullet = BulletManager::AccessBulletEnemy();
			bullet->SetPosition(XMFLOAT3(position.x, position.y, position.z));
			bullet->SetVelocity(velocity);

			scene->AddNoInit(bullet);
		}

		m_ShootDelayCount++;

		if (m_ShootDelayCount == SHOOT_NUM)//3回打ったら
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

	if (m_FastShootDelay >= SHOOT_DELAY)//初回の発射ディレイが終わったら
	{
		m_FastShootDelay = 0;
		m_ShootFlag = true;
	}
}
