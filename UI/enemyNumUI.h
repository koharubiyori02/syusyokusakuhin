//===========================================================
//
//�@�c��G��UI
// 
//===========================================================

#pragma once
#include "..//gameObject.h"

class EnemyNumUI : public GameObject
{
private:
	int m_Time{}; //1�b���v�Z����J�E���g
	int m_Count{};//���b�ŃQ�[�����I��点�邩

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	int GetTime() const { return m_Count; }
};
