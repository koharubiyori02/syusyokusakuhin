//===========================================================
//
// 敵の移動ステート
// 
//===========================================================

#pragma once
#include "enemyState.h"

class EnemyMove : public EnemyState
{
private:
	float m_MoveSpeed = 0.15f;

	bool m_IsAvoidObj = false;//回避しているかどうか
public:
	EnemyMove(Enemy* p) : EnemyState(p) {}

	void Start() override;
	void Update() override;
	void Exit() override {};
	void ImGuiDebug() override;

	XMFLOAT3 GoToPlayer(Enemy* enemy, XMVECTOR enemyVec);//プレイヤーへのベクトル
	XMFLOAT3 AvoidObj(XMVECTOR vec);					 //障害物を回避するベクトル
	XMFLOAT3 AvoidEnemy(Enemy* enemy, XMVECTOR enemyVec);//他の敵を回避するベクトル

	XMFLOAT3 GetVec(XMVECTOR subtract);//単位ベクトルをXMFLOAT3型にして返す
};
