#include "manager.h"
#include "player.h"
#include "playerShoot.h"

void PlayerShoot::Shoot(XMFLOAT3 position)
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	if (!m_IsShooting)//�e�������Ă��Ȃ��Ƃ�
	{
		Manager::GetAudio()->PlaySE(AudioName::SE_Bullet);//���˂�SE
		Manager::GetAudio()->SetVolume(AudioName::SE_Bullet, 0.5f);

		player->GetBulletFactory()->CreateBullet(XMFLOAT3(position.x, position.y + 0.5f, position.z));//�e�𐶐�
		m_IsShooting = true;//�����Ă����Ԃɂ���
	}
}

void PlayerShoot::PlusCount()
{
	if (m_IsShooting)//�����Ă���Ƃ�
		m_Counter++;

	if (m_Counter >= m_ShootDelay)//���ˊԊu�ɃJ�E���g�����B�����猂�Ă�悤�ɂ���
	{
		m_IsShooting = false;
		m_Counter = 0.0f;
	}
}
