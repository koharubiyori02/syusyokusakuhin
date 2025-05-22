//===========================================================
//
//　フェーズUI
// 
//===========================================================

#pragma once
#include "..//gameObject.h"

class PhaseUI : public GameObject
{
private:
	int m_PhaseNum = 0;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
