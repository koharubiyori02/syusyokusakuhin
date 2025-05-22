#include "../manager.h"
#include "../input.h"
#include "../Items/speedUpItem.h"
#include "PlayerState.h"

void PlayerState::Start()
{
	
}

void PlayerState::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = GetPlayer();

	if (m_PlayerShoot->GetIsShooting())//�����Ă����ԂȂ�f�B���C�̃J�E���g��i�߂�
	{
		m_PlayerShoot->PlusCount();
	}
}

void PlayerState::Exit()
{
	m_Player->SetDestroy();
}
