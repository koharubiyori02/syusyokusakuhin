#include "polygon2D.h"
#include "main.h"
#include "renderer.h"

void Polygon2D::Init()
{
	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_Vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(m_TexturePath.c_str(), WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Polygon2D::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Polygon2D::Update()
{

}

void Polygon2D::Draw()
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
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

//2Dポリゴンの線形補間
void Polygon2D::SetVertexPos(XMFLOAT3 start, XMFLOAT3 end, float t)
{
	if (!m_SetPos)
	{
		m_StartPos = start;
		m_EndPos = end;
		m_SetPos = true;
	}

	XMVECTOR startVec = XMLoadFloat3(&m_StartPos);
	XMVECTOR endVec = XMLoadFloat3(&m_EndPos);

	XMVECTOR pos= XMVectorLerp(startVec, endVec, t);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, pos);

	m_Position = newPos;

	if (t >= 1.0f)
	{
		m_SetPos = false;
	}
}

//透明度を変える
void Polygon2D::SetSubject()
{
	if (!m_AlphaBool)
	{
		m_Alpha += 0.01f;
		if (m_Alpha >= 2.0f)
			m_AlphaBool = true;
	}
	else
	{
		m_Alpha -= 0.01f;
		if (m_Alpha == 0.0f)
			m_Moving = false;
	}
}
