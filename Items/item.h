//===========================================================
//
//�@�A�C�e���̊��N���X
// 
//===========================================================

#pragma once
#include "..//gameObject.h"
#include "..//component.h"

class BulletAbility;

class Item : public GameObject
{
private:

protected:
	int m_IsDuplication = 0;//�d���\��0,�s�\��1

public:
	void Init() override {};
	void Uninit() override {};
	void Update() override {};
	void Draw() override {};

	int GetDuplication() { return m_IsDuplication; }
	void SetDuplication(int num) { m_IsDuplication = num; }

	virtual Item* GetClone() = 0;				//���g�𐶐�
	virtual BulletAbility* GetAbility() = 0;	//�e�ɔ\�͕t�^
};