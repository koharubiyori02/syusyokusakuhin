#include "../manager.h"
#include "../renderer.h"
#include "../player.h"
#include "particleEmitterItem.h"

void ParticleEmitterItem::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\particle.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void ParticleEmitterItem::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ParticleEmitterItem::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//�p�[�e�B�N������
	int count = 10;

	float power = 500.0f;
	
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 30;
			m_Particle[i].Position = m_GameObject->GetPosition();
			m_Particle[i].Position.y = 0.0f;
			m_Particle[i].Scale = XMFLOAT3(0.3f, 0.3f, 0.3f);

			m_Particle[i].Velocity.x = (rand() % 85 - 35) / power;
			m_Particle[i].Velocity.y = (rand() % 700 + 50) / power;
			m_Particle[i].Velocity.z = (rand() % 85 - 35) / power;
			count--;
			if (count <= 0)
				break;

		}
	}
	m_InitCount = 0;
	

	//�p�[�e�B�N���ړ�
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//�ړ�
			m_Particle[i].Position.x += m_Particle[i].Velocity.x;
			m_Particle[i].Position.y += m_Particle[i].Velocity.y;
			m_Particle[i].Position.z += m_Particle[i].Velocity.z;

			//���C�t
			m_Particle[i].Life--;
			if (m_Particle[i].Life == 0)
			{
				m_Particle[i].Enable = false;
			}
		}
	}

	m_InitCount++;
}

void ParticleEmitterItem::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//�t�s��
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Z�o�b�t�@����
	Renderer::SetDepthEnable(false);

	//���Z�����L��
	Renderer::SetBlendAddEnable(true);

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//���[���h�}�g���N�X�ݒ�
			XMMATRIX world, scale, trans;
			scale = XMMatrixScaling(m_Particle[i].Scale.x, m_Particle[i].Scale.y, m_Particle[i].Scale.z);
			trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			//�|���S���`��
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	Renderer::SetDepthEnable(true);
	Renderer::SetBlendAddEnable(false);
}
