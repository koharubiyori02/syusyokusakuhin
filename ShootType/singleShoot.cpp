#include "../main.h"
#include "SingleShoot.h"
#include "../manager.h"
#include "../BulletEnemy.h"
#include "../bulletManager.h"

void SingleShoot::Shoot(XMFLOAT3 position, XMFLOAT3 velocity)
{
	if (m_ShootFlag)//”­ŽË‚Å‚«‚éó‘Ô‚¾‚Á‚½‚ç”­ŽË
	{
		Scene* scene = Manager::GetScene();
		BulletEnemy* bullet = BulletManager::AccessBulletEnemy();
		bullet->SetPosition(XMFLOAT3(position.x,position.y,position.z));
		bullet->SetVelocity(velocity);

		scene->AddNoInit(bullet);

		m_ShootFlag = false;
	}

	m_ShootDelayCount++;

	if (m_ShootDelayCount >= SHOOT_DELAY)
	{
		m_ShootDelayCount = 0;
		m_ShootFlag = true;
	}

#ifdef _DEBUG
	ImGui::Text("%f", m_ShootDelayCount);
#endif
}
