//===========================================================
//
//�@�l�p�`�̓����蔻��
// 
//===========================================================

#pragma once

#include "main.h"
#include "gameObject.h"
#include "component.h"

class HitBox :public Component
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	XMFLOAT3 m_Position{};
	XMFLOAT3 m_Rotation{};
	XMFLOAT3 m_Scale{};

	const float POSITOIN_OFFSET = 4.0f;//�v���C���[�̏����O�̍��W�ɂ���I�t�Z�b�g

	bool m_IsHit = false;		//�����������ǂ���
	bool m_CanDamage = false;   //�����蔻�������Ă������ǂ���

	int m_ValidFrame = 0;		//���t���[�������蔻�肪���邩
	int m_Damage = 0;			//�^����_���[�W��

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void AddDamage();//�_���[�W��^����

	void SetPosition(const XMFLOAT3 pos) { m_Position = pos; }
	void SetScale(const XMFLOAT3 scale) { m_Scale = scale; }
	
	void SetIsHit(const bool hit) { m_IsHit = hit; }

	void SetCanDamage(const bool can) { m_CanDamage = can; }//�_���[�W��^�����邩

	void SetDamageSet(const int damage, const int frame)//�^����_���[�W�Ɠ����蔻����o���t���[�����Z�b�g
	{
		m_Damage = damage;
		m_ValidFrame = frame;
	}
};