//===========================================================
//
// �G�̈ړ��X�e�[�g
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyMove : public EnemyState
{
private:
	float m_MoveSpeed = 0.15f;

	bool m_IsAvoidObj = false;//������Ă��邩�ǂ���
public:
	EnemyMove(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override;
	void Exit() override {};
	void ImGuiDebug() override;

	XMFLOAT3 GoToPlayer(Enemy* enemy, XMVECTOR enemyVec);//�v���C���[�ւ̃x�N�g��
	XMFLOAT3 AvoidObj(XMVECTOR vec);					 //��Q�����������x�N�g��
	XMFLOAT3 AvoidEnemy(Enemy* enemy, XMVECTOR enemyVec);//���̓G���������x�N�g��

	XMFLOAT3 GetVec(XMVECTOR subtract);//�P�ʃx�N�g����XMFLOAT3�^�ɂ��ĕԂ�
};
