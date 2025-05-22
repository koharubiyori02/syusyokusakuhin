//===========================================================
//
// �G�̃v���C���[�Ɍ����X�e�[�g
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyPrepare : public EnemyState
{
private:
	float m_Frame = 0.0f;			//��ԗp�̃t���[��
	XMFLOAT3 m_StartForward = {};	//�ŏ��̌���
	XMFLOAT3 m_EndForward = {};		//�Ō�̌���
public:
	EnemyPrepare(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override;
	void Exit() override {};
};