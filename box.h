//===========================================================
//
// ��Q��
// 
//===========================================================

#pragma once

#include "main.h"
#include "gameObject.h"

class Box :public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11PixelShader* m_PixelShader2{};//�������p�V�F�[�_�[
	ID3D11InputLayout* m_VertexLayout{};

	bool m_HitCamera = false;//�J�����Ɠ������Ă��邩

	XMFLOAT3 DEFAULT_SCALE = { 2.0f, 2.0f, 2.0f };

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};