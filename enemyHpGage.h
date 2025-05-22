//===========================================================
//
// éGãõìGÇÃHPÉQÅ[ÉW
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "component.h"
#include "enemy.h"

class EnemyHpGage : public Component
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

	const float POSITION_Y_OFFSET = 3.0f;
public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};