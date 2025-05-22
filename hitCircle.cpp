#include "HitCircle.h"
#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "manager.h"
#include "camera.h"
#include "postProcess.h"

void HitCircle::Init()
{
	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\cylinder.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\hitColorPS.cso");

	m_Active = false;//最初は非アクティブ
}

void HitCircle::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HitCircle::Update()
{
	if (m_CanDamage)//ダメージを与えられる状態なら与える
	{
		AddDamage();
	}
}

void HitCircle::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	if (camera->CheckView(m_Position, m_Scale) == false)
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

void HitCircle::AddDamage()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	m_ValidFrame--;

	if (Collision::GetInstance().OBBCollision(m_Position, player->GetPosition(), player->GetForward(), player->GetUp(), player->GetRight(), m_Scale))
	{
		//多段ヒットを防ぐために一度当たったら当たらないようにする
		if (!m_IsHit)
		{
			scene->GetGameObject<PostProcess>()->SetActive(true);
			scene->GetGameObject<Camera>()->SetShakeActive(true);

			player->AddDamage(m_Damage);
			m_IsHit = true;
		}
	}

	if (m_ValidFrame == 0)//有効フレームが終わったら初期化する
	{
		m_IsHit = false;
		m_CanDamage = false;
		m_Active = false;
		m_ValidFrame = 0;
	}
}
