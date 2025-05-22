//===========================================================
//
//　残り敵数UI
// 
//===========================================================

#pragma once
#include "..//gameObject.h"

class EnemyNumUI : public GameObject
{
private:
	int m_Time{}; //1秒を計算するカウント
	int m_Count{};//何秒でゲームを終わらせるか

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	int GetTime() const { return m_Count; }
};
