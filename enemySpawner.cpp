#include "enemy.h"
#include "main.h"
#include "manager.h"
#include "ShootType//threeBurstShoot.h"
#include "ShootType//singleShoot.h"
#include "Particle//particleEmitterEnemySpawn.h"
#include "EnemySpawner.h"

void EnemySpawner::Init()
{
	//�p�[�e�B�N����o�^
	AddComponent<ParticleEmitterEnemySpawn>();
}

void EnemySpawner::Uninit()
{
	for (auto p : m_Component)
	{
		p->Uninit();
	}
}

void EnemySpawner::Update()
{
	Scene* scene = Manager::GetScene();

	if (m_SpawnCount < SPAWN_NUM)//3��X�|�[������
	{
		if (m_SpawnFrame >= SPAWN_DELAY)
		{
			Spawn();
			m_SpawnFrame = 0;
			m_SpawnCount++;
		}
	}
	else
	{
		SetDestroy();
	}

	m_SpawnFrame++;

	for (auto p : m_Component)
	{
		p->Update();
	}
}

void EnemySpawner::Draw()
{
	for (auto p : m_Component)
	{
		p->Draw();
	}
}

//�G���X�|�[��������
void EnemySpawner::Spawn()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Phase phase = scene->GetPhase();

	//�t�F�[�Y�ɂ���ēG�̒e���Ⴄ
	switch (phase)
	{
	case Phase::PHASE1:
		scene->AddGameObject<Enemy>(LayerType::Type3D, new SingleShoot(),ENEMY_MAXHP, ENEMY_ATK)->SetPosition(m_Position);
		break;

	case Phase::PHASE2:
		scene->AddGameObject<Enemy>(LayerType::Type3D, new ThreeBurstShoot(), ENEMY_MAXHP, ENEMY_ATK)->SetPosition(m_Position);
		break;

	case Phase::PHASE3:		
		break;
	}
}