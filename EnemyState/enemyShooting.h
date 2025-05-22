//===========================================================
//
// 敵の弾発射ステート
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyShooting : public EnemyState
{
private:
	float m_MoveDist = 15.0f;//この数値以上離れたらmoveに切り替える

public:
	EnemyShooting(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override;
	void Exit() override;
};
