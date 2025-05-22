//===========================================================
//
// 弾のプールの管理
// 
//===========================================================

#pragma once
#include "manager.h"

class Bullet;
class BulletEnemy;

#define PLAYER_BULLET_MAX 15
#define ENEMY_BULLET_MAX 100

class BulletManager
{
private:
	static std::vector<Bullet*> m_PlayerBulletPool;
	static std::vector<BulletEnemy*> m_EnemyBulletPool;

public:

	static void InitBullet();		//プールの初期化
	static void UninitBullet();		//プールの終了処理

	static Bullet* AccessBullet();				//プレイヤーの弾を取り出す
	static BulletEnemy* AccessBulletEnemy();	//敵の弾を取り出す

	static void ResetBullet();		//プールをリセットする

	static BulletManager* GetInstance()
	{
		static BulletManager manager;
		return &manager;
	}
};