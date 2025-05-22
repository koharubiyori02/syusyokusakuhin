//===========================================================
//
// “G‚ÌƒXƒe[ƒg‚ÌŠî’êƒNƒ‰ƒX
// 
//===========================================================

#pragma once
#include "..//main.h"

class Enemy;

class EnemyState
{
private:
	Enemy* m_Enemy;

protected:

	Enemy* GetEnemy() { return m_Enemy; }

	float m_Range = 10.0f;//’T’m”ÍˆÍ
	float m_ContactRange = 5.0f;//“G“¯m‚ÌÅ’á‹——£ŠÔ

public:
	EnemyState(Enemy* p) : m_Enemy(p) {}
	~EnemyState() {}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void ImGuiDebug() {};

	float GetDistance(float x1, float x2, float z1, float z2)
	{
		return sqrtf((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));
	}
};
