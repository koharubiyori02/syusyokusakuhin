//===========================================================
//
//�@�v���C���[�̈ړ��X�e�[�g
// 
//===========================================================

#pragma once
#include "playerState.h"

class PlayerMove : public PlayerState
{
private:
	int moveCount = 0;
public:
	PlayerMove(Player* p, PlayerStateName name,PlayerShoot* shoot) : PlayerState(p,name,shoot) {}

	void Start() override { PlayerState::Start(); }
	void Update() override ;
	void Exit() override 
	{
		PlayerState::Exit();
	}
};
