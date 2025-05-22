#include "PlayerBullet/atkUpDecorator.h"
#include "PlayerBullet/speedUpDecorator.h"
#include "PlayerBullet/penetrateDecorator.h"
#include "decoratorManager.h"

std::vector<AtkUpDecorator*> DecoratorManager::m_AtkUpPool;
std::vector<SpeedUpDecorator*> DecoratorManager::m_SpeedUpPool;
std::vector<PenetrateDecorator*> DecoratorManager::m_PenetratePool;

void DecoratorManager::InitDecorator()
{
	for (int i = 0; i < ATKUP_MAX; i++)
	{
		AtkUpDecorator* atkDeco = new AtkUpDecorator;
		m_AtkUpPool.push_back(atkDeco);
	}

	for (int j = 0; j < SPEEDUP_MAX; j++)
	{
		SpeedUpDecorator* speedDeco = new SpeedUpDecorator;
		m_SpeedUpPool.push_back(speedDeco);
	}

	for (int k = 0; k < PENETRATE_MAX; k++)
	{
		PenetrateDecorator* penetrateDeco = new PenetrateDecorator;
		penetrateDeco->AddComponent<ParticleEmitterBullet>();
		m_PenetratePool.push_back(penetrateDeco);
	}
}

void DecoratorManager::UninitDecorator()
{
	for (auto p : m_AtkUpPool)
	{
		p->SetDestroy();
	}

	for (auto p : m_SpeedUpPool)
	{
		p->SetDestroy();
	}

	for (auto p : m_PenetratePool)
	{
		p->SetDestroy();
	}
}

AtkUpDecorator* DecoratorManager::AccessAtkUpDecorator()
{
	for (auto p : m_AtkUpPool)
	{
		if (!p->GetActive())
		{
			return p;
		}
	}

	AtkUpDecorator* decorator = new AtkUpDecorator;
	decorator->Init();
	m_AtkUpPool.push_back(decorator);

	return decorator;
}

SpeedUpDecorator* DecoratorManager::AccessSpeedUpDecorator()
{
	for (auto p : m_SpeedUpPool)
	{
		if (!p->GetActive())
		{
			return p;
		}
	}

	SpeedUpDecorator* decorator = new SpeedUpDecorator;
	decorator->Init();
	m_SpeedUpPool.push_back(decorator);

	return decorator;
}

PenetrateDecorator* DecoratorManager::AccessPenetrateDecorator()
{
	for (auto p : m_PenetratePool)
	{
		if (!p->GetActive())
		{
			return p;
		}
	}

	PenetrateDecorator* decorator = new PenetrateDecorator;
	decorator->Init();
	m_PenetratePool.push_back(decorator);

	return decorator;
}
