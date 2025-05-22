//===========================================================
//
// エフェクトのプールの管理
// 
//===========================================================

#pragma once
#include "main.h"

#define EXPLOSION_MAX 30

class Explosion;

class EffectManager
{
private:
	static std::vector<Explosion*> m_ExplosionPool;

public:

	static void InitEffect();
	static void UninitEffect();

	static Explosion* AccessExplosion();//プールから取り出す

	static EffectManager* GetInstance()
	{
		static EffectManager manager;
		return &manager;
	}
};