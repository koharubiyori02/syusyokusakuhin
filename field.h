//===========================================================
//
//　フィールド
// 
//===========================================================

#pragma once

#include "gameObject.h"

#define VERTEX_MAX (12)
#define INDEX_MAX (30)

class Field :public GameObject
{
private:

	ID3D11ShaderResourceView* m_Texture{};
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};