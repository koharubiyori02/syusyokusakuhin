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

	m_Active = false;//�ŏ��͔�A�N�e�B�u
}

void HitCircle::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HitCircle::Update()
{
	if (m_CanDamage)//�_���[�W��^�������ԂȂ�^����
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

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�	
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
		//���i�q�b�g��h�����߂Ɉ�x���������瓖����Ȃ��悤�ɂ���
		if (!m_IsHit)
		{
			scene->GetGameObject<PostProcess>()->SetActive(true);
			scene->GetGameObject<Camera>()->SetShakeActive(true);

			player->AddDamage(m_Damage);
			m_IsHit = true;
		}
	}

	if (m_ValidFrame == 0)//�L���t���[�����I������珉��������
	{
		m_IsHit = false;
		m_CanDamage = false;
		m_Active = false;
		m_ValidFrame = 0;
	}
}
