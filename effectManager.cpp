#include "explosion.h"
#include "effectManager.h"

std::vector<Explosion*> EffectManager::m_ExplosionPool;

void EffectManager::InitEffect()
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		Explosion* explosion = new Explosion;
		explosion->Init();
		m_ExplosionPool.push_back(explosion);
	}
}

void EffectManager::UninitEffect()
{
	for (auto p : m_ExplosionPool)
	{
		p->SetDestroy();
	}
}

Explosion* EffectManager::AccessExplosion()
{
	for (auto p : m_ExplosionPool)
	{
		if (!p->GetActive())
		{
			p->SetActive(true);
			return p;
		}
	}

	Explosion* explosion = new Explosion;
	explosion->Init();
	m_ExplosionPool.push_back(explosion);

	return explosion;
}
