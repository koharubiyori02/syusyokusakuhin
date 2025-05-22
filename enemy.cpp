#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "enemy.h"
#include "EnemyState//enemyIdle.h"
#include "manager.h"
#include "EnemyHpGage.h"

void Enemy::Init()
{
	AddComponent<ModelRenderer>();
	GetComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");

	AddComponent<EnemyHpGage>();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
	
	m_Scale = DEFAULT_SCALE;

	//�X�e�[�g�̏����l���Z�b�g
	SetEnemyState<EnemyIdle>();
}

void Enemy::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	for (auto p : m_Component)
	{
		p->Uninit();
	}
}

void Enemy::Update()
{
	//�`���[�g���A���̏ꍇ�͓����Ȃ�
	if (!m_Tutorial)
	{
		m_State->Update();
	}

	for (auto p : m_Component)
	{
		p->Update();
	}
}

void Enemy::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y+XM_PI, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	for (auto p : m_Component)
	{
		p->Draw();
	}

}

void Enemy::ImGuiDebug()
{
	ImGui::Text("x:%f y:%f z:%f", m_Position.x, m_Position.y, m_Position.z);
}

//�e�𔭎�
void Enemy::Shoot()
{
	shootType->Shoot(XMFLOAT3(m_Position.x, m_Position.y + 0.5f, m_Position.z), GetForward());
}
