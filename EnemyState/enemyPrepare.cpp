#include "../manager.h"
#include "../player.h"
#include "../enemy.h"
#include "../bulletEnemy.h"
#include "EnemyMove.h"
#include "EnemyShooting.h"
#include "EnemyPrepare.h"

void EnemyPrepare::Start()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Enemy* enemy = GetEnemy();

	m_StartForward = enemy->GetRotation();//回転する前の向き

	//自身とプレイヤーの距離
	float distance = GetDistance(player->GetPosition().x, enemy->GetPosition().x, player->GetPosition().z, enemy->GetPosition().z);

	float vecX = player->GetPosition().x - enemy->GetPosition().x;
	float vecZ = player->GetPosition().z - enemy->GetPosition().z;

	vecX /= distance;
	vecZ /= distance;

	float rotation = atan2(vecX, vecZ);

	m_EndForward = XMFLOAT3(0, rotation, 0);//開店後の向きをセット

	m_Frame = 0.0f;
}

void EnemyPrepare::Update()
{
	float t = m_Frame / 10;

	m_Frame += 1.0f;

	float forwardY = m_StartForward.y + t * (m_EndForward.y - m_StartForward.y);
	GetEnemy()->SetRotation(XMFLOAT3(0, forwardY, 0));

	if (t >= 1.0f)//回り終わったらシューティングステートへ遷移する
	{
		GetEnemy()->SetEnemyState<EnemyShooting>();
	}
}
