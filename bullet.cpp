#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "bullet.h"
#include "manager.h"
#include "collision.h"
#include "box.h"
#include "Boss1.h"
#include "effectManager.h"

void Bullet::Init()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	XMFLOAT3 forward = player->GetForward();

	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Bullet::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Bullet::Update()
{
	if (!m_Active)
		return;

	Scene* scene = Manager::GetScene();

	//�e�̌����Ă�������Ɉړ�������
	m_Position.x += m_Velocity.x * m_Speed;
	m_Position.y += m_Velocity.y * m_Speed;
	m_Position.z += m_Velocity.z * m_Speed;

	int atkUpValue = 10;

	m_Atk = BULLET_DEFAULT_ATK + (m_AtkUpCount * atkUpValue);

	//�G�Ƃ̏Փ˔���
	auto enemyList = scene->GetGameObjects<Enemy>();

	XMFLOAT3 enemyHitScale = { 1.0f,2.0f,1.0f };

	for (auto p : enemyList)
	{
		if (Collision::GetInstance().OBBCollision(
			m_Position, p->GetPosition(), p->GetForward(), p->GetUp(), p->GetRight(), enemyHitScale))
		{
			if (m_Penetrate)
			{
				//�ђʃ��X�g�ɂ����ꍇ�̓X���[����
				if (std::find(m_PenetrateList.begin(), m_PenetrateList.end(), p) != m_PenetrateList.end())
					continue;
			}
			else
			{
				m_Active = false;
			}

			Manager::GetAudio()->PlaySE(AudioName::SE_BulletHit);

			//�G�t�F�N�g���o��
			Explosion* explosion = EffectManager::AccessExplosion();
			explosion->SetPosition(m_Position);
			scene->AddNoInit(explosion);

			p->AddDamage(m_Atk);
			//m_Active = false;

			if (p->GetHp() <= 0)//�G��|������L���J�E���g�𑫂��ēG������
			{
				scene->GetGameObject<Player>()->KillCountPlus();
				p->SetDestroy();
			}		
			else
			{
				m_PenetrateList.push_back(p);//�ђʃ��X�g�ɒǉ�����
			}
		}
	}

	//�{�X�Ƃ̓����蔻��
	Boss1* boss = scene->GetGameObject<Boss1>();

	XMFLOAT3 bossHitScale = { 3.0f, 4.0f, 3.0f };

	if (boss && !boss->GetIsDeath())//�{�X�����݂��邩�A����ł��Ȃ��ꍇ
	{
		if (Collision::GetInstance().OBBCollision(
			m_Position, boss->GetPosition(), boss->GetForward(), boss->GetUp(), boss->GetRight(), bossHitScale))
		{
			if (m_Penetrate)
			{
				//�ђʃ��X�g�ɂ��Ȃ��ꍇ
				if (std::find(m_PenetrateList.begin(), m_PenetrateList.end(), boss) == m_PenetrateList.end())
				{
					//�G�t�F�N�g���o��
					Explosion* explosion = EffectManager::AccessExplosion();
					explosion->SetPosition(m_Position);
					scene->AddNoInit(explosion);

					boss->AddDamage(m_Atk);

					m_PenetrateList.push_back(boss);//�ђʃ��X�g�ɒǉ�����
				}
			}
			else
			{
				boss->AddDamage(m_Atk);

				m_Active = false;
			}

			Manager::GetAudio()->PlaySE(AudioName::SE_BulletHit);
		}
	}

	auto box = scene->GetGameObjects<Box>();

	for (auto p : box)
	{
		//��Q���Ɠ������Ă�����e������
		if (Collision::GetInstance().OBBCollision(
			m_Position, p->GetPosition(), p->GetForward(), p->GetUp(), p->GetRight(), p->GetScale()))
		{
			//�G�t�F�N�g���o��
			Explosion* explosion = EffectManager::AccessExplosion();
			explosion->SetPosition(m_Position);
			scene->AddNoInit(explosion);
			
			m_Active = false;
		}
	}

	//��O�ɏo�������
	if (m_Position.x <= -FIELD_SIZE || m_Position.x >= FIELD_SIZE || m_Position.z <= -FIELD_SIZE || m_Position.z >= FIELD_SIZE)
	{
		m_Active = false;
	}

	if (!m_Active)
	{
		m_SpeedUpCount = 0;
		m_AtkUpCount = 0;

		m_PenetrateList.clear();

		SetRemove(true);
	}
}

void Bullet::Draw()
{
	if (!m_Active)
		return;

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);
	
	for (auto p : m_Component)
	{
		p->Draw();
	}
}

void Bullet::InitBulletStatus()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	XMFLOAT3 playerPos = player->GetPosition();

	m_Velocity = player->GetForward();

	m_Position = XMFLOAT3(playerPos.x, playerPos.y + 1.0f, playerPos.z);

	//�������ꂽ�Ƃ��ɍU����UP�A�C�e���̎擾���ɉ����čU���͂��Z�b�g
	m_Atk = BULLET_DEFAULT_ATK + (m_AtkUpCount * 10);

	//�������ꂽ�Ƃ��ɃX�s�[�hUP�A�C�e���̎擾���ɉ����ăX�s�[�h���Z�b�g
	//m_Speed = BULLET_DEFAULT_SPEED + (m_SpeedUpCount * m_SpeedUpValue);


	m_Active = true;
}
