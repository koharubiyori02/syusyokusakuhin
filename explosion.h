//===========================================================
//
//　弾が当たった時のエフェクト
// 
//===========================================================

#pragma once

#include "gameObject.h"

class Explosion : public GameObject
{
private:
	
	ID3D11ShaderResourceView* m_Texture{};
	ID3D11Buffer* m_VertexBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};							//スプライトアニメーションのカウント

	bool m_Active = false;
public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetActive(const bool active) { m_Active = active; }
	bool GetActive() const { return m_Active; }
};