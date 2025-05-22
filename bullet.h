//===========================================================
//
// �v���C���[�̒e�̃x�[�X
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "component.h"

class Bullet : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	const int BULLET_DEFAULT_ATK = 30;		//�����U����
	const float BULLET_DEFAULT_SPEED = 0.5f;//�����e��

	int m_SpeedUpCount = 0;					//�X�s�[�h�A�b�v�A�C�e���̎擾��
	int m_AtkUpCount = 0;					//�U���̓A�b�v�A�C�e���̎擾��
	float m_SpeedUpValue = 0.5f;			//�X�s�[�h�A�b�v�̒l
	float m_Speed = BULLET_DEFAULT_SPEED;	//�e�̑��x

	bool m_Penetrate = false;				//�ђʂ��邩�ǂ���

	std::vector<GameObject*> m_PenetrateList; //�ђʂ����I�u�W�F�N�g�����Ă���

protected:

	bool m_Active = false;

public:

	Bullet() {};

	void Init() override;
	void Uninit() override;
	virtual void Update() override;
	void Draw() override;

	void PlusSpeed() { m_SpeedUpCount += 1; }
	void PlusAtk() { m_AtkUpCount += 1; }

	virtual Bullet* GetBaseBullet() { return this; }

	void SetPenetrate(const bool frag) { m_Penetrate = frag; }

	void SetActive(const bool active) { m_Active = active; }
	bool const GetActive() { return m_Active; }

	void InitBulletStatus();
};