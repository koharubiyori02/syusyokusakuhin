#include "../manager.h"
#include "../player.h"
#include "../enemy.h"
#include "../bulletEnemy.h"
#include "EnemyMove.h"
#include "EnemyShooting.h"

void EnemyShooting::Start()
{
	
}

void EnemyShooting::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Enemy* enemy = GetEnemy();

	XMFLOAT3 pos = enemy->GetPosition();
	
	//���g�ƃv���C���[�̋���
	float distance = GetDistance(player->GetPosition().x, enemy->GetPosition().x, player->GetPosition().z, enemy->GetPosition().z);	

	XMFLOAT3 forward = enemy->GetForward();
	float vecX = player->GetPosition().x - enemy->GetPosition().x;
	float vecZ = player->GetPosition().z - enemy->GetPosition().z;

	vecX /= distance;
	vecZ /= distance;
	float rotation = atan2(vecX, vecZ);

	enemy->SetRotation(XMFLOAT3(0, rotation, 0));//��Ƀv���C���[�Ɍ����悤�ɂ���
	
	enemy->Shoot();//�e������

	
	if (m_MoveDist <= distance)//�˒��O�Ƀv���C���[�����ꂽ��ړ��X�e�[�g�֑J�ڂ���
	{
		enemy->SetEnemyState<EnemyMove>();
	}
}

void EnemyShooting::Exit()
{
	
}
