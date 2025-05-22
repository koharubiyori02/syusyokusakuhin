#include "manager.h"
#include "bullet.h"
#include "bulletEnemy.h"
#include "bulletManager.h"

std::vector<Bullet*> BulletManager::m_PlayerBulletPool;
std::vector<BulletEnemy*> BulletManager::m_EnemyBulletPool;

void BulletManager::InitBullet()
{
	Scene* scene = Manager::GetScene();

	for (int i = 0; i < PLAYER_BULLET_MAX; i++)
	{
		Bullet* bullet = new Bullet;
		bullet->Init();
		m_PlayerBulletPool.push_back(bullet);
	}

	for (int j = 0; j < ENEMY_BULLET_MAX; j++)
	{
		BulletEnemy* bullet = new BulletEnemy;
		bullet->Init();
		m_EnemyBulletPool.push_back(bullet);
	}
}

void BulletManager::UninitBullet()
{
	for (auto p : m_PlayerBulletPool)
	{
		p->SetDestroy();
	}

	for (auto p : m_EnemyBulletPool)
	{
		p->SetDestroy();
	}
}

Bullet* BulletManager::AccessBullet()
{
	for (auto p : m_PlayerBulletPool)
	{
		if (p != nullptr)
		{
			if (!p->GetActive())
			{
				return p;
			}
		}
	}

	Bullet* bullet = new Bullet;
	bullet->Init();
	m_PlayerBulletPool.push_back(bullet);

	return bullet;
}

BulletEnemy* BulletManager::AccessBulletEnemy()
{
	for (auto p : m_EnemyBulletPool)
	{
		if (p != nullptr)
		{
			if (!p->GetActive())
			{
				p->SetActive(true);
				return p;
			}
		}
	}

	BulletEnemy* bullet = new BulletEnemy;
	bullet->Init();
	m_EnemyBulletPool.push_back(bullet);

	return bullet;
}

void BulletManager::ResetBullet()
{
	Scene* scene = Manager::GetScene();
	auto bullets = scene->GetGameObjects<Bullet>();

	for (auto p : bullets)
	{
		p->SetActive(false);
		p->SetRemove(true);
	}

	auto enemyBullets = scene->GetGameObjects<BulletEnemy>();

	for (auto p : enemyBullets)
	{
		p->SetActive(false);
		p->SetRemove(true);
	}
}
