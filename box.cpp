#include "box.h"
#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "manager.h"
#include "camera.h"

void Box::Init()
{
	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\box.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\alphaDitherPS.cso");

	Renderer::CreatePixelShader(&m_PixelShader2,
		"shader\\hitCameraPS.cso");

	m_Scale = DEFAULT_SCALE;
}

void Box::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_PixelShader2->Release();
}

void Box::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	XMFLOAT3 cameraScale = XMFLOAT3(2.0f, 8.0f, 2.0f);//当たり判定用のカメラスケール

	if (Collision::GetInstance().AABBCollision(m_Position, m_Scale, camera->GetPosition(), cameraScale))
	{
		m_HitCamera = true;
	}
	else
	{
		m_HitCamera = false;
	}
}

void Box::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	if (camera->CheckView(m_Position, m_Scale) == false)
		return;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	if (m_HitCamera)
	{
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader2, NULL, 0);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	}

	//ワールドマトリクス設定	
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	if (m_HitCamera)
	{
		//Zバッファ無効
		Renderer::SetDepthEnable(false);

	}

	for (auto p : m_Component)
	{
		p->Draw();
	}

	if (m_HitCamera)
	{
		//Zバッファ有効
		Renderer::SetDepthEnable(true);
	}
}
