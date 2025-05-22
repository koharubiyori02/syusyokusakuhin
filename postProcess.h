//===========================================================
//
//　ダメージを食らった時の画面の色
// 
//===========================================================

#pragma once
#include "gameObject.h"
#include "main.h"
#include "renderer.h"

class PostProcess : public GameObject
{
private:
	

	ID3D11Buffer* m_VertexBuffer = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	std::wstring texturePath{};	//テクスチャパス

	float m_Alpha = 0.5f;		//アルファ値
	
	bool m_Active = false;

	const int EFFECT_COUNT = 15;//何フレーム画面を赤くするか
	int m_EffectFrame = 0;		//そのカウント

public:
	

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetActive(const bool active) { m_Active = active; }
};