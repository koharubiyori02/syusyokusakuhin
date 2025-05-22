//===========================================================
//
//�@�t�F�[�h
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

	float m_Alpha = 1.0f;				//�t�F�[�h�̃A���t�@�l

	FadeType m_FadeName;				//�t�F�[�h�C�����t�F�[�h�A�E�g������

	float FADE_IN_SPEED = 0.02f;
	float FADE_OUT_SPEED = 0.01f;

	bool m_Start = false;				//�t�F�[�h���n�܂������ǂ���
	bool m_FinishFade = false;			//�t�F�[�h���I��������ǂ���

public:
	Fade(FadeType name) : m_FadeName(name) {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetFade(const FadeType type) { m_FadeName = type; }			//�t�F�[�h�C�����t�F�[�h�A�E�g�����Z�b�g
	bool GetFinishFade() const { return m_FinishFade; }					//�t�F�[�h���I��������ǂ���	
};

