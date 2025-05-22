#include "../main.h"
#include "../manager.h"
#include "../renderer.h"
#include "../player.h"
#include "particleEmitterBullet.h"

void  ParticleEmitterBullet::Init()
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

	m_Particle[0][0].Position.x = m_GameObject->GetPosition().x + sinf(0);
	m_Particle[0][0].Position.y = m_GameObject->GetPosition().y + cosf(0);
	m_Particle[0][0].Position.z = m_GameObject->GetPosition().z + cosf(0);

	m_Particle[1][0].Position.x = m_GameObject->GetPosition().x + sinf(3.0f);
	m_Particle[1][0].Position.y = m_GameObject->GetPosition().y + cosf(3.0f);
	m_Particle[1][0].Position.z = m_GameObject->GetPosition().z + cosf(3.0f);

	m_Particle[2][0].Position.x = m_GameObject->GetPosition().x + sinf(6.0f);
	m_Particle[2][0].Position.y = m_GameObject->GetPosition().y + cosf(6.0f);
	m_Particle[2][0].Position.z = m_GameObject->GetPosition().z + cosf(6.0f);
}

void ParticleEmitterBullet::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ParticleEmitterBullet::Update()
{
	Scene* scene = Manager::GetScene();
	
	//�p�[�e�B�N������
	int count = 1;//����p�[�e�B�N���𐶐����邩
	float offset = 0.0f;

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < PARTICLE_MAX; i++)
		{
			if (m_Particle[j][i].Enable == false)
			{
				m_Particle[j][i].Enable = true;
				m_Particle[j][i].Life = 20;
				m_Particle[j][i].Position.x = m_GameObject->GetPosition().x + sinf(m_Count + offset);
				m_Particle[j][i].Position.y = m_GameObject->GetPosition().y + cosf(m_Count + offset);
				m_Particle[j][i].Position.z = m_GameObject->GetPosition().z + cosf(m_Count + offset);
				m_Particle[j][i].Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

				count--;
				if (count <= 0)
					break;
			}
			
		}
		offset += 2.0f;
	}

	m_Count += 0.3f;
	//�p�[�e�B�N���ړ�
	//���C�t
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < PARTICLE_MAX; i++)
		{
			m_Particle[j][i].Scale.x -= 0.01f;
			m_Particle[j][i].Scale.y -= 0.01f;
			m_Particle[j][i].Scale.z -= 0.01f;

			if (m_Particle[j][i].Scale.x <= 0)
				m_Particle[j][i].Enable = false;
		}
	}

}

void ParticleEmitterBullet::Draw()
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
	material.Diffuse = XMFLOAT4(0.3f, 0.3f, 0.8f, 0.9f);
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

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < PARTICLE_MAX; i++)
		{

			if (m_Particle[j][i].Enable == true)
			{
				//���[���h�}�g���N�X�ݒ�
				XMMATRIX world, scale, trans;
				scale = XMMatrixScaling(m_Particle[j][i].Scale.x, m_Particle[j][i].Scale.y, m_Particle[j][i].Scale.z);
				trans = XMMatrixTranslation(m_Particle[j][i].Position.x, m_Particle[j][i].Position.y, m_Particle[j][i].Position.z);
				world = scale * invView * trans;
				Renderer::SetWorldMatrix(world);

				//�|���S���`��
				Renderer::GetDeviceContext()->Draw(4, 0);
			}

		}
	}

	Renderer::SetDepthEnable(true);
	Renderer::SetBlendAddEnable(false);
}

void ParticleEmitterBullet::ResetParticle()
{
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < PARTICLE_MAX; i++)
		{
			m_Particle[j][i].Scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_Particle[j][i].Enable = false;
		}
	}
}
