#include "../manager.h"
#include "../player.h"
#include "../enemy.h"
#include "../box.h"
#include "../bulletEnemy.h"
#include "../collision.h"
#include "EnemyPrepare.h"
#include "EnemyShooting.h"
#include "EnemyMove.h"

void EnemyMove::Start()
{

}

void EnemyMove::Update()
{
	Scene* scene = Manager::GetScene();
	Enemy* enemy = GetEnemy();
	
	XMFLOAT3 enemyPos = enemy->GetPosition();
	XMVECTOR enemyVec = XMLoadFloat3(&enemyPos);

	XMFLOAT3 toPlayer = GoToPlayer(enemy,enemyVec);//�v���C���[�ւ̃x�N�g��
	XMFLOAT3 avoidObj= AvoidObj(enemyVec);//�I�u�W�F�N�g���������x�N�g��
	XMFLOAT3 avoidEnemy = AvoidEnemy(enemy,enemyVec);//�I�u�W�F�N�g���������x�N�g��

	XMFLOAT3 total{};//��̃x�N�g���𑫂����g�[�^���̃x�N�g��

	//�e�x�N�g���̏d��
	float w1 = 1.0f;
	float w2 = 1.5f;
	float w3 = 1.0f;

	//����x�N�g��������ꍇ�A�v���C���[�ւ̃x�N�g���̏d�݂����炷
	if (avoidObj.x != 0.0f|| avoidObj.z != 0.0f)
	{
		w1 = 0.05f;
	}

	//����
	total.x = toPlayer.x * w1 + avoidObj.x * w2 + avoidEnemy.x * w3;
	total.y = toPlayer.y * w1 + avoidObj.y * w2 + avoidEnemy.y * w3;
	total.z = toPlayer.z * w1 + avoidObj.z * w2 + avoidEnemy.z * w3;

	enemyPos.x += total.x * 0.13f;
	enemyPos.y += total.y * 0.13f;
	enemyPos.z += total.z * 0.13f;

	if (enemyPos.z <= -FIELD_MAX)
	{
		enemyPos.z = -FIELD_MAX;
	}
	if (enemyPos.z >= FIELD_MAX)
	{
		enemyPos.z = FIELD_MAX;
	}
	if (enemyPos.x <= -FIELD_MAX)
	{
		enemyPos.x = -FIELD_MAX;
	}
	if (enemyPos.x >= FIELD_MAX)
	{
		enemyPos.x = FIELD_MAX;
	}

	float rotation = atan2(toPlayer.x, toPlayer.z);
	enemy->SetRotation(XMFLOAT3(0, rotation, 0));
	enemy->SetPosition(enemyPos);

	return;
}

void EnemyMove::ImGuiDebug()
{

}

//�v���C���[�֌������x�N�g�������߂�
XMFLOAT3 EnemyMove::GoToPlayer(Enemy* enemy,XMVECTOR enemyVec)
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	XMFLOAT3 playerPos = player->GetPosition();
	XMVECTOR playerVec = XMLoadFloat3(&playerPos);

	XMVECTOR subtract = XMVectorSubtract(playerVec, enemyVec);//�v���C���[�ƓG�̃x�N�g���̌��Z
	
	XMFLOAT3 vec = GetVec(subtract);

	float dist = XMVectorGetX(XMVector3Length(subtract));//�v���C���[�ƓG�̋���

	if (m_Range <= dist)
	{
		return vec;
	}
	else
	{
		enemy->SetEnemyState<EnemyPrepare>();
	}

	return XMFLOAT3(0, 0, 0);
}

//��Q�����������x�N�g�������߂�
XMFLOAT3 EnemyMove::AvoidObj(XMVECTOR enemyVec)
{
	Scene* scene = Manager::GetScene();
	auto objects = scene->GetGameObjects<Box>();

	for (int i = 0; i < objects.size(); i++)
	{
		XMFLOAT3 objPos = objects[i]->GetPosition();
		XMVECTOR objVec = XMLoadFloat3(&objPos);
		XMVECTOR subtract = XMVectorSubtract(enemyVec, objVec);//�I�u�W�F�N�g����G�̃x�N�g��

		float dist = XMVectorGetX(XMVector3Length(subtract));//�I�u�W�F�N�g�ƓG�̋���

		XMFLOAT3 avoidance = GetVec(subtract);

		float avoidanceDist = 4.0f;//�������

		if (!m_IsAvoidObj)//������Ă��Ȃ�������
		{
			if (dist < avoidanceDist)//��������ɂȂ�����
			{
				m_IsAvoidObj = true;

				return avoidance;
			}
			else
			{
				if (i == objects.size() - 1)
				{
					return XMFLOAT3(0, 0, 0);
				}
				else
				{
					continue;
				}
			}
		}
		else//������Ă�����
		{
			if (dist > avoidanceDist + 1.0f)//���������菭���O���ɂȂ�����
			{
				m_IsAvoidObj = false;
			}

			return avoidance;			
		}
	}

	return XMFLOAT3(0, 0, 0);
}

//�G���m�̕����x�N�g�������߂�
XMFLOAT3 EnemyMove::AvoidEnemy(Enemy* enemy, XMVECTOR enemyVec)
{
	Scene* scene = Manager::GetScene();
	auto enemies = scene->GetGameObjects<Enemy>();

	XMVECTOR avoidVec = XMVectorZero();
	int count = 0;

	for (auto &other : enemies)
	{
		if (other != enemy)
		{
			XMFLOAT3 otherPos = other->GetPosition();
			XMVECTOR otherVec = XMLoadFloat3(&otherPos);
			XMVECTOR subtract = XMVectorSubtract(enemyVec, otherVec);

			XMFLOAT3 vec = GetVec(subtract);

			float dist= XMVectorGetX(XMVector3Length(subtract));//�I�u�W�F�N�g�ƓG�̋���
			
			if (dist < m_ContactRange)
			{
				avoidVec = XMVectorAdd(avoidVec, XMVector3Normalize(XMVectorSubtract(enemyVec, otherVec)));
				count++;
			}
		}
	}

	if (count > 0)
	{
		avoidVec = XMVectorScale(avoidVec, 1.0f / count);
		XMFLOAT3 avoid;
		XMStoreFloat3(&avoid, XMVector3Normalize(avoidVec));

		return avoid;
	}
	else
	{
		return XMFLOAT3(0, 0, 0);
	}
}

//�P�ʃx�N�g����XMFLOAT3�^�ɂ��ĕԂ�
XMFLOAT3 EnemyMove::GetVec(XMVECTOR subtract)
{
	XMVECTOR vec = XMVector3Normalize(subtract);
	XMFLOAT3 totalVec;
	XMStoreFloat3(&totalVec, vec);

	return totalVec;
}
