#include "../manager.h"
#include "../input.h"
#include "result.h"
#include "../ShootType/singleShoot.h"
#include "../ShootType/threeBurstShoot.h"
#include "../PlayerBullet/penetrateAbility.h"
#include "../Items/speedUpItem.h"
#include "../EnemySpawner.h"
#include "../itemManager.h"
#include "../fade.h"
#include "../UI/UI.h"
#include "../UI/PhaseUI.h"
#include "../UI/EnemyNumUI.h"
#include "../UI/AbilityUI.h"
#include "../Particle/particleEmitterTutibokori.h"
#include "../Particle/particleEmitterEnemySpawn.h"
#include "../Scenes/Lose.h"
#include "../boss1.h"
#include "../box.h"
#include "../field.h"
#include "../sky.h"
#include "../enemy.h"
#include "../renderer.h"
#include "../postProcess.h"
#include "../polygon2D.h"
#include "../bulletManager.h"
#include "../bossManager.h"
#include "game.h"

void Game::Init()
{
	AddGameObject<Camera>(LayerType::TypeCamera);
	AddGameObject<Field>(LayerType::Type3D);
	AddGameObject<Player>(LayerType::Type3D, PLAYER_DEFAULT_HP);
	AddGameObject<Sky>(LayerType::Type3D);

	for (auto& pos : m_BoxPositions)
	{
		Box* box = AddGameObject<Box>(LayerType::Type3D);
		box->SetPosition(pos);
		box->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
	}

	AddGameObject<ItemManager>(LayerType::Type3D)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));
	AddGameObject<Fade>(LayerType::Type2D, FadeType::FadeIn);
	AddGameObject<UI>(LayerType::Type2D);
	AddGameObject<AbilityUI>(LayerType::Type2D);

	AddGameObject<Polygon2D>(LayerType::Type2D, L"asset\\texture\\target.png", XMFLOAT3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f),
		400.0f, 400.0f, false, "target");

	AddGameObject<PostProcess>(LayerType::Type2D);

	BossManager::GetInstance()->InitBoss();

	AudioManager* audio = Manager::GetAudio();

	audio->StopAll();
	audio->Play(AudioName::BGM_Game);
}

void Game::Update()
{
	Scene::Update();
	
	Player* player = GetGameObject<Player>();

	std::vector<Enemy*> enemyList = GetGameObjects<Enemy>();

	Fade* fade = GetGameObject<Fade>();

	//�t�F�[�Y�ɂ���ď�����ς���
	switch (m_Phase)
	{
	case Phase::NONE:
		m_Phase = Phase::PHASE1;
		ResetGame();
		break;

	case Phase::PHASE1:
		if (player->GetKillCount() == ENEMY_MAX1)
		{
			player->ResetKillCount();
			m_Phase = Phase::PHASE2;
			ResetGame();
		}
		break;

	case Phase::PHASE2:
		if (player->GetKillCount() == ENEMY_MAX2)
		{
			player->ResetKillCount();
			m_Phase = Phase::PHASE3;
			ResetGame();
			GetGameObject<UI>()->DeleteUI();//��ɕ\������Ă���UI������
			
			auto box = GetGameObjects<Box>();
			for (auto p : box)
			{
				p->SetDestroy();//��Q��������
			}
		}
		break;

	case Phase::PHASE3:
		Boss1* boss = GetGameObject<Boss1>();

		if (boss && boss->GetIsDeath())//�G�����񂾂�
		{
			fade->SetFade(FadeType::FadeOut);//�t�F�[�h�A�E�g����
		}
		if (!boss&&fade->GetFinishFade())//�t�F�[�h�A�E�g������������
		{
			BulletManager::ResetBullet();
			Manager::SetScene<Result>();//���U���g�V�[���֑J��
			fade->SetDestroy();//�t�F�[�h������
		}

		break;
	}
	if (player->GetHp()==0)//�v���C���[�����񂾂�
	{
		fade->SetFade(FadeType::FadeOut);//�t�F�[�h�A�E�g

		if (fade->GetFinishFade())//�t�F�[�h�A�E�g������������
		{
			BulletManager::ResetBullet();
			Manager::SetScene<Lose>();//�Q�[���I�[�o�[�V�[���֑J��
			fade->SetDestroy();//�t�F�[�h������
		}
	}	
}

void Game::Draw()
{
	Scene::Draw();

	if (!m_FadeFinish)//�e�L�X�g�̃t�F�[�h�����Ă��Ȃ�������
	{
		TextFade();
		//�Q�[���X�^�[�g���̃e�L�X�g
		Renderer::TextDrawFont(L"�X�^�[�g!", 500.0f, 200.0f, 1000.0f, 100.0f, L"Meiryo", 70.0f, XMFLOAT4(1.0f, 1.0f, 0.0f, m_Alpha));
	}
}

//���̃t�F�[�Y�̓G�����ׂē|������Ă�
void Game::ResetGame()
{
	//�t�F�[�Y�ɂ���ď�������G�̎�ނ�ς���
	switch (m_Phase)
	{
	case Phase::PHASE1:
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(-26.0f, 0.0f, 10.0f));
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(26.0f, 0.0f, 10.0f));
		break;

	case Phase::PHASE2:
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(-26.0f, 0.0f, 10.0f));
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(26.0f, 0.0f, 10.0f));
		AddGameObject<EnemySpawner>(LayerType::Type3D)->SetPosition(XMFLOAT3(40.0f, 0.0f, 10.0f));
		break;

	case Phase::PHASE3:
		AddNoInit(BossManager::AccessBoss1());
		break;		
	}
}
