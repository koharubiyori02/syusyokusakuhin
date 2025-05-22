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

	//弾の向いている方向に移動させる
	m_Position.x += m_Velocity.x * m_Speed;
	m_Position.y += m_Velocity.y * m_Speed;
	m_Position.z += m_Velocity.z * m_Speed;

	int atkUpValue = 10;

	m_Atk = BULLET_DEFAULT_ATK + (m_AtkUpCount * atkUpValue);

	//敵との衝突判定
	auto enemyList = scene->GetGameObjects<Enemy>();

	XMFLOAT3 enemyHitScale = { 1.0f,2.0f,1.0f };

	for (auto p : enemyList)
	{
		if (Collision::GetInstance().OBBCollision(
			m_Position, p->GetPosition(), p->GetForward(), p->GetUp(), p->GetRight(), enemyHitScale))
		{
			if (m_Penetrate)
			{
				//貫通リストにいた場合はスルーする
				if (std::find(m_PenetrateList.begin(), m_PenetrateList.end(), p) != m_PenetrateList.end())
					continue;
			}
			else
			{
				m_Active = false;
			}

			Manager::GetAudio()->PlaySE(AudioName::SE_BulletHit);

			//エフェクトを出す
			Explosion* explosion = EffectManager::AccessExplosion();
			explosion->SetPosition(m_Position);
			scene->AddNoInit(explosion);

			p->AddDamage(m_Atk);
			//m_Active = false;

			if (p->GetHp() <= 0)//敵を倒したらキルカウントを足して敵を消す
			{
				scene->GetGameObject<Player>()->KillCountPlus();
				p->SetDestroy();
			}		
			else
			{
				m_PenetrateList.push_back(p);//貫通リストに追加する
			}
		}
	}

	//ボスとの当たり判定
	Boss1* boss = scene->GetGameObject<Boss1>();

	XMFLOAT3 bossHitScale = { 3.0f, 4.0f, 3.0f };

	if (boss && !boss->GetIsDeath())//ボスが存在するかつ、死んでいない場合
	{
		if (Collision::GetInstance().OBBCollision(
			m_Position, boss->GetPosition(), boss->GetForward(), boss->GetUp(), boss->GetRight(), bossHitScale))
		{
			if (m_Penetrate)
			{
				//貫通リストにいない場合
				if (std::find(m_PenetrateList.begin(), m_PenetrateList.end(), boss) == m_PenetrateList.end())
				{
					//エフェクトを出す
					Explosion* explosion = EffectManager::AccessExplosion();
					explosion->SetPosition(m_Position);
					scene->AddNoInit(explosion);

					boss->AddDamage(m_Atk);

					m_PenetrateList.push_back(boss);//貫通リストに追加する
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
		//障害物と当たっていたら弾を消す
		if (Collision::GetInstance().OBBCollision(
			m_Position, p->GetPosition(), p->GetForward(), p->GetUp(), p->GetRight(), p->GetScale()))
		{
			//エフェクトを出す
			Explosion* explosion = EffectManager::AccessExplosion();
			explosion->SetPosition(m_Position);
			scene->AddNoInit(explosion);
			
			m_Active = false;
		}
	}

	//場外に出たら消す
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

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
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

	//生成されたときに攻撃力UPアイテムの取得数に応じて攻撃力をセット
	m_Atk = BULLET_DEFAULT_ATK + (m_AtkUpCount * 10);

	//生成されたときにスピードUPアイテムの取得数に応じてスピードをセット
	//m_Speed = BULLET_DEFAULT_SPEED + (m_SpeedUpCount * m_SpeedUpValue);


	m_Active = true;
}
