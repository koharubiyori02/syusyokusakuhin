#include "../manager.h"
#include "../bulletManager.h"
#include "bulletFactory.h"

//�e�𐶐�
Bullet* BulletFactory::CreateBullet(XMFLOAT3 position)
{
	Scene* scene = Manager::GetScene();

	Bullet* bullet = BulletManager::AccessBullet();//��{�̒e�𐶐�
	bullet->InitBulletStatus();

	for (auto decorator : decorators)//�擾���Ă���A�C�e�������b�v����
	{
		bullet = decorator->Apply(bullet);
	}

	scene->AddNoInit(bullet);//���b�v���I������e��GameObject�ɓo�^����
	return bullet;
}