#include "manager.h"
#include "renderer.h"
#include "animationModel.h"
#include "Boss1.h"
#include "BehaviorTree//BehaviorTree.h"
#include "BehaviorTree//SelectorNode.h"
#include "BehaviorTree//DecoratorNode.h"
#include "BehaviorTree//ActionNode.h"
#include "BehaviorTree//sequencerNode.h"
#include "BossHpGage.h"
#include "HitBox.h"
#include "HitCircle.h"
#include "ShootType//singleShoot.h"
#include "enemy.h"
#include "bulletEnemy.h"
#include "bullet.h"
#include "bulletManager.h"
#include "ImGUI/imgui.h"

#include <random>

void Boss1::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
	
	//���f���ƃA�j���[�V���������[�h����
	AddComponent<AnimationModel>()->Load("asset\\model\\Boss1.fbx");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Idle.fbx", "Idle");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_JumpAttack.fbx", "JumpingAttack");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Punch.fbx", "Punch");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_EnemySpawn.fbx", "EnemySpawn");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_SlapAttack.fbx", "SlapAttack");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_HalfHP.fbx", "HalfHP");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Death.fbx", "Death");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Walk.fbx", "Walk");

	//�����蔻��̃{�b�N�X����������
	AddComponent<HitBox>();
	AddComponent<HitCircle>();

	//�������W�A�����A�T�C�Y���Z�b�g����
	m_Position = DEFAULT_POSITION;
	m_Rotation.y = XM_PI;
	m_Scale = DEFAULT_SCALE;

	m_Hp = BOSS_MAXHP;
	//m_Hp = 1;
	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";

	//�P��
	shootType = new SingleShoot();

	Scene* scene = Manager::GetScene();



	///////////////////////////////////////////////////////////////////////////////////////////////////
	//�r�w�C�r�A�c���[���\�z
	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_BehaviorTree = new BehaviorTree(this);

	//�m�[�h�̏�����o�^
	RegisterNodes();

	SelectorNode* selector = new SelectorNode;

	DecoratorNode* decorator1 = new DecoratorNode(m_Register->GetValue("CheckDeath"));
	decorator1->SetNeedsConditionalAbort(true);//�ĕ]���Ώ�
	DecoratorNode* decorator2 = new DecoratorNode(m_Register->GetValue("CheckHP"));
	decorator2->SetNeedsConditionalAbort(true);//�ĕ]���Ώ�
	DecoratorNode* decorator3 = new DecoratorNode(m_Register->GetValue("CheckSetAttackNode"));
	decorator3->SetNeedsConditionalAbort(true);//�ĕ]���Ώ�
	
	ActionNode* action1 = new ActionNode(m_Register->GetValue("Death"));
	ActionNode* action2 = new ActionNode(m_Register->GetValue("HalfHP"));
	ActionNode* action3 = new ActionNode(m_Register->GetValue("SetAttackNodeName"));
	ActionNode* action4 = new ActionNode(m_Register->GetValue("MoveToPlayer"));
	ActionNode* action5 = new ActionNode(m_Register->GetValue("Attack"));

	SequencerNode* sequencer = new SequencerNode;
	sequencer->SetNeedsConditionalAbort(true);//�ĕ]���Ώ�

	decorator1->AddChild(action1);
	decorator2->AddChild(action2);
	decorator3->AddChild(action3);

	sequencer->AddChild(action4);
	sequencer->AddChild(action5);

	selector->AddChild(decorator1);
	selector->AddChild(decorator2);
	selector->AddChild(decorator3);
	selector->AddChild(sequencer);

	m_BehaviorTree->SetRootNode(selector);//���[�g�m�[�h�ɃZ�b�g

	m_BehaviorTree->Start();//�r�w�C�r�A�c���[�̏�����

	///////////////////////////////////////////////////////////////////////////////////////////////////
}

void Boss1::Uninit()
{
	delete shootType;
	delete m_BehaviorTree;

	for (auto p : m_Component)
	{
		p->Uninit();		
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Boss1::Update()
{
	//�r�w�C�r�A�c���[�̍X�V
	m_BehaviorTree->Update();

	if (m_AnimationName1 != m_AnimationName2)
	{
		m_AnimationFrame = 0;
	}
	
	//�A�N�e�B�u�ȃR���|�[�l���g���X�V
	for (auto p : m_Component)
	{
		if (p->IsActive())
		{
			p->Update();
		}
	}

	//�A�N�e�B�u��false�ɂȂ�����
	if (!m_Active)
	{
		SetRemove(true);
	}
}

void Boss1::Draw()
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
	rot = XMMatrixRotationRollPitchYaw(0.0f, m_Rotation.y, 0.0f);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	for (auto p : m_Component)
	{
		if (p->IsActive())
		{
			p->Draw();
		}
	}
}

void Boss1::ImGuiDebug()
{
	ImGui::Text("frame:%d", m_Frame);
}

//�m�[�h�̊֐���o�^
void Boss1::RegisterNodes()
{
	m_Register = NodeRegistry::GetInstance();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	//�U���͈͊O��������ړ�
	m_Register->SetKey("MoveToPlayer", [this,scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);//�v���C���[�ƓG�̃x�N�g���̌��Z
		XMFLOAT3 currentVec = GetVec(subtract);					 //XMVECTOR��XMFLOAT3�ɕϊ�
		float dist = XMVectorGetX(XMVector3Length(subtract));    //�v���C���[�ƓG�̋���

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "Walk";

		m_Position.x += currentVec.x * m_Speed;
		m_Position.y += currentVec.y * m_Speed;
		m_Position.z += currentVec.z * m_Speed;
		m_Rotation.y = atan2(currentVec.x, currentVec.z);		  //�������v���C���[�Ɍ�����

		if (dist <= m_AttackRange)//�U���͈͓��ɓ�������
		{
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�W�����s���O�A�^�b�N
	m_Register->SetKey("JumpingAttack", [this, scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);	//�v���C���[�ƓG�̃x�N�g���̌��Z
		XMFLOAT3 currentVec = GetVec(subtract);						//XMVECTOR��XMFLOAT3�ɕϊ�
		float dist = XMVectorGetX(XMVector3Length(subtract));		//�v���C���[�ƓG�̋���
		XMVECTOR startVec{};		//�W�����v����O�̃x�N�g��
		static XMVECTOR endVec{};	//���n����x�N�g��

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "JumpingAttack";

		static float t = 0.0f;			//���`��ԗp�̃J�E���g
		static int currentFrame = 0;	//�W�����v���̃t���[���J�E���g

		HitCircle* circle = GetComponent<HitCircle>();//�����蔻��

#ifdef _DEBUG
		ImGui::Text("t:%f", dist);
#endif

		int jumpFrame = 40;				//�W�����v����u�Ԃ̃t���[��
		int tyakuchiFrame = 160;		//�U���̓����蔻���L���ɂ���t���[��
		int finishAnimationFrame = 360;	//�A�j���[�V�������I���t���[��

		if (m_Frame < jumpFrame)//�W�����v����܂Ńv���C���[�̕����Ɍ���
		{
			m_Rotation.y = atan2(currentVec.x, currentVec.z);
		}
		if (m_Frame == jumpFrame)//�W�����v����u�Ԃ�startVec��endVec���Z�b�g����
		{
			startVec = subtract;
			endVec = playerVec;

			int hitFrame = 3;//�����蔻��̗L���t���[��

			circle->SetActive(true);
			circle->SetDamageSet(ENEMY_ATK, hitFrame);

			XMFLOAT3 circlePos;
			XMStoreFloat3(&circlePos, endVec);
			circle->SetPosition(circlePos);
		}

		if (jumpFrame < m_Frame && m_Frame <= tyakuchiFrame)//���ł���Ԃ͈ړ��A���n������ړ����Ȃ�
		{
			currentFrame++;
			t = currentFrame / 2000.0f;
			XMVECTOR resultVec = XMVectorLerp(bossVec, endVec, t);//���`���
			XMFLOAT3 resultPos{};//��Ԍ�̍��W
			XMStoreFloat3(&resultPos, resultVec);
			m_Position = resultPos;
		}
		if (m_Frame == tyakuchiFrame)//�����蔻���L���ɂ���
		{
			circle->SetCanDamage(true);
			Manager::GetAudio()->PlaySE(AudioName::SE_Tatakituke);
			Manager::GetAudio()->SetVolume(AudioName::SE_Tatakituke, 1.0f);
		}

		if (m_Frame >= finishAnimationFrame)//�A�j���[�V�������I������珉��������
		{
			m_Anim = false;
			m_Frame = 0;
			t = 0.0f;
			currentFrame = 0;
			endVec = {};
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�p���`
	m_Register->SetKey("Punch", [this,scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);//�v���C���[�ƓG�̃x�N�g���̌��Z
		XMFLOAT3 currentVec = GetVec(subtract);//XMVECTOR��XMFLOAT3�ɕϊ�
		float dist = XMVectorGetX(XMVector3Length(subtract));//�v���C���[�ƓG�̋���

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "Punch";
		
		int rotationFrame = 10;		  //�v���C���[�̕��������t���[��
		int activeCollisionFrame = 12;//�U���̓����蔻���L���ɂ���t���[��
		int finishAnimationFrame = 51;//�A�j���[�V�������I���t���[��

		HitBox* box = GetComponent<HitBox>();

		if (m_Frame < rotationFrame)//�U�肩�Ԃ鎞�̓v���C���[�̕�������
		{
			m_Rotation.y = atan2(currentVec.x, currentVec.z);
		}
		if (m_Frame == activeCollisionFrame)//�����蔻���L���ɂ���
		{
			int hitFrame = 3;//�����蔻��̗L���t���[��

			box->SetActive(true);
			box->SetDamageSet(ENEMY_ATK, hitFrame);
			box->SetCanDamage(true);
		}

		if (m_Frame >= finishAnimationFrame)//�A�j���[�V�������I������珉��������
		{
			m_Anim = false;
			m_Frame = 0;

			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�G���G����
	m_Register->SetKey("EnemySpawn", [this, scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);//�v���C���[�ƓG�̃x�N�g���̌��Z
		XMFLOAT3 currentVec = GetVec(subtract);//XMVECTOR��XMFLOAT3�ɕϊ�
		float dist = XMVectorGetX(XMVector3Length(subtract));//�v���C���[�ƓG�̋���

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "EnemySpawn";

		int spawnEnemyFrame = 75;	  //�G���X�|�[������t���[��
		int finishAnimationFrame = 80;//�A�j���[�V�������I���t���[��

		if (m_Frame == spawnEnemyFrame)//�G���G�𐶂ݏo���^�C�~���O
		{
			Scene* scene = Manager::GetScene();

			XMVECTOR bossVec = XMLoadFloat3(&m_Position);
			XMFLOAT3 forward = GetForward();//�O�����x�N�g��
			XMFLOAT3 left = GetRight();//�������x�N�g��
			XMFLOAT3 right = XMFLOAT3(left.x * -1.0f, left.y * -1.0f, left.z * -1.0f);//�E�����x�N�g��

			//XMVECTOR�ɕύX
			XMVECTOR forwardVec = XMLoadFloat3(&forward);
			XMVECTOR leftVec = XMLoadFloat3(&left);
			XMVECTOR rightVec = XMLoadFloat3(&right);

			XMFLOAT3 resultPos;

			float posOffset = 2.0f;//�������̃|�W�V�����̃I�t�Z�b�g

			forwardVec += bossVec + forwardVec * posOffset;
			XMStoreFloat3(&resultPos, forwardVec);
			scene->AddGameObject<Enemy>(LayerType::Type3D, new SingleShoot(), ENEMY_MAXHP, ENEMY_ATK)->SetPosition(resultPos);

			leftVec += bossVec + leftVec * posOffset;
			XMStoreFloat3(&resultPos, leftVec);
			scene->AddGameObject<Enemy>(LayerType::Type3D, new SingleShoot(), ENEMY_MAXHP, ENEMY_ATK)->SetPosition(resultPos);

			rightVec += bossVec + rightVec * posOffset;
			XMStoreFloat3(&resultPos, rightVec);
			scene->AddGameObject<Enemy>(LayerType::Type3D, new SingleShoot(), ENEMY_MAXHP, ENEMY_ATK)->SetPosition(resultPos);
		}


		if (m_Frame >= finishAnimationFrame)//�A�j���[�V�������I������珉��������
		{
			m_Anim = false;
			m_Frame = 0;

			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�@����
	m_Register->SetKey("SlapAttack", [this, scene,camera]()->NodeStatus {

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "SlapAttack";

		Scene* scene = Manager::GetScene();

		XMFLOAT3 forward = GetForward();
		XMFLOAT3 right = GetRight();

		XMVECTOR forwardVec = XMVector3Normalize(XMLoadFloat3(&forward));	//�O�����x�N�g��
		XMVECTOR backVec = forwardVec * -1.0f;								//�������x�N�g��
		XMVECTOR leftVec = XMVector3Normalize(XMLoadFloat3(&right));		//�������x�N�g��
		XMVECTOR rightVec = leftVec * -1.0f;								//�E�����x�N�g��
		XMVECTOR rightForward = XMVectorAdd(forwardVec, rightVec);			//�E���x�N�g��
		XMVECTOR leftForward = XMVectorAdd(forwardVec, leftVec);			//�����x�N�g��
		XMVECTOR rightBack = XMVectorAdd(backVec, rightVec);				//�E���x�N�g��
		XMVECTOR leftBack = XMVectorAdd(backVec, leftVec);					//�����x�N�g��
		XMFLOAT3 result;

		XMVECTOR vec[8] = { forwardVec ,backVec ,leftVec ,rightVec ,rightForward ,leftForward ,rightBack ,leftBack };

		HitCircle* circle = GetComponent<HitCircle>();//�����蔻��

		int slapFrame = 80;			   //�r��@������t���[��
		int finishAnimationFrame = 160;//�A�j���[�V�������I���t���[��

		if (m_Frame == 1)//�ŏ��ɓ����蔻��̐ݒ������
		{
			int hitFrame = 3;//�����蔻��̗L���t���[��

			circle->SetActive(true);
			circle->SetDamageSet(ENEMY_ATK, hitFrame);
			circle->SetPosition(m_Position);
		}
		if (m_Frame == slapFrame)//�@������
		{
			Manager::GetAudio()->PlaySE(AudioName::SE_Tatakituke);

			circle->SetCanDamage(true);//�����蔻���L���ɂ���

			camera->ShakeCamera();//�J������h�炷

			//8�����ɒe�𔭎˂���
			for (int i = 0; i < 8; i++)
			{
				XMStoreFloat3(&result, vec[i]);

				BulletEnemy * bullet = BulletManager::AccessBulletEnemy();;
				bullet->SetPosition(XMFLOAT3(m_Position));
				bullet->SetVelocity(result);
				scene->AddNoInit(bullet);
			}
		}

		if (m_Frame >= finishAnimationFrame)//�A�j���[�V�������I�������
		{
			m_Anim = false;
			m_Frame = 0;
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�`�ԕω�
	m_Register->SetKey("HalfHP", [this]()->NodeStatus {

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "HalfHP";

		//�ړ��X�s�[�h�𑁂�����
		m_Speed = 0.4f;
		m_Frame++;

		int finishAnimationFrame = 84;//�A�j���[�V�������I���t���[��

		if (m_Frame == finishAnimationFrame)//�A�j���[�V�������I�킽��
		{
			Manager::GetAudio()->PlaySE(AudioName::SE_Roar);
			m_Anim = false;
			m_Frame = 0;
			m_IsHalfHP = true;
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//��
	m_Register->SetKey("Death", [this]()->NodeStatus {

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "Death";

		m_Frame++;

		int finishAnimationFrame = 180;//�A�j���[�V�������I���t���[��

		if (m_Frame >= finishAnimationFrame)//�����蔻��𖳌��ɂ���
		{
			m_Anim = false;
			m_Frame = 0;
			m_Active = false;

			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�U�����Z�b�g
	m_Register->SetKey("SetAttackNodeName", [this]()->NodeStatus {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, 3);

		m_AttackNodeName = m_AttackNodeNameArray[dist(gen)];//�Z�b�g����
		m_AttackRange = m_AttackRangeMap.at(m_AttackNodeName);
		
		XMFLOAT3 scale = m_AttackScaleMap.at(m_AttackNodeName);//�U���͈�

		//�U���ɂ���ă{�b�N�X���T�[�N���̃T�C�Y��ύX����
		if (m_AttackNodeName == "Punch")
		{
			GetComponent<HitBox>()->SetScale(scale);
		}
		else
		{
			GetComponent<HitCircle>()->SetScale(scale);
		}

		if (m_AttackNodeName != "")//�Z�b�g���ꂽ��
		{
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//�Z�b�g���ꂽ�U�������s
	m_Register->SetKey("Attack", [this]()->NodeStatus {

		std::function<NodeStatus()> m_Action = m_Register->GetValue(m_AttackNodeName);
		m_Anim = true;
		m_Frame++;//�A�j���[�V�����̃t���[���𑫂�
		NodeStatus status = m_Action();//�Z�b�g���ꂽ�U���m�[�h�����s����

		if (!m_Anim)
		{
			m_AttackNodeName = "";
		}

		return status;
		});

	//HP��0���ǂ���
	m_Register->SetKey("CheckDeath", [this]()->NodeStatus {

		if (m_Hp <= 0)//HP��0�Ȃ�
		{
			Scene* scene = Manager::GetScene();

			GetComponent<HitCircle>()->SetActive(false);
			GetComponent<HitBox>()->SetActive(false);

			//�S�ẴG�l�~�[������
			auto enemy = scene->GetGameObjects<Enemy>();
			for (auto p : enemy)
			{
				p->SetDestroy();
			}

			//�S�Ă̓G�̒e������
			auto bullet = scene->GetGameObjects<BulletEnemy>();
			for (auto p : bullet)
			{
				p->SetActive(false);
			}

			m_IsDeath = true;//���񂾃t���O���I���ɂ���

			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
		});

	//HP�������؂������ǂ���
	m_Register->SetKey("CheckHP", [this]()->NodeStatus {
		if (m_Anim)//�A�j���[�V���������Ă���Œ��Ȃ�
		{
			return RUNNING;
		}

		if (m_Hp <= BOSS_MAXHP / 2 && !m_IsHalfHP)//HP�������ȉ��Ȃ�
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
		});

	//�U���m�[�h���Z�b�g����Ă��邩�ǂ���
	m_Register->SetKey("CheckSetAttackNode", [this]()->NodeStatus {
		if (m_Anim)//�A�j���[�V���������Ă���Œ��Ȃ�
		{
			return RUNNING;
		}

		if (m_AttackNodeName == "")//�Z�b�g����Ă��Ȃ�������
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
		});
}

//�{�X�̃t�F�[�Y�ɂȂ�Ƃ��ɌĂяo��
void Boss1::ResetBoss()
{
	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";

	m_AnimationBlendRatio = 0.0f;
	m_AnimationFrame = 0;

	//m_Hp = 1;
	m_Hp = BOSS_MAXHP;

	m_Position = DEFAULT_POSITION;
	m_Rotation.y = XM_PI;
	m_Scale = DEFAULT_SCALE;

	m_AttackNodeName = "";
	m_Speed = DEFAULT_SPEED;

	Scene* scene = Manager::GetScene();

	//�{�X�Q�[�W���o��
	scene->AddGameObject<BossHpGage>(LayerType::Type2D, this);

	m_IsDeath = false;
}
