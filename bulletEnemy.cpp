#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "bulletEnemy.h"
#include "box.h"
#include "manager.h"
#include "effectManager.h"

void BulletEnemy::Init()
{
	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void BulletEnemy::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BulletEnemy::Update()
{
	for (auto p : m_Component)
	{
		p->Update();
	}
	
	m_Life++;

	Scene* scene = Manager::GetScene();

	//セットした弾の種類の更新（弾のポジションを変えるなど）
	m_Position.x += m_Velocity.x * m_Speed;
	m_Position.y += m_Velocity.y * m_Speed;
	m_Position.z += m_Velocity.z * m_Speed;

	//寿命が過ぎたら消す
	if (m_Life > MAX_LIFE)
	{
		m_Active = false;
		m_Life = 0;
		Explosion* explosion = EffectManager::AccessExplosion();
		explosion->SetPosition(m_Position);
		scene->AddNoInit(explosion);
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

			ResetBullet();
		}
	}

	if (!m_Active)
	{
		SetRemove(true);
	}
}

void BulletEnemy::Draw()
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