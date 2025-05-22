//===========================================================
//
// �e�̃v�[���̊Ǘ�
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

	static void InitBullet();		//�v�[���̏�����
	static void UninitBullet();		//�v�[���̏I������

	static Bullet* AccessBullet();				//�v���C���[�̒e�����o��
	static BulletEnemy* AccessBulletEnemy();	//�G�̒e�����o��

	static void ResetBullet();		//�v�[�������Z�b�g����

	static BulletManager* GetInstance()
	{
		static BulletManager manager;
		return &manager;
	}
};