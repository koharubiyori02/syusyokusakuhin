//===========================================================
//
// 敵のプレイヤーに向くステート
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyPrepare : public EnemyState
{
private:
	float m_Frame = 0.0f;			//補間用のフレーム
	XMFLOAT3 m_StartForward = {};	//最初の向き
	XMFLOAT3 m_EndForward = {};		//最後の向き
public:
	EnemyPrepare(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override;
	void Exit() override {};
};