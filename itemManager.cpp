#include "ItemManager.h"
#include "manager.h"
#include "box.h"

void ItemManager::Init()
{
	//アイテムのファクトリーを生成
	m_ItemFactory = new ItemFactory;
	
	//出現するアイテムを登録
	m_ItemFactory->RoadItem();
}

void ItemManager::Uninit()
{

}

void ItemManager::Update()
{
	m_SpawnFrame++;

	//12秒ごとにアイテムを生成
	if (m_SpawnFrame > SPAWN_DELAY)
	{
		Spawn();//アイテムを生成

		m_SpawnFrame = 0;
	}
}

void ItemManager::Draw()
{

}

//アイテムをランダムな場所に生成
void ItemManager::Spawn()
{
	Scene* scene = Manager::GetScene();

	auto box = scene->GetGameObjects<Box>();

	float boxDist = 1.0f;//障害物と被る距離

	bool canSpawn = true;

	while (1)//障害物と被らなくなるまで座標を変える
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
