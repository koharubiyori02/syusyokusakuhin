#include "../manager.h"
#include "../bulletManager.h"
#include "bulletFactory.h"

//弾を生成
Bullet* BulletFactory::CreateBullet(XMFLOAT3 position)
{
	Scene* scene = Manager::GetScene();

	Bullet* bullet = BulletManager::AccessBullet();//基本の弾を生成
	bullet->InitBulletStatus();

	for (auto decorator : decorators)//取得しているアイテムをラップする
	{
		bullet = decorator->Apply(bullet);
	}

	scene->AddNoInit(bullet);//ラップし終わった弾をGameObjectに登録する
	return bullet;
}