#include "manager.h"
#include "player.h"
#include "playerShoot.h"

void PlayerShoot::Shoot(XMFLOAT3 position)
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	if (!m_IsShooting)//弾を撃っていないとき
	{
		Manager::GetAudio()->PlaySE(AudioName::SE_Bullet);//発射のSE
		Manager::GetAudio()->SetVolume(AudioName::SE_Bullet, 0.5f);

		player->GetBulletFactory()->CreateBullet(XMFLOAT3(position.x, position.y + 0.5f, position.z));//弾を生成
		m_IsShooting = true;//撃っている状態にする
	}
}

void PlayerShoot::PlusCount()
{
	if (m_IsShooting)//撃っているとき
		m_Counter++;

	if (m_Counter >= m_ShootDelay)//発射間隔にカウントが到達したら撃てるようにする
	{
		m_IsShooting = false;
		m_Counter = 0.0f;
	}
}
