#include "ItemManager.h"
#include "manager.h"
#include "box.h"

void ItemManager::Init()
{
	//�A�C�e���̃t�@�N�g���[�𐶐�
	m_ItemFactory = new ItemFactory;
	
	//�o������A�C�e����o�^
	m_ItemFactory->RoadItem();
}

void ItemManager::Uninit()
{

}

void ItemManager::Update()
{
	m_SpawnFrame++;

	//12�b���ƂɃA�C�e���𐶐�
	if (m_SpawnFrame > SPAWN_DELAY)
	{
		Spawn();//�A�C�e���𐶐�

		m_SpawnFrame = 0;
	}
}

void ItemManager::Draw()
{

}

//�A�C�e���������_���ȏꏊ�ɐ���
void ItemManager::Spawn()
{
	Scene* scene = Manager::GetScene();

	auto box = scene->GetGameObjects<Box>();

	float boxDist = 1.0f;//��Q���Ɣ�鋗��

	bool canSpawn = true;

	while (1)//��Q���Ɣ��Ȃ��Ȃ�܂ō��W��ς���
	{
		float x = static_cast<float>(rand() % SPAWN_POSITION_MAX - SPAWN_POSITION_MIN);
		float z = static_cast<float>(rand() % SPAWN_POSITION_MAX - SPAWN_POSITION_MIN);

		for (auto p : box)
		{
			float posX = std::abs(p->GetPosition().x - static_cast<float>(x));
			float posZ = std::abs(p->GetPosition().z - static_cast<float>(z));

			if (posX < boxDist && posZ < boxDist)
			{
				canSpawn = false;
				break;
			}
			else
			{
				canSpawn = true;
			}
		}

		if (canSpawn)
		{
			scene->Add(m_ItemFactory->CreateItem())->SetPosition(XMFLOAT3(x, 0.0f, z));
			break;
		}
	}
}
