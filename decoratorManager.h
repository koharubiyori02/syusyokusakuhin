//===========================================================
//
//�@�f�R���[�^�[�̊Ǘ�
// 
//===========================================================

#pragma once
#include "manager.h"

class AtkUpDecorator;
class SpeedUpDecorator;
class PenetrateDecorator;

#define ATKUP_MAX 100
#define SPEEDUP_MAX 100
#define PENETRATE_MAX 15

class DecoratorManager
{
private:
	static std::vector<AtkUpDecorator*> m_AtkUpPool;			//�U����UP�f�R���[�^�[�̃v�[��
	static std::vector<SpeedUpDecorator*> m_SpeedUpPool;		//�X�s�[�hUP�f�R���[�^�[�̃v�[��
	static std::vector<PenetrateDecorator*> m_PenetratePool;	//�ђʃf�R���[�^�[�̃v�[��

public:
	
	static void InitDecorator();
	static void UninitDecorator();

	static AtkUpDecorator* AccessAtkUpDecorator();			//�U����UP�f�R���[�^�[�����o��
	static SpeedUpDecorator* AccessSpeedUpDecorator();		//�X�s�[�hUP�f�R���[�^�[�����o��
	static PenetrateDecorator* AccessPenetrateDecorator();	//�ђʃf�R���[�^�[�����o��

	static DecoratorManager* GetInstance()
	{
		static DecoratorManager manager;
		return &manager;
	}
};
