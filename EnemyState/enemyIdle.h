//===========================================================
//
// 敵のアイドルステート
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyIdle : public EnemyState
{
private:

public:
	EnemyIdle(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override {};
	void Exit() override {};
};

