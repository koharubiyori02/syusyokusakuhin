//===========================================================
//
//　プレイヤーのアイドルステート
// 
//===========================================================

#pragma once
#include "playerState.h"

class PlayerIdle : public PlayerState
{
private:

public:
	PlayerIdle(Player* p, PlayerStateName name,PlayerShoot* shoot) : PlayerState(p,name,shoot) {}

	void Start() override 
	{ 
		PlayerState::Start(); 
	}
	void Update() override;

	void Exit() override 
	{ 
		delete m_PlayerShoot; 
		
		PlayerState::Exit(); 
	}
};
