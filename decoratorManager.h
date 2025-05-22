//===========================================================
//
//　デコレーターの管理
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
	static std::vector<AtkUpDecorator*> m_AtkUpPool;			//攻撃力UPデコレーターのプール
	static std::vector<SpeedUpDecorator*> m_SpeedUpPool;		//スピードUPデコレーターのプール
	static std::vector<PenetrateDecorator*> m_PenetratePool;	//貫通デコレーターのプール

public:
	
	static void InitDecorator();
	static void UninitDecorator();

	static AtkUpDecorator* AccessAtkUpDecorator();			//攻撃力UPデコレーターを取り出す
	static SpeedUpDecorator* AccessSpeedUpDecorator();		//スピードUPデコレーターを取り出す
	static PenetrateDecorator* AccessPenetrateDecorator();	//貫通デコレーターを取り出す

	static DecoratorManager* GetInstance()
	{
		static DecoratorManager manager;
		return &manager;
	}
};
