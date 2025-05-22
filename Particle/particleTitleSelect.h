//===========================================================
//
//　タイトルのセレクト中のパーティクル
// 
//===========================================================

#pragma once
#include "..//gameObject.h"

enum class TitleSelect : unsigned int
{
	START,
	EXIT,
};

class ParticleTitleSelect :public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader{};
	ID3D11PixelShader*			m_PixelShader{};
	ID3D11InputLayout*			m_VertexLayout{};

	TitleSelect m_TitleSelect;//選択中のセレクト

	int m_Count{};

	struct PARTICLE//選択中のところに表示するパーティクル
	{
		bool		 Enable = false;
		int			 Life;
		XMFLOAT2	 Position;
		XMFLOAT2	 Velocity;
		float		 Size;
	};
	static const int PARTICLE_MAX = 1000;

	PARTICLE m_Particle[PARTICLE_MAX]{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	TitleSelect GetTitleSelect() const
	{
		return m_TitleSelect;
	}
};
