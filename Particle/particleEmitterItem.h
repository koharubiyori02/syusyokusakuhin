//===========================================================
//
//　アイテムのパーティクル
// 
//===========================================================

#pragma once
#include "..//component.h"
#include "..//main.h"

class ParticleEmitterItem : public Component
{
private:
	ID3D11ShaderResourceView* m_Texture{};
	ID3D11Buffer* m_VertexBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};
	int m_InitCount{};

	struct PARTICLE		//パーティクルの基本情報
	{
		bool		 Enable = false;
		int			 Life;
		float		 A;
		XMFLOAT3	 Position;
		XMFLOAT3	 Velocity;
		XMFLOAT3	 Scale;
	};
	static const int PARTICLE_MAX = 1000;	//最大パーティクル数
	PARTICLE m_Particle[PARTICLE_MAX]{};

	XMFLOAT4 m_Color{};
public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetColor(const XMFLOAT4 color)	//パーティクルの色をセット
	{
		m_Color = color;
	}
};
