#include "../main.h"
#include "../manager.h"
#include "../input.h"
#include "../renderer.h"
#include "ParticleTitleSelect.h"
#include "../ImGUI/imgui.h"

void ParticleTitleSelect::Init()
{
	VERTEX_3D vertex[4];
	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord= XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\particle.png",WIC_FLAGS_NONE,&metadata,image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
	
	m_TitleSelect = TitleSelect::START;
}

void ParticleTitleSelect::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ParticleTitleSelect::Update()
{
	//パーティクルの全体的座標
	float posX = 0.0f;
	float posY = 0.0f;

	if (m_TitleSelect == TitleSelect::START)
	{
		posX = 800.0f;
		posY = 400.0f;
	}
	else if (m_TitleSelect == TitleSelect::EXIT)
	{
		posX = 800.0f;
		posY = 550.0f;
	}

	int cnt = 1;//パーティクルの生成回数
	static float size = 20.0f;//パーティクルのサイズ
	static float power = 150.0f;//パーティクルの広がり具合
	static int life = 60;//パーティクルの寿命

	int maxVecX = 200;
	int minVecX = 100;
	int maxVecY = 200;
	int minVecY = 100;

#ifdef _DEBUG
	if (ImGui::TreeNode("Particle"))
	{
		ImGui::SliderInt("count", &cnt, 1, 10);
		ImGui::SliderFloat("size", &size, 10.0f, 100.0f);
		ImGui::SliderFloat("power", &power, 10.0f, 1000.0f);
		ImGui::SliderInt("life", &life, 30, 300);
		ImGui::TreePop();
	}
#endif

	//パーティクル生成
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == false)//非アクティブのものを有効にする
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = life;
			m_Particle[i].Position = XMFLOAT2(posX, posY);
			m_Particle[i].Size = size;

			m_Particle[i].Velocity.x = (rand() % maxVecX - minVecX) / power;
			m_Particle[i].Velocity.y = (rand() % maxVecY - minVecY) / power;

			cnt--;
			if (cnt <= 0)//回数分作り終わったら終わり
				break;
		}
	}

	//パーティクル移動
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//移動
			m_Particle[i].Position.x += m_Particle[i].Velocity.x;
			m_Particle[i].Position.y += m_Particle[i].Velocity.y;
			m_Particle[i].Size -= size / 100.0f;
			//ライフ
			m_Particle[i].Life--;
			if (m_Particle[i].Life == 0)//ライフが0になったら非アクティブにする
			{
				m_Particle[i].Enable = false;
			}
		}
	}

	//キー入力で選択を変える
	if (m_TitleSelect == TitleSelect::START)
	{
		if (Input::GetKeyTrigger(VK_DOWN))
		{
			m_TitleSelect = TitleSelect::EXIT;

			for (int i = 0; i < PARTICLE_MAX; i++)
			{
				if (m_Particle[i].Enable == true)//最初からパーティクルを出す
				{
					m_Particle[i].Enable = false;
				}
			}
		}
	}
	else if (m_TitleSelect == TitleSelect::EXIT)
	{
		if (Input::GetKeyTrigger(VK_UP))
		{
			m_TitleSelect = TitleSelect::START;
			for (int i = 0; i < PARTICLE_MAX; i++)
			{
				if (m_Particle[i].Enable == true)//最初からパーティクルを出す
				{
					m_Particle[i].Enable = false;
				}
			}
		}
	}
}

void ParticleTitleSelect::Draw()
{
	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//加算合成有効
	Renderer::SetBlendAddEnable(true);

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//頂点データ書き換え
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
				D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			vertex[0].Position = XMFLOAT3(m_Particle[i].Position.x - m_Particle[i].Size, m_Particle[i].Position.y - m_Particle[i].Size, 0.0f);
			vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[0].TexCoord = XMFLOAT2(0, 0);

			vertex[1].Position = XMFLOAT3(m_Particle[i].Position.x + m_Particle[i].Size, m_Particle[i].Position.y - m_Particle[i].Size, 0.0f);
			vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

			vertex[2].Position = XMFLOAT3(m_Particle[i].Position.x - m_Particle[i].Size, m_Particle[i].Position.y + m_Particle[i].Size, 0.0f);
			vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

			vertex[3].Position = XMFLOAT3(m_Particle[i].Position.x + m_Particle[i].Size, m_Particle[i].Position.y + m_Particle[i].Size, 0.0f);
			vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

			//ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	Renderer::SetBlendAddEnable(false);
}