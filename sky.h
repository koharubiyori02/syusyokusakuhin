//===========================================================
//
// スカイドーム
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "component.h"

class Sky : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	XMFLOAT3 DEFAULT_SCALE = { 100.0f,100.0f,100.0f };

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};