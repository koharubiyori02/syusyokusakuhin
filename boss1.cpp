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
	
	//モデルとアニメーションをロードする
	AddComponent<AnimationModel>()->Load("asset\\model\\Boss1.fbx");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Idle.fbx", "Idle");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_JumpAttack.fbx", "JumpingAttack");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Punch.fbx", "Punch");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_EnemySpawn.fbx", "EnemySpawn");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_SlapAttack.fbx", "SlapAttack");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_HalfHP.fbx", "HalfHP");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Death.fbx", "Death");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Boss1_Walk.fbx", "Walk");

	//当たり判定のボックスを持たせる
	AddComponent<HitBox>();
	AddComponent<HitCircle>();

	//初期座標、向き、サイズをセットする
	m_Position = DEFAULT_POSITION;
	m_Rotation.y = XM_PI;
	m_Scale = DEFAULT_SCALE;

	m_Hp = BOSS_MAXHP;
	//m_Hp = 1;
	m_AnimationName1 = "Idle";
	m_AnimationName2 = "Idle";

	//単発
	shootType = new SingleShoot();

	Scene* scene = Manager::GetScene();



	///////////////////////////////////////////////////////////////////////////////////////////////////
	//ビヘイビアツリーを構築
	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_BehaviorTree = new BehaviorTree(this);

	//ノードの処理を登録
	RegisterNodes();

	SelectorNode* selector = new SelectorNode;

	DecoratorNode* decorator1 = new DecoratorNode(m_Register->GetValue("CheckDeath"));
	decorator1->SetNeedsConditionalAbort(true);//再評価対象
	DecoratorNode* decorator2 = new DecoratorNode(m_Register->GetValue("CheckHP"));
	decorator2->SetNeedsConditionalAbort(true);//再評価対象
	DecoratorNode* decorator3 = new DecoratorNode(m_Register->GetValue("CheckSetAttackNode"));
	decorator3->SetNeedsConditionalAbort(true);//再評価対象
	
	ActionNode* action1 = new ActionNode(m_Register->GetValue("Death"));
	ActionNode* action2 = new ActionNode(m_Register->GetValue("HalfHP"));
	ActionNode* action3 = new ActionNode(m_Register->GetValue("SetAttackNodeName"));
	ActionNode* action4 = new ActionNode(m_Register->GetValue("MoveToPlayer"));
	ActionNode* action5 = new ActionNode(m_Register->GetValue("Attack"));

	SequencerNode* sequencer = new SequencerNode;
	sequencer->SetNeedsConditionalAbort(true);//再評価対象

	decorator1->AddChild(action1);
	decorator2->AddChild(action2);
	decorator3->AddChild(action3);

	sequencer->AddChild(action4);
	sequencer->AddChild(action5);

	selector->AddChild(decorator1);
	selector->AddChild(decorator2);
	selector->AddChild(decorator3);
	selector->AddChild(sequencer);

	m_BehaviorTree->SetRootNode(selector);//ルートノードにセット

	m_BehaviorTree->Start();//ビヘイビアツリーの初期化

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
	//ビヘイビアツリーの更新
	m_BehaviorTree->Update();

	if (m_AnimationName1 != m_AnimationName2)
	{
		m_AnimationFrame = 0;
	}
	
	//アクティブなコンポーネントを更新
	for (auto p : m_Component)
	{
		if (p->IsActive())
		{
			p->Update();
		}
	}

	//アクティブがfalseになったら
	if (!m_Active)
	{
		SetRemove(true);
	}
}

void Boss1::Draw()
{
	GetComponent<AnimationModel>()->Update(m_AnimationName1.c_str(), m_AnimationFrame, m_AnimationName2.c_str(), m_AnimationFrame, m_AnimationBlendRatio);
	m_AnimationFrame++;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
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

//ノードの関数を登録
void Boss1::RegisterNodes()
{
	m_Register = NodeRegistry::GetInstance();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	//攻撃範囲外だったら移動
	m_Register->SetKey("MoveToPlayer", [this,scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);//プレイヤーと敵のベクトルの減算
		XMFLOAT3 currentVec = GetVec(subtract);					 //XMVECTORをXMFLOAT3に変換
		float dist = XMVectorGetX(XMVector3Length(subtract));    //プレイヤーと敵の距離

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "Walk";

		m_Position.x += currentVec.x * m_Speed;
		m_Position.y += currentVec.y * m_Speed;
		m_Position.z += currentVec.z * m_Speed;
		m_Rotation.y = atan2(currentVec.x, currentVec.z);		  //向きをプレイヤーに向ける

		if (dist <= m_AttackRange)//攻撃範囲内に入ったら
		{
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//ジャンピングアタック
	m_Register->SetKey("JumpingAttack", [this, scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);	//プレイヤーと敵のベクトルの減算
		XMFLOAT3 currentVec = GetVec(subtract);						//XMVECTORをXMFLOAT3に変換
		float dist = XMVectorGetX(XMVector3Length(subtract));		//プレイヤーと敵の距離
		XMVECTOR startVec{};		//ジャンプする前のベクトル
		static XMVECTOR endVec{};	//着地するベクトル

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "JumpingAttack";

		static float t = 0.0f;			//線形補間用のカウント
		static int currentFrame = 0;	//ジャンプ中のフレームカウント

		HitCircle* circle = GetComponent<HitCircle>();//当たり判定

#ifdef _DEBUG
		ImGui::Text("t:%f", dist);
#endif

		int jumpFrame = 40;				//ジャンプする瞬間のフレーム
		int tyakuchiFrame = 160;		//攻撃の当たり判定を有効にするフレーム
		int finishAnimationFrame = 360;	//アニメーションが終わるフレーム

		if (m_Frame < jumpFrame)//ジャンプするまでプレイヤーの方向に向く
		{
			m_Rotation.y = atan2(currentVec.x, currentVec.z);
		}
		if (m_Frame == jumpFrame)//ジャンプする瞬間にstartVecとendVecをセットする
		{
			startVec = subtract;
			endVec = playerVec;

			int hitFrame = 3;//当たり判定の有効フレーム

			circle->SetActive(true);
			circle->SetDamageSet(ENEMY_ATK, hitFrame);

			XMFLOAT3 circlePos;
			XMStoreFloat3(&circlePos, endVec);
			circle->SetPosition(circlePos);
		}

		if (jumpFrame < m_Frame && m_Frame <= tyakuchiFrame)//飛んでいる間は移動、着地したら移動しない
		{
			currentFrame++;
			t = currentFrame / 2000.0f;
			XMVECTOR resultVec = XMVectorLerp(bossVec, endVec, t);//線形補間
			XMFLOAT3 resultPos{};//補間後の座標
			XMStoreFloat3(&resultPos, resultVec);
			m_Position = resultPos;
		}
		if (m_Frame == tyakuchiFrame)//当たり判定を有効にする
		{
			circle->SetCanDamage(true);
			Manager::GetAudio()->PlaySE(AudioName::SE_Tatakituke);
			Manager::GetAudio()->SetVolume(AudioName::SE_Tatakituke, 1.0f);
		}

		if (m_Frame >= finishAnimationFrame)//アニメーションが終わったら初期化する
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

	//パンチ
	m_Register->SetKey("Punch", [this,scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);//プレイヤーと敵のベクトルの減算
		XMFLOAT3 currentVec = GetVec(subtract);//XMVECTORをXMFLOAT3に変換
		float dist = XMVectorGetX(XMVector3Length(subtract));//プレイヤーと敵の距離

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "Punch";
		
		int rotationFrame = 10;		  //プレイヤーの方を向くフレーム
		int activeCollisionFrame = 12;//攻撃の当たり判定を有効にするフレーム
		int finishAnimationFrame = 51;//アニメーションが終わるフレーム

		HitBox* box = GetComponent<HitBox>();

		if (m_Frame < rotationFrame)//振りかぶる時はプレイヤーの方を向く
		{
			m_Rotation.y = atan2(currentVec.x, currentVec.z);
		}
		if (m_Frame == activeCollisionFrame)//当たり判定を有効にする
		{
			int hitFrame = 3;//当たり判定の有効フレーム

			box->SetActive(true);
			box->SetDamageSet(ENEMY_ATK, hitFrame);
			box->SetCanDamage(true);
		}

		if (m_Frame >= finishAnimationFrame)//アニメーションが終わったら初期化する
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

	//雑魚敵召喚
	m_Register->SetKey("EnemySpawn", [this, scene]()->NodeStatus {
		Player* player = scene->GetGameObject<Player>();
		XMFLOAT3 playerPos = player->GetPosition();
		XMVECTOR playerVec = XMLoadFloat3(&playerPos);
		XMVECTOR bossVec = XMLoadFloat3(&m_Position);
		XMVECTOR subtract = XMVectorSubtract(playerVec, bossVec);//プレイヤーと敵のベクトルの減算
		XMFLOAT3 currentVec = GetVec(subtract);//XMVECTORをXMFLOAT3に変換
		float dist = XMVectorGetX(XMVector3Length(subtract));//プレイヤーと敵の距離

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "EnemySpawn";

		int spawnEnemyFrame = 75;	  //敵をスポーンするフレーム
		int finishAnimationFrame = 80;//アニメーションが終わるフレーム

		if (m_Frame == spawnEnemyFrame)//雑魚敵を生み出すタイミング
		{
			Scene* scene = Manager::GetScene();

			XMVECTOR bossVec = XMLoadFloat3(&m_Position);
			XMFLOAT3 forward = GetForward();//前方向ベクトル
			XMFLOAT3 left = GetRight();//左方向ベクトル
			XMFLOAT3 right = XMFLOAT3(left.x * -1.0f, left.y * -1.0f, left.z * -1.0f);//右方向ベクトル

			//XMVECTORに変更
			XMVECTOR forwardVec = XMLoadFloat3(&forward);
			XMVECTOR leftVec = XMLoadFloat3(&left);
			XMVECTOR rightVec = XMLoadFloat3(&right);

			XMFLOAT3 resultPos;

			float posOffset = 2.0f;//召喚時のポジションのオフセット

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


		if (m_Frame >= finishAnimationFrame)//アニメーションが終わったら初期化する
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

	//叩きつけ
	m_Register->SetKey("SlapAttack", [this, scene,camera]()->NodeStatus {

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "SlapAttack";

		Scene* scene = Manager::GetScene();

		XMFLOAT3 forward = GetForward();
		XMFLOAT3 right = GetRight();

		XMVECTOR forwardVec = XMVector3Normalize(XMLoadFloat3(&forward));	//前方向ベクトル
		XMVECTOR backVec = forwardVec * -1.0f;								//後ろ方向ベクトル
		XMVECTOR leftVec = XMVector3Normalize(XMLoadFloat3(&right));		//左方向ベクトル
		XMVECTOR rightVec = leftVec * -1.0f;								//右方向ベクトル
		XMVECTOR rightForward = XMVectorAdd(forwardVec, rightVec);			//右奥ベクトル
		XMVECTOR leftForward = XMVectorAdd(forwardVec, leftVec);			//左奥ベクトル
		XMVECTOR rightBack = XMVectorAdd(backVec, rightVec);				//右後ろベクトル
		XMVECTOR leftBack = XMVectorAdd(backVec, leftVec);					//左後ろベクトル
		XMFLOAT3 result;

		XMVECTOR vec[8] = { forwardVec ,backVec ,leftVec ,rightVec ,rightForward ,leftForward ,rightBack ,leftBack };

		HitCircle* circle = GetComponent<HitCircle>();//当たり判定

		int slapFrame = 80;			   //腕を叩きつけるフレーム
		int finishAnimationFrame = 160;//アニメーションが終わるフレーム

		if (m_Frame == 1)//最初に当たり判定の設定をする
		{
			int hitFrame = 3;//当たり判定の有効フレーム

			circle->SetActive(true);
			circle->SetDamageSet(ENEMY_ATK, hitFrame);
			circle->SetPosition(m_Position);
		}
		if (m_Frame == slapFrame)//叩きつけ時
		{
			Manager::GetAudio()->PlaySE(AudioName::SE_Tatakituke);

			circle->SetCanDamage(true);//当たり判定を有効にする

			camera->ShakeCamera();//カメラを揺らす

			//8方向に弾を発射する
			for (int i = 0; i < 8; i++)
			{
				XMStoreFloat3(&result, vec[i]);

				BulletEnemy * bullet = BulletManager::AccessBulletEnemy();;
				bullet->SetPosition(XMFLOAT3(m_Position));
				bullet->SetVelocity(result);
				scene->AddNoInit(bullet);
			}
		}

		if (m_Frame >= finishAnimationFrame)//アニメーションが終わったら
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

	//形態変化
	m_Register->SetKey("HalfHP", [this]()->NodeStatus {

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "HalfHP";

		//移動スピードを早くする
		m_Speed = 0.4f;
		m_Frame++;

		int finishAnimationFrame = 84;//アニメーションが終わるフレーム

		if (m_Frame == finishAnimationFrame)//アニメーションが終わたら
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

	//死
	m_Register->SetKey("Death", [this]()->NodeStatus {

		m_AnimationName1 = m_AnimationName2;
		m_AnimationName2 = "Death";

		m_Frame++;

		int finishAnimationFrame = 180;//アニメーションが終わるフレーム

		if (m_Frame >= finishAnimationFrame)//当たり判定を無効にする
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

	//攻撃をセット
	m_Register->SetKey("SetAttackNodeName", [this]()->NodeStatus {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, 3);

		m_AttackNodeName = m_AttackNodeNameArray[dist(gen)];//セットする
		m_AttackRange = m_AttackRangeMap.at(m_AttackNodeName);
		
		XMFLOAT3 scale = m_AttackScaleMap.at(m_AttackNodeName);//攻撃範囲

		//攻撃によってボックスかサークルのサイズを変更する
		if (m_AttackNodeName == "Punch")
		{
			GetComponent<HitBox>()->SetScale(scale);
		}
		else
		{
			GetComponent<HitCircle>()->SetScale(scale);
		}

		if (m_AttackNodeName != "")//セットされたら
		{
			return SUCCESS;
		}
		else
		{
			return RUNNING;
		}
		});

	//セットされた攻撃を実行
	m_Register->SetKey("Attack", [this]()->NodeStatus {

		std::function<NodeStatus()> m_Action = m_Register->GetValue(m_AttackNodeName);
		m_Anim = true;
		m_Frame++;//アニメーションのフレームを足す
		NodeStatus status = m_Action();//セットされた攻撃ノードを実行する

		if (!m_Anim)
		{
			m_AttackNodeName = "";
		}

		return status;
		});

	//HPが0かどうか
	m_Register->SetKey("CheckDeath", [this]()->NodeStatus {

		if (m_Hp <= 0)//HPが0なら
		{
			Scene* scene = Manager::GetScene();

			GetComponent<HitCircle>()->SetActive(false);
			GetComponent<HitBox>()->SetActive(false);

			//全てのエネミーを消す
			auto enemy = scene->GetGameObjects<Enemy>();
			for (auto p : enemy)
			{
				p->SetDestroy();
			}

			//全ての敵の弾を消す
			auto bullet = scene->GetGameObjects<BulletEnemy>();
			for (auto p : bullet)
			{
				p->SetActive(false);
			}

			m_IsDeath = true;//死んだフラグをオンにする

			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
		});

	//HPが半分切ったかどうか
	m_Register->SetKey("CheckHP", [this]()->NodeStatus {
		if (m_Anim)//アニメーションをしている最中なら
		{
			return RUNNING;
		}

		if (m_Hp <= BOSS_MAXHP / 2 && !m_IsHalfHP)//HPが半分以下なら
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
		});

	//攻撃ノードがセットされているかどうか
	m_Register->SetKey("CheckSetAttackNode", [this]()->NodeStatus {
		if (m_Anim)//アニメーションをしている最中なら
		{
			return RUNNING;
		}

		if (m_AttackNodeName == "")//セットされていなかったら
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
		});
}

//ボスのフェーズになるときに呼び出す
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

	//ボスゲージを出す
	scene->AddGameObject<BossHpGage>(LayerType::Type2D, this);

	m_IsDeath = false;
}
