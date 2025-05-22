//===========================================================
//
//�@�_���[�W��H��������̉�ʂ̐F
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

	std::wstring texturePath{};	//�e�N�X�`���p�X

	float m_Alpha = 0.5f;		//�A���t�@�l
	
	bool m_Active = false;

	const int EFFECT_COUNT = 15;//���t���[����ʂ�Ԃ����邩
	int m_EffectFrame = 0;		//���̃J�E���g

public:
	

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetActive(const bool active) { m_Active = active; }
};