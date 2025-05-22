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

	m_Active = false;//�ŏ��͔�A�N�e�B�u
}

void HitBox::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HitBox::Update()
{
	if (m_CanDamage)//�_���[�W��^�������ԂȂ�^����
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

	//������gameObject�Ɠ����ɂ��āA���W�������O�ɂ���
	m_Rotation = rot;
	newPos += vec * POSITOIN_OFFSET;
	XMStoreFloat3(&m_Position, newPos);

	if (Collision::GetInstance().OBBCollision(m_Position, player->GetPosition(), player->GetForward(), player->GetUp(), player->GetRight(), m_Scale))
	{
		//���i�q�b�g��h�����߂Ɉ�x���������瓖����Ȃ��悤�ɂ���
		if (!m_IsHit)
		{
			Manager::GetAudio()->PlaySE(AudioName::SE_Punch);
			scene->GetGameObject<PostProcess>()->SetActive(true);
			scene->GetGameObject<Camera>()->SetShakeActive(true);

			player->AddDamage(1);
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
