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

	XMFLOAT3 toPlayer = GoToPlayer(enemy,enemyVec);//プレイヤーへのベクトル
	XMFLOAT3 avoidObj= AvoidObj(enemyVec);//オブジェクトを回避するベクトル
	XMFLOAT3 avoidEnemy = AvoidEnemy(enemy,enemyVec);//オブジェクトを回避するベクトル

	XMFLOAT3 total{};//上のベクトルを足したトータルのベクトル

	//各ベクトルの重み
	float w1 = 1.0f;
	float w2 = 1.5f;
	float w3 = 1.0f;

	//回避ベクトルがある場合、プレイヤーへのベクトルの重みを減らす
	if (avoidObj.x != 0.0f|| avoidObj.z != 0.0f)
	{
		w1 = 0.05f;
	}

	//足す
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

//プレイヤーへ向かうベクトルを求める
XMFLOAT3 EnemyMove::GoToPlayer(Enemy* enemy,XMVECTOR enemyVec)
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	XMFLOAT3 playerPos = player->GetPosition();
	XMVECTOR playerVec = XMLoadFloat3(&playerPos);

	XMVECTOR subtract = XMVectorSubtract(playerVec, enemyVec);//プレイヤーと敵のベクトルの減算
	
	XMFLOAT3 vec = GetVec(subtract);

	float dist = XMVectorGetX(XMVector3Length(subtract));//プレイヤーと敵の距離

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

//障害物を回避するベクトルを求める
XMFLOAT3 EnemyMove::AvoidObj(XMVECTOR enemyVec)
{
	Scene* scene = Manager::GetScene();
	auto objects = scene->GetGameObjects<Box>();

	for (int i = 0; i < objects.size(); i++)
	{
		XMFLOAT3 objPos = objects[i]->GetPosition();
		XMVECTOR objVec = XMLoadFloat3(&objPos);
		XMVECTOR subtract = XMVectorSubtract(enemyVec, objVec);//オブジェクトから敵のベクトル

		float dist = XMVectorGetX(XMVector3Length(subtract));//オブジェクトと敵の距離

		XMFLOAT3 avoidance = GetVec(subtract);

		float avoidanceDist = 4.0f;//回避距離

		if (!m_IsAvoidObj)//回避していなかったら
		{
			if (dist < avoidanceDist)//回避距離になったら
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
		else//回避していたら
		{
			if (dist > avoidanceDist + 1.0f)//回避距離より少し外側になったら
			{
				m_IsAvoidObj = false;
			}

			return avoidance;			
		}
	}

	return XMFLOAT3(0, 0, 0);
}

//敵同士の分離ベクトルを求める
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

			float dist= XMVectorGetX(XMVector3Length(subtract));//オブジェクトと敵の距離
			
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

//単位ベクトルをXMFLOAT3型にして返す
XMFLOAT3 EnemyMove::GetVec(XMVECTOR subtract)
{
	XMVECTOR vec = XMVector3Normalize(subtract);
	XMFLOAT3 totalVec;
	XMStoreFloat3(&totalVec, vec);

	return totalVec;
}
