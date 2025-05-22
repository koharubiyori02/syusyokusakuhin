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
	
	//自身とプレイヤーの距離
	float distance = GetDistance(player->GetPosition().x, enemy->GetPosition().x, player->GetPosition().z, enemy->GetPosition().z);	

	XMFLOAT3 forward = enemy->GetForward();
	float vecX = player->GetPosition().x - enemy->GetPosition().x;
	float vecZ = player->GetPosition().z - enemy->GetPosition().z;

	vecX /= distance;
	vecZ /= distance;
	float rotation = atan2(vecX, vecZ);

	enemy->SetRotation(XMFLOAT3(0, rotation, 0));//常にプレイヤーに向くようにする
	
	enemy->Shoot();//弾を撃つ

	
	if (m_MoveDist <= distance)//射程外にプレイヤーが離れたら移動ステートへ遷移する
	{
		enemy->SetEnemyState<EnemyMove>();
	}
}

void EnemyShooting::Exit()
{
	
}
