//===========================================================
//
//�@�v���C���[HPUI
// 
//===========================================================

#pragma once
#include "..//gameObject.h"

class PlayerHpUI : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	struct Print	//�v���C���[��HP�̍\����
	{
		XMFLOAT3 position{};//�|�W�V����
		bool isDraw = true; //�`�悷�邩�ǂ���
	};

	Print m_Print[PLAYER_DEFAULT_HP];

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};