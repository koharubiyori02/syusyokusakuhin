//===========================================================
//
//　プレイヤーが持つデコレーターのファクトリー
// 
//===========================================================

#pragma once

#include <list>
#include "BulletDecorator.h"
#include "BulletAbility.h"

class BulletFactory
{
private:
	std::list<BulletAbility*> decorators;
public:
	~BulletFactory() 
	{
		for (auto p : decorators)
		{
			delete p;
		}

		decorators.clear();
	}

	void AddDecorator(BulletAbility* decorator)
	{
		decorators.push_back(decorator);
		decorator->Init();
	}

	Bullet* CreateBullet(XMFLOAT3 position);//弾を生成
		
	std::list<BulletAbility*> GetDecorators() { return decorators; }
};