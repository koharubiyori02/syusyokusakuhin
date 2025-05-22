#include "../main.h"
#include "../manager.h"
#include "../renderer.h"
#include "../player.h"
#include "particleEmitterEnemySpawn.h"

void  ParticleEmitterEnemySpawn::Init()
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

void ParticleEmitterEnemySpawn::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ParticleEmitterEnemySpawn::Update()
{
	float offset = 0.0f;//�p�[�e�B�N���̉~����邽�߂̃I�t�Z�b�g
	float power = 500.0f;//�p�[�e�B�N���̍L����

	//�p�[�e�B�N���𐶐�
	for (int i = 0; i < 10; i++)
	{
		offset += 0.7f;
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 30.0f;
			m_Particle[i].Position.x = m_GameObject->GetPosition().x + sinf(offset + 0.1f);
			m_Particle[i].Position.y = -0.5f;
			m_Particle[i].Position.z = m_GameObject->GetPosition().z + cosf(offset+0.1f);

			m_Particle[i].Scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
		}
	}

	//�p�[�e�B�N���̈ړ�
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (i + 11 >= PARTICLE_MAX)
			break;

		offset += 0.3f;
		if (m_Particle[11 + i].Enable == false)
		{
			m_Particle[11 + i].Enable = true;
			m_Particle[11 + i].Life = 60.0f;
			m_Particle[11 + i].Position.x = m_GameObject->GetPosition().x + sinf(offset);
			m_Particle[11 + i].Position.y = m_GameObject->GetPosition().y - 1.0f;
			m_Particle[11 + i].Position.z = m_GameObject->GetPosition().z + cosf(offset);

			m_Particle[11 + i].Position.y -= m_GameObject->GetScale().y;

			m_Particle[11 + i].Velocity.x = (rand() % 15 - 5) / power;
			m_Particle[11 + i].Velocity.y = 50.0f / power;
			m_Particle[11 + i].Velocity.z = (rand() % 15 - 5) / power;

			m_Particle[11 + i].Scale = XMFLOAT3(0.3f, 0.5f, 0.3f);
		}
	}

	//�p�[�e�B�N���ړ�
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (i + 11 >= PARTICLE_MAX)
			break;

		if (m_Particle[11 + i].Enable == true)
		{
			m_Particle[11 + i].Position.y += m_Particle[11 + i].Velocity.y;
			
			m_Particle[11 + i].Scale.y -= 0.01f;
			
			m_Particle[i].Life -= 0.6f;
			if (m_Particle[i].Life <= 0)
			{
				m_Particle[i].Enable = false;
			}
		}
	}
}

void ParticleEmitterEnemySpawn::Draw()
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
	material.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
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
			if (PARTICLE_MAX - i <= 11)
				continue;

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
