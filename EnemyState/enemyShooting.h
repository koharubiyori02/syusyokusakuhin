//===========================================================
//
// �G�̒e���˃X�e�[�g
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyShooting : public EnemyState
{
private:
	float m_MoveDist = 15.0f;//���̐��l�ȏ㗣�ꂽ��move�ɐ؂�ւ���

public:
	EnemyShooting(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override;
	void Exit() override;
};
