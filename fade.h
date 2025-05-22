//===========================================================
//
//　フェード
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "manager.h"

enum class FadeType : unsigned int
{
	FadeIn,
	FadeOut
};

class Fade : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	float m_Alpha = 1.0f;				//フェードのアルファ値

	FadeType m_FadeName;				//フェードインかフェードアウトを入れる

	float FADE_IN_SPEED = 0.02f;
	float FADE_OUT_SPEED = 0.01f;

	bool m_Start = false;				//フェードが始まったかどうか
	bool m_FinishFade = false;			//フェードが終わったかどうか

public:
	Fade(FadeType name) : m_FadeName(name) {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetFade(const FadeType type) { m_FadeName = type; }			//フェードインかフェードアウトかをセット
	bool GetFinishFade() const { return m_FinishFade; }					//フェードが終わったかどうか	
};

