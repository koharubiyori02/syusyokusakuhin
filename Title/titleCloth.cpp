#include "..//main.h"
#include "..//manager.h"
#include "..//renderer.h"
#include "TitleCloth.h"
#include "..//ImGUI/imgui.h"

void TitleCloth::Init()
{
	{
		for (int y = 0; y < 21; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				m_vertex[y][x].Position = XMFLOAT3((x - 10.0f) * 30.0f, (y - 10.0f) * -10.0f, 0);
				m_vertex[y][x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_vertex[y][x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_vertex[y][x].TexCoord = XMFLOAT2(x / 20.0f, y / -20.0f);
			}
		}

		//頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ生成
	{
		unsigned int index[((21 + 1) * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}

			if (x == 19)
				break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		//インデックスバッファ生成
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\titleLogo.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
	
	m_Position = { 650.0f,180.0f,0.0f };
	m_Status = 0;
	m_Wind.status = 0;
	m_Wind.force.x = WIND_FORCE_DEF_X;		// 風力の初期値を設定
	m_Wind.force.y = WIND_FORCE_DEF_Y;
	m_Wind.force.z = WIND_FORCE_DEF_Z;
}

void TitleCloth::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TitleCloth::Update()
{
	XMFLOAT3	vec_spr;			// 粒子２点間のベクトル
	XMFLOAT3	resistance;			// 抵抗力ベクトル
	XMFLOAT3	windforce;			// 風力ベクトル
	float	f1, f2;					// バネが粒子に与える力（２種）
	int		h1, v1, h2, v2;			// バネ端にある粒子のインデックス（横、縦別２個分）
	float	f1x, f1y, f1z;			// 長さからバネに発生する力
	float	f2x, f2y, f2z;
	float	length;					// 粒子間の距離
	float	xx, yy, zz;				// 粒子間の距離（成分毎）
	int		count;					// バネカウント用
	int		x, y;					// ループカウンタ

	//旗の挙動の計算
	switch (m_Status) {
	case 0:			// 初期化
		// 粒子の初期化
		for (y = 0; y <= PARTICLE_VERTICAL; y++) {
			for (x = 0; x <= PARTICLE_HORIZONTAL; x++) {
				// 座標を設定
				m_vertex[y][x].velocity = {};									// 速度初期化
				m_vertex[y][x].acceleration = {};								// 加速度初期化
				m_vertex[y][x].resultant = {};									// 合力初期化
				m_vertex[y][x].gravity = { 0.0f,SPRING_MASS * -9.8f, 0.0f }; 		// 重力初期化
				// 特定の粒子はロック状態へ
				m_vertex[y][x].onLock = x == 0;	// ロック
			}
		}

		// バネの初期化
		count = 0;
		for (y = 0; y <= PARTICLE_VERTICAL; y++) {
			for (x = 0; x <= PARTICLE_HORIZONTAL; x++) {
				// バネ参照インデックスの設定
				if (x < PARTICLE_HORIZONTAL) {
					m_Spring[count].p1.horz = x;		// １個目の参照粒子インデックスを設定
					m_Spring[count].p1.vert = y;
					m_Spring[count].p2.horz = x + 1;	// ２個目の参照粒子インデックスを設定
					m_Spring[count].p2.vert = y;
					// ２個の粒子間の距離を求め、バネの自然長とする
					xx = m_vertex[y][x].Position.x - m_vertex[y][x + 1].Position.x;
					yy = m_vertex[y][x].Position.y - m_vertex[y][x + 1].Position.y;
					zz = m_vertex[y][x].Position.z - m_vertex[y][x + 1].Position.z;
					m_Spring[count].length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
				if (y < PARTICLE_VERTICAL) {
					m_Spring[count].p1.horz = x;
					m_Spring[count].p1.vert = y;
					m_Spring[count].p2.horz = x;
					m_Spring[count].p2.vert = y + 1;
					xx = m_vertex[y][x].Position.x - m_vertex[y + 1][x].Position.x;
					yy = m_vertex[y][x].Position.y - m_vertex[y + 1][x].Position.y;
					zz = m_vertex[y][x].Position.z - m_vertex[y + 1][x].Position.z;
					m_Spring[count].length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
				if (x < PARTICLE_HORIZONTAL && y < PARTICLE_VERTICAL) {
					m_Spring[count].p1.horz = x;
					m_Spring[count].p1.vert = y;
					m_Spring[count].p2.horz = x + 1;
					m_Spring[count].p2.vert = y + 1;
					xx = m_vertex[y][x].Position.x - m_vertex[y + 1][x + 1].Position.x;
					yy = m_vertex[y][x].Position.y - m_vertex[y + 1][x + 1].Position.y;
					zz = m_vertex[y][x].Position.z - m_vertex[y + 1][x + 1].Position.z;
					m_Spring[count].length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
				if (x > 0 && y < PARTICLE_VERTICAL) {
					m_Spring[count].p1.horz = x;
					m_Spring[count].p1.vert = y;
					m_Spring[count].p2.horz = x - 1;
					m_Spring[count].p2.vert = y + 1;
					xx = m_vertex[y][x].Position.x - m_vertex[y + 1][x - 1].Position.x;
					yy = m_vertex[y][x].Position.y - m_vertex[y + 1][x - 1].Position.y;
					zz = m_vertex[y][x].Position.z - m_vertex[y + 1][x - 1].Position.z;
					m_Spring[count].length = sqrtf(xx * xx + yy * yy + zz * zz);
					count++;
				}
			}
		}
		m_Status = 1;
		break;
	case 1:
		// 粒子の処理
		for (y = 0; y <= PARTICLE_VERTICAL; y++) {
			for (x = 0; x <= PARTICLE_HORIZONTAL; x++) {
				// 合力を０にする
				m_vertex[y][x].resultant.x = 0;
				m_vertex[y][x].resultant.y = 0;
				m_vertex[y][x].resultant.z = 0;

				if (!m_vertex[y][x].onLock) {
					// 重力を求める
					m_vertex[y][x].gravity.x = 0.0f;
					m_vertex[y][x].gravity.y = 9.8f * SPRING_MASS;
					m_vertex[y][x].gravity.z = 0.0f;


					// 抵抗力を求める
					resistance.x = -m_vertex[y][x].velocity.x * ATT_COEFFICIENT;
					resistance.y = -m_vertex[y][x].velocity.y * ATT_COEFFICIENT;
					resistance.z = -m_vertex[y][x].velocity.z * ATT_COEFFICIENT;

					//風力を求める
					
					windforce.x = static_cast<float>(rand() % 60 - 20);
					windforce.y = m_Wind.force.y;
					windforce.z = m_Wind.force.z;

					// 合力＝重力＋抵抗力＋風力
					m_vertex[y][x].resultant.x = m_vertex[y][x].gravity.x
						+ resistance.x + windforce.x;
					m_vertex[y][x].resultant.y = m_vertex[y][x].gravity.y
						+ resistance.y + windforce.y;
					m_vertex[y][x].resultant.z = m_vertex[y][x].gravity.z
						+ resistance.z + windforce.z;
				}
			}
		}

		// バネの処理
		for (x = 0; x < SPRING_NUMS; x++) {
			// バネの参照粒子（２個）を取得
			h1 = m_Spring[x].p1.horz;
			v1 = m_Spring[x].p1.vert;
			h2 = m_Spring[x].p2.horz;
			v2 = m_Spring[x].p2.vert;
			// ２個の粒子間のベクトルを求める
			vec_spr.x = m_vertex[v1][h1].Position.x - m_vertex[v2][h2].Position.x;
			vec_spr.y = m_vertex[v1][h1].Position.y - m_vertex[v2][h2].Position.y;
			vec_spr.z = m_vertex[v1][h1].Position.z - m_vertex[v2][h2].Position.z;
			// 粒子間の距離を求める
			length = sqrtf(vec_spr.x * vec_spr.x + vec_spr.y * vec_spr.y + vec_spr.z * vec_spr.z);
			// 距離、自然長、バネ係数からかかる力を求める（２つ目は逆方向）
			f1 = -SPRING_COEFFICIENT * (length - m_Spring[x].length);
			f2 = -f1;

			// ベクトルの成分に力をかける
			f1x = f1 * vec_spr.x / length;
			f1y = f1 * vec_spr.y / length;
			f1z = f1 * vec_spr.z / length;

			f2x = f2 * vec_spr.x / length;
			f2y = f2 * vec_spr.y / length;
			f2z = f2 * vec_spr.z / length;

			// 求めた力を合力に加える
			m_vertex[v1][h1].resultant.x += f1x;
			m_vertex[v1][h1].resultant.y += f1y;
			m_vertex[v1][h1].resultant.z += f1z;

			m_vertex[v2][h2].resultant.x += f2x;
			m_vertex[v2][h2].resultant.y += f2y;
			m_vertex[v2][h2].resultant.z += f2z;
		}

		for (y = 0; y <= PARTICLE_VERTICAL; y++) {
			for (x = 0; x <= PARTICLE_HORIZONTAL; x++) {
				// 固定されていればスキップ
				if (m_vertex[y][x].onLock) continue;

				// 合力と質量から加速度を求める
				m_vertex[y][x].acceleration.x = m_vertex[y][x].resultant.x / SPRING_MASS;
				m_vertex[y][x].acceleration.y = m_vertex[y][x].resultant.y / SPRING_MASS;
				m_vertex[y][x].acceleration.z = m_vertex[y][x].resultant.z / SPRING_MASS;

				// 速度に加速度を加える
				m_vertex[y][x].velocity.x += m_vertex[y][x].acceleration.x * DELTA_TIME;
				m_vertex[y][x].velocity.y += m_vertex[y][x].acceleration.y * DELTA_TIME;
				m_vertex[y][x].velocity.z += m_vertex[y][x].acceleration.z * DELTA_TIME;

				// 速度から座標を移動
				m_vertex[y][x].Position.x += m_vertex[y][x].velocity.x * DELTA_TIME;
				m_vertex[y][x].Position.y += m_vertex[y][x].velocity.y * DELTA_TIME;
				m_vertex[y][x].Position.z += m_vertex[y][x].velocity.z * DELTA_TIME;
			}
		}
		break;
	}


	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX* vertex = (VERTEX*)msr.pData;

	memcpy(vertex, m_vertex, sizeof(VERTEX) * 21 * 21);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void TitleCloth::Draw()
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

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetIndexBuffer(
		m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	//Renderer::GetDeviceContext()->Draw(21*21, 0);
	Renderer::GetDeviceContext()->DrawIndexed(
		(22 * 2) * 20 - 2, 0, 0);
}

void TitleCloth::ImGuiDebug()
{
	static float posx = 650.0f;
	static float posy = 180.0f;

	if (ImGui::TreeNode("Cloth"))
	{
		ImGui::SliderFloat("clothPosX", &posx, 0.0f, 800.0f);
		ImGui::SliderFloat("clothPosY", &posy, 0.0f, 500.0f);
		ImGui::Text("%f", m_Position.x);
		ImGui::Text("%f", m_Position.y);
		ImGui::TreePop();
	}
	m_Position.x = posx;
	m_Position.y = posy;
}
