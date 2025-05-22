//===========================================================
//
//　エネミースポナー
// 
//===========================================================

#pragma once

#include "gameObject.h"
#include "Scenes//game.h"

#define SPAWN_NUM 3
#define ENEMY_MAX1 9
#define ENEMY_MAX2 12
#define ENEMY_MAX3 15

class EnemySpawner : public GameObject
{
private:

	int m_SpawnCount = 0;		//召喚数
	int m_SpawnFrame = 0;		//召喚間隔

	const int SPAWN_DELAY = 600;//スポーン間隔

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Spawn();				//召喚用関数
};