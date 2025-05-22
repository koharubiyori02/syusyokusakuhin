#include "AbilityUI.h"
#include "..//renderer.h"
#include "..//manager.h"
#include "..//player.h"

void AbilityUI::Init()
{

}

void AbilityUI::Uninit()
{

}

void AbilityUI::Update()
{

}

void AbilityUI::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//持っているアイテムの数ぶんアイテムを表示する
	auto list = player->GetBulletFactory()->GetDecorators();

	for (auto p : list)
	{
		p->Draw();
	}
	
}
