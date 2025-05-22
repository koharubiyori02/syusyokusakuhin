#include "../main.h"
#include "../renderer.h"
#include "../manager.h"
#include "../modelRenderer.h"
#include "../PlayerBullet/atkUpAbility.h"
#include "../Particle/particleEmitterItem.h"
#include "atkUpItem.h"

void AtkUpItem::Init()
{
	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\can2.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	AddComponent<ParticleEmitterItem>()->SetColor(XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f));

	m_Scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
}

void AtkUpItem::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	for (auto p : m_Component)
	{
		p->Uninit();
	}
}

void AtkUpItem::Update()
{
	GetComponent<ParticleEmitterItem>()->Update();
}

void AtkUpItem::Draw()
{
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