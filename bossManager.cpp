#include "bossManager.h"
#include "manager.h"
#include "boss1.h"

Boss1* BossManager::m_Boss1;

void BossManager::InitBoss()
{
	if (!m_Boss1)
	{
		m_Boss1 = new Boss1;
		m_Boss1->Init();
	}
}

void BossManager::UninitBoss()
{
	if (m_Boss1)
	{
		m_Boss1->SetDestroy();
	}
}

//ƒ{ƒX1‚ð•Ô‚·
Boss1* BossManager::AccessBoss1()
{
	m_Boss1->SetActive(true);
	m_Boss1->ResetBoss();
	return m_Boss1;
}
