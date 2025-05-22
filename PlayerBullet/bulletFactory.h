//===========================================================
//
//�@�v���C���[�����f�R���[�^�[�̃t�@�N�g���[
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

	Bullet* CreateBullet(XMFLOAT3 position);//�e�𐶐�
		
	std::list<BulletAbility*> GetDecorators() { return decorators; }
};