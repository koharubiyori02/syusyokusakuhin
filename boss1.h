//===========================================================
//
// �{�X1
// 
//===========================================================

#pragma once

#include "main.h"
#include "gameObject.h"
#include "BehaviorTree//behaviorTree.h"
#include "BehaviorTree//nodeRegistry.h"
#include "ShootType//shootType.h"

#define BOSS_MAXHP 1500

class Boss1 : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	std::string				m_AnimationName1{};
	std::string				m_AnimationName2{};
	float					m_AnimationBlendRatio = 0.0f;
	int						m_AnimationFrame = 0;

	XMFLOAT3 DEFAULT_POSITION = { 0.0f,0.0f,20.0f };
	XMFLOAT3 DEFAULT_SCALE = { 0.03f,0.03f,0.03f };
	float DEFAULT_SPEED = 0.2f;

	ShootType* shootType;				//�P��

	int m_Frame = 0;					//�ړ��ȊO�̃A�j���[�V�����t���[��
	bool m_Anim = false;				//�A�j���[�V���������ǂ���
	bool m_IsHalfHP = false;			//HP�������ȉ����ǂ���

	BehaviorTree* m_BehaviorTree;
	NodeRegistry* m_Register;

	std::string m_AttackNodeName = "";	//�U���m�[�h�̖��O
	float m_AttackRange = 0.0f;			//�U���͈�
	float m_Speed = DEFAULT_SPEED;		//�ړ��X�s�[�h

	bool m_IsDeath = false;

	bool m_Active = true;

	//�U�����@�ƕK�v�ȋ���
	std::unordered_map<std::string, float> m_AttackRangeMap{
		{"Punch",			 5.0f},
		{"JumpingAttack",	10.0f},
		{"EnemySpawn",		10.0f},
		{"SlapAttack",		 5.0f}
	};

	//�U�����@�Ɣ���̃T�C�Y
	std::unordered_map<std::string, XMFLOAT3> m_AttackScaleMap{
		{"Punch",			 XMFLOAT3(1.0f,1.0f,1.0f)},
		{"JumpingAttack",	XMFLOAT3(10.0f,1.0f,10.0f)},
		{"EnemySpawn",		XMFLOAT3(1.0f,1.0f,1.0f)},
		{"SlapAttack",		 XMFLOAT3(7.0f,1.0f,7.0f)}
	};

	//�U���̎�ނ����Ēu���z��
	std::string m_AttackNodeNameArray[ATTACKTYPE_NUM] = {
		"Punch",				//�ߋ����p���`
		"JumpingAttack",		//�������������э��ݒ@����
		"EnemySpawn",			//�G���G����
		"SlapAttack"			//���̏�Œ@�����Ēe��S���ʂɔ���
	};
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ImGuiDebug() override;

	//�m�[�}���C�Y����XMFLOAT3�ɂ���
	XMFLOAT3 GetVec(XMVECTOR subtract)
	{
		XMVECTOR vec = XMVector3Normalize(subtract);
		XMFLOAT3 totalVec;
		XMStoreFloat3(&totalVec, vec);

		return totalVec;
	}

	//�m�[�h�̏����������ɂ܂Ƃ߂ď������ŌĂ�
	void RegisterNodes();

	bool GetIsDeath() const { return m_IsDeath; }

	bool GetActive() const { return m_Active; }
	void SetActive(const bool active) { m_Active = active; }

	void ResetBoss();//�|�W�V�����Ȃǂ̏�����
};