#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "input.h"
#include "PlayerState//playerState.h"
#include "PlayerState//playerIdle.h"
#include "PlayerState//playerMove.h"
#include "animationModel.h"
#include "Particle//particleEmitterTutibokori.h"
#include "box.h"
#include "Items//item.h"
#include "bulletEnemy.h"
#include "ImGUI/imgui.h"
#include "Scenes/game.h"
#include "postProcess.h"
#include "player.h"
#include "effectManager.h"

void Player::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	PlayerShoot* shoot = new PlayerShoot;

	//�v���C���[�X�e�[�g�̏����l���Z�b�g
	PlayerIdle* playerIdle = new PlayerIdle(this,PlayerStateName::PlayerIdle,shoot);
	PlayerMove* playerMove = new PlayerMove(this,PlayerStateName::PlayerMove,shoot);

	playerIdle->SetActive(true);//�A�C�h����Ԃ��A�N�e�B�u�ɂ���

	m_States.emplace_back(playerIdle);
	m_States.emplace_back(playerMove);

	m_State = playerIdle;//�ŏ���Idle��Ԃɂ���
	m_State->Start();

	//�e�̃t�@�N�g���[�𐶐�
	m_BulletFactory = new BulletFactory;

	//���f���ƃA�j���[�V������o�^
	AddComponent<AnimationModel>()->Load("asset\\model\\Ely By K.Atienza.fbx");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Idle.fbx", "Idle");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Walk Forward.fbx", "Forward");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Walk Left.fbx", "Left");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Walk Right.fbx", "Right");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Walk Backward.fbx", "Back");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\player_Death.fbx", "Death");
	
	//�Q�[���V�[����������y����ǉ�
	if (dynamic_cast<Game*>(Manager::GetScene()))
	{
		AddComponent<ParticleEmitterTutibokori>();
	}

	m_Scale = DEFAULT_SCALE;
	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";
}

void Player::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	for (auto p : m_Component)
	{
		p->Uninit();
		delete p;
	}

	delete m_BulletFactory;

	for (auto p : m_States)
	{
		p->Exit();
		delete p;
	}
}

void Player::Update()
{
	if (!dynamic_cast<Title*>(Manager::GetScene()))//�^�C�g���V�[���łȂ��Ȃ�
	{
		if (m_Hp <= 0)//���񂾂Ƃ�
		{
			if (!m_IsDeath)
				m_AnimationFrame = 0;

			m_IsDeath = true;
			m_Speed = 0.0f;
			m_DeathCount++;

			m_AnimationName1 = m_AnimationName2;
			m_AnimationName2 = "Death";
			m_AnimationBlendRatio = 0.0f;

			int deathFrame = 80;

			if (m_DeathCount > deathFrame)
			{
				m_AnimationFrame--;
			}
		}


		//�O�t���[���̍��W��ۑ�
		XMFLOAT3 oldPos = m_Position;

		//�X�e�[�g�̍X�V
		m_State->Update();

		if (!m_IsDeath)
		{
			//�ړ�
			if (Input::GetKeyPress('W'))
			{
				if (m_AnimationName2 != "Forward")
				{
					m_AnimationName1 = m_AnimationName2;
					m_AnimationName2 = "Forward";
					m_AnimationBlendRatio = 0.0f;
				}
			}
			if (Input::GetKeyPress('A'))
			{
				if (m_AnimationName2 != "Left")
				{
					m_AnimationName1 = m_AnimationName2;
					m_AnimationName2 = "Left";
					m_AnimationBlendRatio = 0.0f;
				}
			}
			if (Input::GetKeyPress('D'))
			{
				if (m_AnimationName2 != "Right")
				{
					m_AnimationName1 = m_AnimationName2;
					m_AnimationName2 = "Right";
					m_AnimationBlendRatio = 0.0f;
				}
			}
			if (Input::GetKeyPress('S'))
			{
				if (m_AnimationName2 != "Back")
				{
					m_AnimationName1 = m_AnimationName2;
					m_AnimationName2 = "Back";
					m_AnimationBlendRatio = 0.0f;
				}
			}

			//�������͂���Ă��Ȃ�������Idle��Ԃɂ���
			if (!Input::GetKeyPress('W') && !Input::GetKeyPress('A') && !Input::GetKeyPress('D') && !Input::GetKeyPress('S'))
			{
				if (m_AnimationName2 != "Idle")
				{
					m_AnimationName1 = m_AnimationName2;
					m_AnimationName2 = "Idle";
					m_AnimationBlendRatio = 0.0f;
				}
				m_IsMoving = false;//�����Ă��Ȃ�
			}
			else
			{
				m_IsMoving = true;//�����Ă���
			}


			Scene* scene = Manager::GetScene();
			auto box = scene->GetGameObjects<Box>();

			//��Q���Ƃ̓����蔻��
			for (auto p : box)
			{
				//OBB�̓����蔻��
				if (Collision::GetInstance().OBBCollision(m_Position, p->GetPosition(), p->GetForward(), p->GetUp(), p->GetRight(), p->GetScale()))
				{
					//�������Ă�����O�t���[���ɍ��W���Z�b�g����
					m_Position = oldPos;
				}
				else
				{
					m_Speed = PLAYER_MOVESPEED;
				}
			}

			auto item = scene->GetGameObjects<Item>();

			//�A�C�e���Ƃ̓����蔻��
			for (auto p : item)
			{
				if (Collision::GetInstance().NormalCollision(m_Position, p->GetPosition()))
				{
					if (m_BulletFactory->GetDecorators().size() <= 4)
					{
						Manager::GetAudio()->PlaySE(AudioName::SE_Item);
						Manager::GetAudio()->SetVolume(AudioName::SE_Item, 1);

						//�������Ă�����\�͂�ǉ�����
						m_BulletFactory->AddDecorator(p->GetAbility());
						p->SetDestroy();
					}
				}
			}

			auto bullet = scene->GetGameObjects<BulletEnemy>();

			//�G�̒e�Ƃ̓����蔻��
			for (auto p : bullet)
			{
				if (Collision::GetInstance().OBBCollision(m_Position, p->GetPosition(), p->GetForward(), p->GetUp(), p->GetRight(), p->GetScale()))
				{
					//�������Ă�����v���C���[�Ƀ_���[�W������
					Manager::GetAudio()->PlaySE(AudioName::SE_BulletHit);

					scene->GetGameObject<PostProcess>()->SetActive(true);
					scene->GetGameObject<Camera>()->SetShakeActive(true);

					Explosion* explosion = EffectManager::AccessExplosion();
					explosion->SetPosition(m_Position);
					scene->AddNoInit(explosion);

					AddDamage(ENEMY_ATK);
					
					p->ResetBullet();
				}
			}
		}
	}

	//�R���|�[�l���g�̍X�V
	for (auto p : m_Component)
	{
		p->Update();
	}

	m_AnimationBlendRatio += 0.1f;
	if (m_AnimationBlendRatio > 1.0f)
		m_AnimationBlendRatio = 1.0f;
}

void Player::Draw()
{
	GetComponent<AnimationModel>()->Update(m_AnimationName1.c_str(), m_AnimationFrame, m_AnimationName2.c_str(), m_AnimationFrame, m_AnimationBlendRatio);
	m_AnimationFrame++;

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(0.0f, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	for (auto p : m_Component)
	{
		p->Draw();
	}
}

//�f�o�b�O�p
void Player::ImGuiDebug()
{
	if (ImGui::TreeNode("PlayerPos"))
	{
		ImGui::Text("x:%f", m_Position.x);
		ImGui::Text("y:%f", m_Position.y);
		ImGui::Text("z:%f", m_Position.z);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerState"))
	{
		ImGui::Text("idle:%d", m_States[0]);
		ImGui::Text("move:%d", m_States[1]);
		ImGui::TreePop();
	}
}

void Player::ChangeState(PlayerStateName name)
{
	for (auto p : m_States)
	{
		if (name == p->GetStateName())
		{
			m_State = p;
		}
	}
}

void Player::PlusShootDelayCount() 
{
	m_State->GetPlayerShoot()->PlusDelayCount();
}