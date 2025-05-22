//===========================================================
//
//�@�{�X�}�l�[�W���[
// 
//===========================================================

#pragma once

class Boss1;

class BossManager
{
private:
	static Boss1* m_Boss1;
public:
	static void InitBoss();		//������
	static void UninitBoss();		//�I������

	static Boss1* AccessBoss1();	//�{�X�P�����o��

	static BossManager* GetInstance()
	{
		static BossManager manager;
		return &manager;
	}
};