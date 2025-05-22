#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "field.h"

void  Field::Init()
{
	VERTEX_3D vertex[VERTEX_MAX];
	//床
	vertex[0].Position = XMFLOAT3(-50.0f, 0.0f, 50.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(50.0f, 0.0f, 50.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(10.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-50.0f, 0.0f, -50.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 10.0f);

	vertex[3].Position = XMFLOAT3(50.0f, 0.0f, -50.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(10.0f, 10.0f);
	

	//ここから壁
	//手前
	vertex[4].Position = XMFLOAT3(-50.0f, 3.0f, -50.0f);//左上
	vertex[4].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[4].TexCoord = XMFLOAT2(0.0f, 0.0f);
		  
	vertex[5].Position = XMFLOAT3(50.0f, 3.0f, -50.0f);//右上
	vertex[5].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[5].TexCoord = XMFLOAT2(10.0f, 0.0f);  

	vertex[6].Position = XMFLOAT3(-50.0f, 0.0f, -50.0f);//左下
	vertex[6].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[6].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[6].TexCoord = XMFLOAT2(0.0f, 1.0f);
		  
	vertex[7].Position = XMFLOAT3(50.0f, 0.0f, -50.0f);//右下
	vertex[7].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[7].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[7].TexCoord = XMFLOAT2(10.0f, 1.0f);

	//右
	vertex[8].Position = XMFLOAT3(50.0f, 3.0f, 50.0f);//右上
	vertex[8].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex[8].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[8].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[9].Position = XMFLOAT3(50.0f, 0.0f, 50.0f);//右下
	vertex[9].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[9].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[9].TexCoord = XMFLOAT2(0.0f, 1.0f);

	//左
	vertex[10].Position = XMFLOAT3(-50.0f,3.0f, 50.0f);//左上
	vertex[10].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[10].TexCoord = XMFLOAT2(10.0f, 0.0f);

	vertex[11].Position = XMFLOAT3(-50.0f, 0.0f, 50.0f);//左下
	vertex[11].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[11].TexCoord = XMFLOAT2(10.0f, 1.0f);

	unsigned short indexArray[] =
	{
		//床
		0,1,2,
		1,3,2,

		4,6,5,
		5,7,6,

		5,8,7,
		8,9,7,

		8,10,9,
		10,11,9,

		10,4,11,
		4,6,11

	};

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FrontCounterClockwise = FALSE;

	ID3D11RasterizerState* rs;
	Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs);

	Renderer::GetDeviceContext()->RSSetState(rs);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_MAX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//インデックスバッファ生成
	D3D11_BUFFER_DESC bd2{};
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(unsigned int) * INDEX_MAX;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd2{};
	ZeroMemory(&sd2, sizeof(sd2));
	sd2.pSysMem = indexArray;
	Renderer::GetDevice()->CreateBuffer(&bd2, &sd2, &m_IndexBuffer);

	

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\field000.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Field::Uninit()
{
	m_IndexBuffer->Release();
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Field::Update()
{

}

void Field::Draw()
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
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(INDEX_MAX, 0, 0);
}
