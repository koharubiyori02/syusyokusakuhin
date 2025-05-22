//===========================================================
//
//　土埃のパーティクル
// 
//===========================================================

#pragma once

#include "..//gameObject.h"
#include "..//component.h"

class ParticleEmitterTutibokori : public Component
{
private:

	ID3D11ShaderResourceView* m_Texture{};
	ID3D11Buffer* m_VertexBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	XMFLOAT3 m_Position{};
	XMFLOAT3 m_Rotation{};
	XMFLOAT3 m_Scale{};

	int m_Count{};

	struct PARTICLE		//パーティクルの基本情報
	{
		bool		 Enable=false;
		int			 Life;
		XMFLOAT3	 Position;
		XMFLOAT3	 Velocity;
	};
	static const int PARTICLE_MAX = 100;	//最大パーティクル数
	PARTICLE m_Particle[PARTICLE_MAX]{};

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};