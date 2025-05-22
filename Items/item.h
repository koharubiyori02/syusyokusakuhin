//===========================================================
//
//　アイテムの基底クラス
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
	int m_IsDuplication = 0;//重複可能は0,不可能は1

public:
	void Init() override {};
	void Uninit() override {};
	void Update() override {};
	void Draw() override {};

	int GetDuplication() { return m_IsDuplication; }
	void SetDuplication(int num) { m_IsDuplication = num; }

	virtual Item* GetClone() = 0;				//自身を生成
	virtual BulletAbility* GetAbility() = 0;	//弾に能力付与
};