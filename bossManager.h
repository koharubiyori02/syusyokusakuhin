//===========================================================
//
//　ボスマネージャー
// 
//===========================================================

#pragma once

class Boss1;

class BossManager
{
private:
	static Boss1* m_Boss1;
public:
	static void InitBoss();		//初期化
	static void UninitBoss();		//終了処理

	static Boss1* AccessBoss1();	//ボス１を取り出す

	static BossManager* GetInstance()
	{
		static BossManager manager;
		return &manager;
	}
};