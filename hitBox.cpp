#include "HitBox.h"
#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "manager.h"
#include "camera.h"
#include "postProcess.h"

void HitBox::Init()
{
	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\box.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Active = false;//最初は非アクティブ
}

void HitBox::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HitBox::Update()
{
	if (m_CanDamage)//ダメージを与えられる状態なら与える
	{
		AddDamage();
	}
}

void HitBox::Draw()
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

#ifdef _DEBUG
	for (auto p : m_Component)
	{
		p->Draw();
	}
#endif
}

void HitBox::AddDamage()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	m_ValidFrame--;

	XMFLOAT3 pos = m_GameObject->GetPosition();
	XMFLOAT3 rot = m_GameObject->GetRotation();
	XMFLOAT3 forward = m_GameObject->GetForward();

	XMVECTOR newPos = XMLoadFloat3(&pos);
	XMVECTOR vec = XMLoadFloat3(&forward);

	//向きをgameObjectと同じにして、座標を少し前にする
	m_Rotation = rot;
	newPos += vec * POSITOIN_OFFSET;
	XMStoreFloat3(&m_Position, newPos);

	if (Collision::GetInstance().OBBCollision(m_Position, player->GetPosition(), player->GetForward(), player->GetUp(), player->GetRight(), m_Scale))
	{
		//多段ヒットを防ぐために一度当たったら当たらないようにする
		if (!m_IsHit)
		{
			Manager::GetAudio()->PlaySE(AudioName::SE_Punch);
			scene->GetGameObject<PostProcess>()->SetActive(true);
			scene->GetGameObject<Camera>()->SetShakeActive(true);

			player->AddDamage(1);
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
