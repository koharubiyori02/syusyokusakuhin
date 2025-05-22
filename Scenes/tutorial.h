//===========================================================
//
//　チュートリアルシーン
// 
//===========================================================

#pragma once
#include "scene.h"
#include "..//main.h"
#include "..//polygon2D.h"

//チュートリアルのミッション
enum class Mission : unsigned int
{
	Move,
	Shoot,
	Item,
};

class Tutorial : public Scene
{
private:
	Mission m_Mission = Mission::Move;//チュートリアルのミッション

	int m_SpawnItemFrame = 0;//アイテムスポーン用のフレーム
	const int SPAWN_DELAY = 180;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};