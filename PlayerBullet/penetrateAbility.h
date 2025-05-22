//===========================================================
//
//　貫通アイテムの能力部分、アイテムスロットの描画
// 
//===========================================================

#pragma once
#include "..//manager.h"
#include "..//Scenes//scene.h"
#include "..//renderer.h"
#include "..//player.h"
#include "BulletAbility.h"
#include "PenetrateDecorator.h"
#include "../decoratorManager.h"

class PenetrateAbility : public BulletAbility
{
public:
    Bullet* Apply(Bullet* bullet) override {
		PenetrateDecorator* decorator = DecoratorManager::AccessPenetrateDecorator();
		decorator->SetActive(true);
		decorator->SetParentBullet(bullet);
		decorator->Init();
		return decorator;
    }

    void Init() override
    {
		Scene* scene = Manager::GetScene();
		Player* player = scene->GetGameObject<Player>();
		float num = static_cast<float>(player->GetBulletFactory()->GetDecorators().size());

		VERTEX_3D vertex[4];
		vertex[0].Position = XMFLOAT3(num*100.0f, 500.0f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[1].Position = XMFLOAT3(num * 100.0f +100.0f, 500.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[2].Position = XMFLOAT3(num * 100.0f, 600.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[3].Position = XMFLOAT3(num * 100.0f + 100.0f, 600.0f, 0.0f);
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

		LoadFromWICFile(L"asset\\texture\\PenetrateIcon.png", WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
			"shader\\unlitTextureVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader,
			"shader\\unlitTexturePS.cso");
    }
};