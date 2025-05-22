//===========================================================
//
//　アイテムマネージャー
// 
//===========================================================

#pragma once
#include "gameObject.h"
#include "Items//itemFactory.h"

#define SPAWN_DELAY 720

class Item;

class ItemManager : public GameObject
{
private:
	ItemFactory* m_ItemFactory;	//アイテムを生成するファクトリー

	int m_SpawnFrame = 0;		//生成間隔

	int SPAWN_POSITION_MAX = 80;//アイテムのポジションの最大値
	int SPAWN_POSITION_MIN = 40;//アイテムのポジションの最低値

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Spawn();				//アイテム生成
};